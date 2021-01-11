#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <base/buffer/Buffer.h>
#include <base/buffer/Buffer.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/fileSystem/fileHandler/async/util/AsyncFHEvent.h>
#include <base/sp.h>

namespace base {
	class IFileHandlerBlocking;
	class IFileHandlerAsync;
}

namespace base {
class AsyncFileOperation {
	pub class Type {
		pub static Type* createOrClear;

		pub static Type* deleteIfExists;

		pub static Type* rename;
		pub static Type* renameOrMove;

		pub static Type* write_Buffer;
		pub static Type* write_string;
		pub static Type* write_stringstream;

		pub static Type* createBoundedDirectories;

		pub static Type* readBinarySPAsync;
		pub static Type* readStringSPAsync;

		pub int id;

		pub bool isCacheWriteLockUsed;

		pub explicit Type(int id, bool isCacheWriteLockUsed)
			: id(id), isCacheWriteLockUsed(isCacheWriteLockUsed)
		{};

	};

	pub Type* type;

	// Also save a s_p of the fHandler to make sure it doesn't get deleted if the user doesn't save the received s_p when making the file I.O. calls.
	pub std::shared_ptr<IFileHandlerAsync> fHandler = nullptr;

	pub std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH;
	pub std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher;

	pub std::string newResourceName;
	pub std::string newPath;
	pub bool replaceIfExists;

	pub base::Buffer data_Buffer;
	pub std::shared_ptr<std::string> data_spString;
	pub sp<std::string> data_spStringB;
	pub std::shared_ptr<std::stringstream> data_stringstream;

	pub explicit AsyncFileOperation(
		Type* type,
		std::shared_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher);

	pub static std::shared_ptr<AsyncFileOperation> newSP_createOrClear(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher);
	pub static std::shared_ptr<AsyncFileOperation> newSP_deleteIfExists(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher);
	pub static std::shared_ptr<AsyncFileOperation> newSP_rename(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, std::string const& newResourceName, bool replaceIfExists);
	pub static std::shared_ptr<AsyncFileOperation> newSP_renameOrMove(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, std::string const& newPath, bool replaceIfExists);
	pub static std::shared_ptr<AsyncFileOperation> newSP_write(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, base::Buffer data);
	pub static std::shared_ptr<AsyncFileOperation> newSP_write(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, std::shared_ptr<std::string> data);
	pub static std::shared_ptr<AsyncFileOperation> newSP_write(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, sp<std::string> data);
	pub static std::shared_ptr<AsyncFileOperation> newSP_write(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher, std::shared_ptr<std::stringstream> data);
	pub static std::shared_ptr<AsyncFileOperation> newSP_createBoundedDirectories(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher);
	pub static std::shared_ptr<AsyncFileOperation> newSP_readBinarySPAsync(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher);
	pub static std::shared_ptr<AsyncFileOperation> newSP_readStringSPAsync(std::weak_ptr<IFileHandlerAsync> fHandler, std::shared_ptr<IFileHandlerBlocking> underlyingBlockingFH, std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> undelyingEventDispatcher);

    pub virtual ~AsyncFileOperation();
};
};
