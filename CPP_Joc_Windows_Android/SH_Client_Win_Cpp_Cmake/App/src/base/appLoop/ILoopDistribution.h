#pragma once

#include <base/gh.h>
#include <memory>
#include <functional>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/list/ArrayList.h>

class IEventListener;
namespace base {
	class LoopEvent;
};

namespace base {
class ILoopDistribution {
	pub static base::LoopEvent* AppLoopEvent_input;
	pub static base::LoopEvent* AppLoopEvent_inputDispatch;
	pub static base::LoopEvent* AppLoopEvent_general;
	pub static base::LoopEvent* AppLoopEvent_eco;
	pub static base::LoopEvent* AppLoopEvent_gameWorld_pre;
	pub static base::LoopEvent* AppLoopEvent_gameWorld;
	pub static base::LoopEvent* AppLoopEvent_ui;
	pub static base::LoopEvent* AppLoopEvent_delayedComponentsDispose;

	pub virtual base::TypedEventDispatcher1D<base::LoopEvent>* getEventDispatcher() = 0;
	pub virtual std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb) = 0;
	pub virtual void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEventListener(
		ArrayList<int>& eventTypesList,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;

    pub virtual ~ILoopDistribution() = default;
};
};
