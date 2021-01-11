#include "AsyncFileOperation.h"
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>

using namespace base;

AsyncFileOperation::Type* AsyncFileOperation::Type::createOrClear = new AsyncFileOperation::Type(1, true/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::deleteIfExists = new AsyncFileOperation::Type(2, true/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::rename = new AsyncFileOperation::Type(3, true/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::renameOrMove = new AsyncFileOperation::Type(4, true/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::write_Buffer = new AsyncFileOperation::Type(5, true/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::write_string = new AsyncFileOperation::Type(6, true/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::write_stringstream = new AsyncFileOperation::Type(7, true/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::createBoundedDirectories = new AsyncFileOperation::Type(8, true/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::readBinarySPAsync = new AsyncFileOperation::Type(9, false/*isCacheWriteLockUsed*/);
AsyncFileOperation::Type* AsyncFileOperation::Type::readStringSPAsync = new AsyncFileOperation::Type(10, false/*isCacheWriteLockUsed*/);

AsyncFileOperation::AsyncFileOperation(
	Type* type,
	std::shared_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher)
	: type(type),
	fHandler(fHandler), underlyingBlockingFH(underlyingBlockingFH), undelyingEventDispatcher(undelyingEventDispatcher)
{
	if (fHandler == nullptr) {
		throw LogicException(LOC);
	}
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_createOrClear(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::createOrClear, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_deleteIfExists(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::deleteIfExists, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_rename(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, std::string const& newResourceName, bool replaceIfExists) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::rename, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	asyncOperation->newResourceName = newResourceName;
	asyncOperation->replaceIfExists = replaceIfExists;

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_renameOrMove(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, std::string const& newPath, bool replaceIfExists) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::renameOrMove, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	asyncOperation->newPath = newPath;
	asyncOperation->replaceIfExists = replaceIfExists;

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_write(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, base::Buffer data) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::write_Buffer, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	asyncOperation->data_Buffer = data;

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_write(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, std::shared_ptr<std::string> data) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::write_string, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	asyncOperation->data_spString = data;
	asyncOperation->data_spStringB = nullptr;

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_write(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, sp<std::string> data) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::write_string, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	asyncOperation->data_spString = nullptr;
	asyncOperation->data_spStringB = data;

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_write(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, std::shared_ptr<std::stringstream> data) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::write_stringstream, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	asyncOperation->data_stringstream = data;

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_createBoundedDirectories(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::createBoundedDirectories, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_readBinarySPAsync(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::readBinarySPAsync, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	return asyncOperation;
}

std::shared_ptr<AsyncFileOperation> AsyncFileOperation::newSP_readStringSPAsync(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher) {
	if (fHandler.expired()) {
		// The fHandler w_p is invalid. Make sure the var is set when the fHandler was created.
		throw LogicException(LOC);
	}

	std::shared_ptr<AsyncFileOperation> asyncOperation = std::make_shared<AsyncFileOperation>(
		Type::readStringSPAsync, 
		fHandler.lock(), underlyingBlockingFH, undelyingEventDispatcher);

	return asyncOperation;
}

AsyncFileOperation::~AsyncFileOperation() {
	//void
}
