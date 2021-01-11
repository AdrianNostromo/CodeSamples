#include "AMProcessorsList.h"

using namespace worldGame3D;

AMProcessorsList::AMProcessorsList(
	IWorldEntity* entity,
	ActionsTemplate* t)
	: super(entity, t)
{
	//void
}

base::TypedEventDispatcher1D<worldGame3D::IAction>* AMProcessorsList::getActionsDispatcher() {
	return &actionsDispatcher;
}

std::shared_ptr<IEventListener> AMProcessorsList::setActionProcessor(
	int type,
	std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb)
{
	return actionsDispatcher.getDispatcher(type, true)->addListener(cb);
}

void AMProcessorsList::setActionProcessor(
	int type,
	std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	actionsDispatcher.getDispatcher(type, true)->addListener(cb, &autoListenersList);
}

void AMProcessorsList::setActionProcessor(
	int type1, int type2,
	std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	setActionProcessor(type1, cb, autoListenersList);
	setActionProcessor(type2, cb, autoListenersList);
}

void AMProcessorsList::setActionProcessor(
	int type1, int type2, int type3,
	std::function<void(IEventListener& eventListener, worldGame3D::IAction& action)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	setActionProcessor(type1, cb, autoListenersList);
	setActionProcessor(type2, cb, autoListenersList);
	setActionProcessor(type3, cb, autoListenersList);
}

int AMProcessorsList::dispatchAction(worldGame3D::IAction& action) {
	base::EventDispatcher<worldGame3D::IAction>* d = actionsDispatcher.getDispatcher(action.getType(), false);
    if(d == nullptr || d->getListenersCount() == 0) {
        if(action.getCanHaveNoListeners()) {
            // This is used by tools update,deactivate if they are removed while active.
            return 0;
        }else {
            // The action must have exactly 1 processor or be optional and have 0 listeners.
            throw LogicException(LOC);
        }
    }else if(d->getListenersCount() == 1) {
	    //void
	}else {
		// The action must have exactly 1 processor or be optional and have 0 listeners.
		throw LogicException(LOC);
	}

	int ct = d->dispatch(action);
	if(ct != 1) {
		// The action must have exactly 1 processor.
		// The optional check was made above.
		throw LogicException(LOC);
	}

	return ct;
}

AMProcessorsList::~AMProcessorsList() {
	//void
}
