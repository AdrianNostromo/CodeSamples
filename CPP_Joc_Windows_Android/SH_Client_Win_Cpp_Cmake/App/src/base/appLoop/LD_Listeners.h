#pragma once

#include <base/gh.h>
#include "LD_Base.h"
#include <base/event/dispatcher/TypedEventDispatcher1D.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/appLoop/event/LoopEvent.h>

namespace base {
class LD_Listeners : public LD_Base {priv typedef LD_Base super;pub dCtor(LD_Listeners);
	priv base::TypedEventDispatcher1D<base::LoopEvent> eventDispatcher{};

    pub explicit LD_Listeners();

	pub base::TypedEventDispatcher1D<base::LoopEvent>* getEventDispatcher() final;
	pub std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb) final;
	pub void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) final;
	pub void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) final;
	pub void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) final;
	pub void addEventListener(
		ArrayList<int>& eventTypesList,
		std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) final;

	pub int dispatchEvent(base::LoopEvent& event);

    pub ~LD_Listeners() override;
};
};
