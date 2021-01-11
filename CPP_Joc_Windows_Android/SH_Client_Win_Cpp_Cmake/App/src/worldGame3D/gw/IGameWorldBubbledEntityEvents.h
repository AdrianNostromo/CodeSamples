#pragma once

#include <base/gh.h>
#include "IGameWorldEvents.h"
#include <base/event/dispatcher/EventDispatcher.h>

namespace worldGame3D {
class IGameWorldBubbledEntityEvents : public IGameWorldEvents {
	pub virtual base::TypedEventDispatcher1D<base::EntityEvent>* getEntityBubbledEventDispatcher() = 0;
	pub virtual std::shared_ptr<IEventListener> addEntityBubbledEventListener(
	    int type,
	    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb) = 0;
	pub virtual void addEntityBubbledEventListener(
	    int type,
	    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
	    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEntityBubbledEventListener(
	    int type1, int type2,
	    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
	    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;
	pub virtual void addEntityBubbledEventListener(
	    int type1, int type2, int type3,
	    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
	    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) = 0;

    pub ~IGameWorldBubbledEntityEvents() override = default;
};
};
