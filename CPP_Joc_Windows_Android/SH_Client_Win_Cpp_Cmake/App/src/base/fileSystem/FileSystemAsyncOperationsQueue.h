#pragma once

#include <base/gh.h>
#include "FileSystemBase.h"
#include <base/fileSystem/fileHandler/async/util/IAsyncHandler.h>
#include <memory>
#include <base/list/ListDL_ThreadSafe.h>

namespace base {
	class FileHandlerAsyncHdd;
	class AsyncFileOperation;
	class IManagedThread;
};

namespace base {
class FileSystemAsyncOperationsQueue : public FileSystemBase, virtual public IAsyncHandler {priv typedef FileSystemBase super;pub dCtor(FileSystemAsyncOperationsQueue);
	priv static const bool IS_DEBUG;
	
	priv ListDL_ThreadSafe<std::shared_ptr<AsyncFileOperation>> asyncOperationsQueueList{};

	IManagedThread* managedThread = nullptr;

    pub explicit FileSystemAsyncOperationsQueue();
	prot void createMain() override;

	priv unsigned int threadTick(IManagedThread* managedThread);

	pub void pushAsyncOperation(std::shared_ptr<AsyncFileOperation> asyncOperation) override;

	// This must be called by native (Probably a dedicated thread with a 10ms sleep).
	pub bool flush() final;
	priv void processAsyncOperation(std::shared_ptr<AsyncFileOperation> asyncOperation);

	prot void disposeMain() override;
    pub ~FileSystemAsyncOperationsQueue() override;

};
};
