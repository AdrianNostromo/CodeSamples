#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <functional>
#include <sstream>
#include <base/buffer/Buffer.h>
#include <base/string/String.h>
#include <base/fileSystem/fileHandler/IFileHandlerBase.h>
#include <base/event/listener/IEventListener.h>
#include <base/list/ArrayList.h>
#include <base/fileSystem/fileHandler/async/util/AsyncFHEvent.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/event/dispatcher/EventDispatcher.h>

namespace base {
class IFileHandlerAsync : virtual public IFileHandlerBase {
	pub static base::AsyncFHEvent* EVENT_IO_failed;

	pub static base::AsyncFHEvent* EVENT_read_string_success;
	pub static base::AsyncFHEvent* EVENT_read_binary_success;

	pub virtual std::shared_ptr<base::TypedEventDispatcher1D<base::AsyncFHEvent>> getEventDispatcher() = 0;

	pub virtual std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb) = 0;
	pub virtual void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		ArrayList<int>& eventTypesList,
		std::function<void(IEventListener& eventListener, base::AsyncFHEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual int dispatchEvent(base::AsyncFHEvent& event) = 0;

	pub virtual void readBinarySPAsync() = 0;
	pub virtual void readStringSPAsync() = 0;

	pub virtual void cacheGroupWriteLockingOperationsCountChange(bool isLockOperationAdded) = 0;

    virtual ~IFileHandlerAsync() = default;
};
};
