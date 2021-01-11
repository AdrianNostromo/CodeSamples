#include "FileSystemAsyncOperationsQueue.h"
#include <base/fileSystem/fileHandler/async/FileHandlerAsyncHdd.h>
#include <base/fileSystem/fileHandler/async/util/AsyncFileOperation.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/fileSystem/fileHandler/util/FileIOProfilerStatics.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>
#include <base/app/IApp.h>
#include <base/threadsManager/IThreadsManager.h>
#include <base/threadsManager/managedThread/IManagedThread.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/IOException.h>

using namespace base;

const bool FileSystemAsyncOperationsQueue::IS_DEBUG = true;

FileSystemAsyncOperationsQueue::FileSystemAsyncOperationsQueue()
	: super()
{
	//void
}

void FileSystemAsyncOperationsQueue::createMain() {
	super::createMain();

	managedThread = app->getThreadsManager()->newThread("FileSystem_AsyncOperations", std::bind(&FileSystemAsyncOperationsQueue::threadTick, this, std::placeholders::_1));
}

unsigned int FileSystemAsyncOperationsQueue::threadTick(IManagedThread* managedThread) {
	flush();

	return 10/*sleepDurationMs*/;
}

void FileSystemAsyncOperationsQueue::pushAsyncOperation(std::shared_ptr<AsyncFileOperation> asyncOperation) {
	if (asyncOperation->type->isCacheWriteLockUsed) {
		asyncOperation->fHandler->cacheGroupWriteLockingOperationsCountChange(true);
	}
	asyncOperationsQueueList.appendReference(asyncOperation);
}

bool FileSystemAsyncOperationsQueue::flush() {
	bool ret = false;

	ListDL_ThreadSafe<std::shared_ptr<AsyncFileOperation>>::Entry* assyncOperation;
	while ((assyncOperation = asyncOperationsQueueList.getFirst()) != nullptr) {
		processAsyncOperation(assyncOperation->data);
		
		if (assyncOperation->data->type->isCacheWriteLockUsed) {
			assyncOperation->data->fHandler->cacheGroupWriteLockingOperationsCountChange(false);
		}

		assyncOperation->remove();

		ret = true;
	}

	return ret;
}

void FileSystemAsyncOperationsQueue::processAsyncOperation(std::shared_ptr<AsyncFileOperation> asyncOperation) {
	if (asyncOperation == nullptr) {
		throw LogicException(LOC);
	}

	if (asyncOperation->type == AsyncFileOperation::Type::createOrClear) {
		asyncOperation->underlyingBlockingFH->createOrClear();
	} else if (asyncOperation->type == AsyncFileOperation::Type::deleteIfExists) {
		asyncOperation->underlyingBlockingFH->deleteIfExists();
	} else if (asyncOperation->type == AsyncFileOperation::Type::rename) {
		asyncOperation->underlyingBlockingFH->rename(asyncOperation->newResourceName, asyncOperation->replaceIfExists);
	} else if (asyncOperation->type == AsyncFileOperation::Type::renameOrMove) {
		asyncOperation->underlyingBlockingFH->renameOrMove(asyncOperation->newPath, asyncOperation->replaceIfExists);
	} else if (asyncOperation->type == AsyncFileOperation::Type::write_Buffer) {
		asyncOperation->underlyingBlockingFH->write(asyncOperation->data_Buffer);
	} else if (asyncOperation->type == AsyncFileOperation::Type::write_string) {
		if (asyncOperation->data_spString != nullptr) {
			asyncOperation->underlyingBlockingFH->write(asyncOperation->data_spString);
		} else if (asyncOperation->data_spStringB != nullptr) {
			asyncOperation->underlyingBlockingFH->write(asyncOperation->data_spStringB);
		} else {
			throw LogicException(LOC);
		}
	} else if (asyncOperation->type == AsyncFileOperation::Type::write_stringstream) {
		asyncOperation->underlyingBlockingFH->write(asyncOperation->data_stringstream);
	} else if (asyncOperation->type == AsyncFileOperation::Type::createBoundedDirectories) {
		asyncOperation->underlyingBlockingFH->createBoundedDirectories();
	} else if (asyncOperation->type == AsyncFileOperation::Type::readBinarySPAsync) {
		base::Buffer data = nullptr;
		bool isIOFailed = false;
		try {
			data = asyncOperation->underlyingBlockingFH->readBinarySP();
		} catch (FileNotFoundException& x) {
			isIOFailed = true;
		} catch (IOException& x) {
			isIOFailed = true;
		}

		if (isIOFailed) {
			int r = asyncOperation->undelyingEventDispatcher->getDispatcher(IFileHandlerAsync::EVENT_IO_failed->type, true)
				->dispatch(*IFileHandlerAsync::EVENT_IO_failed);
			if (r <= 0) {
				// At least 1 listener must exist.
				throw LogicException(LOC);
			}
		} else {
			IFileHandlerAsync::EVENT_read_binary_success->data_binary = data;
			int r = asyncOperation->undelyingEventDispatcher->getDispatcher(IFileHandlerAsync::EVENT_read_binary_success->type, true)
				->dispatch(*IFileHandlerAsync::EVENT_read_binary_success);
			if (r <= 0) {
				// At least 1 listener must exist.
				throw LogicException(LOC);
			}
		}
	} else if (asyncOperation->type == AsyncFileOperation::Type::readStringSPAsync) {
		std::shared_ptr<std::string> data = nullptr;
		bool isIOFailed = false;
		try {
			data = asyncOperation->underlyingBlockingFH->readCppStringSP();
		} catch (FileNotFoundException& x) {
			isIOFailed = true;
		} catch (IOException& x) {
			isIOFailed = true;
		}


		if (isIOFailed) {
			int r = asyncOperation->undelyingEventDispatcher->getDispatcher(IFileHandlerAsync::EVENT_IO_failed->type, true)
				->dispatch(*IFileHandlerAsync::EVENT_IO_failed);
			if (r <= 0) {
				// At least 1 listener must exist.
				throw LogicException(LOC);
			}
		} else {
			IFileHandlerAsync::EVENT_read_string_success->data_string = data;
			int r = asyncOperation->undelyingEventDispatcher->getDispatcher(IFileHandlerAsync::EVENT_read_string_success->type, true)
				->dispatch(*IFileHandlerAsync::EVENT_read_string_success);
			if (r <= 0) {
				// At least 1 listener must exist.
				throw LogicException(LOC);
			}
		}
	} else {
		throw LogicException(LOC);
	}

	base::FileIOProfilerStatics::COUNTER_FileIOAsyncTotal++;
}

void FileSystemAsyncOperationsQueue::disposeMain() {
	if (asyncOperationsQueueList.count() > 0) {
		// This should never occur, active fHandlers are saved in a fileSystem list until they are fully processed.
		throw LogicException(LOC);
	}

	if (managedThread != nullptr) {
		managedThread->removeManaged();
		managedThread = nullptr;
	}

	super::disposeMain();
}

FileSystemAsyncOperationsQueue::~FileSystemAsyncOperationsQueue() {
	//void
}
