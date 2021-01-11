#pragma once

#include <base/gh.h>
#include <base/fileSystem/fileHandler/async/FileHandlerAsyncBase.h>

namespace base {
	class IAsyncHandler;
	class AsyncFileOperation;
	class IFileHandlerBlocking;
};

namespace base {
class FileHandlerAsyncHdd : public FileHandlerAsyncBase {priv typedef FileHandlerAsyncBase super;pub dCtor(FileHandlerAsyncHdd);
	pub std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH;
	
	// Use shared_ptr to avoid large map block memory usage on change.
	priv std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> eventDispatcher = std::make_shared<base::TypedEventDispatcher1D<base::AsyncFHEvent>>();

	prot IAsyncHandler* asyncHandler;

    pub explicit FileHandlerAsyncHdd(
		std::string const& path, 
		IAsyncHandler* asyncHandler,
		std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH,
		base::ICacheGroup* cacheGroup);

	pub std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> getEventDispatcher() override;

	pub std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb) override;
	pub void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	pub void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	pub void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	pub void addEventListener(
		ArrayList<int>& eventTypesList,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	pub int dispatchEvent(base::AsyncFHEvent& event) override;

	pub void createOrClear() override;
	pub void deleteIfExists() override;

	pub void rename(std::string const& newResourceName, bool replaceIfExists = false) override;
	pub void renameOrMove(std::string const& newPath, bool replaceIfExists = false) override;

	pub void write(base::Buffer& data) final;
	pub void write(std::shared_ptr<std::string> data) final;
	pub void write(sp<std::string> data) final;
	pub void write(std::shared_ptr<std::stringstream> data) final;

	pub void createBoundedDirectories() override;

	pub void readBinarySPAsync() override;
	pub void readStringSPAsync() override;

    pub ~FileHandlerAsyncHdd() override;
};
};
