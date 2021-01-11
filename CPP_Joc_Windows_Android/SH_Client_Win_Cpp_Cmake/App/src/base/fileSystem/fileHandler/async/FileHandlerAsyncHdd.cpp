#include "FileHandlerAsyncHdd.h"
#include <base/exceptions/LogicException.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/fileSystem/fileHandler/async/util/AsyncFileOperation.h>
#include <base/fileSystem/fileHandler/async/util/IAsyncHandler.h>

using namespace base;

FileHandlerAsyncHdd::FileHandlerAsyncHdd(
	std::string const& path,
	IAsyncHandler* asyncHandler,
	std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH,
	base::ICacheGroup* cacheGroup)
	: super(path, cacheGroup),
	underlyingBlockingFH(underlyingBlockingFH),
	asyncHandler(asyncHandler)
{
	//void
}

std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> FileHandlerAsyncHdd::getEventDispatcher() {
	return eventDispatcher;
}

std::shared_ptr<IEventListener> FileHandlerAsyncHdd::addEventListener(
	int type,
	std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb)
{
	return eventDispatcher->getDispatcher(type, true)->addListener(cb);
}

void FileHandlerAsyncHdd::addEventListener(
	int type,
	std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	eventDispatcher->getDispatcher(type, true)->addListener(cb, &autoListenersList);
}

void FileHandlerAsyncHdd::addEventListener(
	int type1, int type2,
	std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	addEventListener(type1, cb, autoListenersList);
	addEventListener(type2, cb, autoListenersList);
}

void FileHandlerAsyncHdd::addEventListener(
	int type1, int type2, int type3,
	std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	addEventListener(type1, cb, autoListenersList);
	addEventListener(type2, cb, autoListenersList);
	addEventListener(type3, cb, autoListenersList);
}

void FileHandlerAsyncHdd::addEventListener(
	ArrayList<int>& eventTypesList,
	std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	for (int i = 0; i < eventTypesList.count(); i++) {
		int type = eventTypesList.getDirect(i);

		addEventListener(type, cb, autoListenersList);
	}
}

int FileHandlerAsyncHdd::dispatchEvent(base::AsyncFHEvent& event) {
	int ct = 0;

	base::EventDispatcher<base::AsyncFHEvent>* d = eventDispatcher->getDispatcher(event.type, false);
	if (d != nullptr) {
		ct += d->dispatch(event);
	}

	event.clearData();

	return ct;
}

void FileHandlerAsyncHdd::createOrClear() {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_createOrClear(self_weakPointer, underlyingBlockingFH, eventDispatcher));
}

void FileHandlerAsyncHdd::deleteIfExists() {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_deleteIfExists(self_weakPointer, underlyingBlockingFH, eventDispatcher));
}

void FileHandlerAsyncHdd::rename(std::string const& newResourceName, bool replaceIfExists) {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_rename(self_weakPointer, underlyingBlockingFH, eventDispatcher, newResourceName, replaceIfExists));
}

void FileHandlerAsyncHdd::renameOrMove(std::string const& newPath, bool replaceIfExists) {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_renameOrMove(self_weakPointer, underlyingBlockingFH, eventDispatcher, newPath, replaceIfExists));
}

void FileHandlerAsyncHdd::write(base::Buffer& data) {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_write(self_weakPointer, underlyingBlockingFH, eventDispatcher, data));
}

void FileHandlerAsyncHdd::write(std::shared_ptr<std::string> data) {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_write(self_weakPointer, underlyingBlockingFH, eventDispatcher, data));
}

void FileHandlerAsyncHdd::write(sp<std::string> data) {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_write(self_weakPointer, underlyingBlockingFH, eventDispatcher, data));
}

void FileHandlerAsyncHdd::write(std::shared_ptr<std::stringstream> data) {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_write(self_weakPointer, underlyingBlockingFH, eventDispatcher, data));
}

void FileHandlerAsyncHdd::createBoundedDirectories() {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_createBoundedDirectories(self_weakPointer, underlyingBlockingFH, eventDispatcher));
}

void FileHandlerAsyncHdd::readBinarySPAsync() {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_readBinarySPAsync(self_weakPointer, underlyingBlockingFH, eventDispatcher));
}

void FileHandlerAsyncHdd::readStringSPAsync() {
	asyncHandler->pushAsyncOperation(AsyncFileOperation::newSP_readStringSPAsync(self_weakPointer, underlyingBlockingFH, eventDispatcher));
}

FileHandlerAsyncHdd::~FileHandlerAsyncHdd() {
	//void
}
