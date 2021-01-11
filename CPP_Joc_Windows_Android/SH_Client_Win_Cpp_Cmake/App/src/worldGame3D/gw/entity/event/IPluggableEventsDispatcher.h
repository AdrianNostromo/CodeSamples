#pragma once

#include <base/gh.h>
#include <memory>
#include <functional>

class IEventListener;
namespace base {
	class GWEvent;
};

namespace worldGame3D {
class IPluggableEventsDispatcher {
	pub virtual std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr) = 0;
	pub virtual void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr) = 0;
	pub virtual void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr) = 0;
	pub virtual void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr) = 0;

	pub virtual ~IPluggableEventsDispatcher() = default;
};
};
