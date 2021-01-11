#include "GWPartsBase.h"
#include <worldGame3D/gw/entity/event/EntityEvent.h>
#include <base/component/ComponentsHandler.h>
#include <base/component/IComponent_UIView.h>

GWPartsBase::GWPartsBase(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
    : super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

IComponentsHandler* GWPartsBase::getViewComponentsHandler() {
    return viewComponentsHandler;
}

void GWPartsBase::addGamePartUninitialised(IGamePart* gamePart) {
	if (gamePart == nullptr) {
		throw LogicException(LOC);
	}

	addComponent(gamePart, true/*doCreate*/);
}

void GWPartsBase::disposeGamePart(IGamePart* gamePart) {
	if (gamePart == nullptr) {
		throw LogicException(LOC);
	}

    removeComponent(gamePart, true/*doDispose*/);
}

void GWPartsBase::addComponent(IComponent* component, bool doCreate, int disposeOrderDefault5) {
	GamePart* gamePart = static_cast<GamePart*>(component->componentClassVoidP);

    super::addComponent(component, doCreate, disposeOrderDefault5);
	
	if (doCreate && isInitialGamePartsPostCreated) {
		gamePart->reservedCreateB();
	}
}

void GWPartsBase::removeComponent(IComponent* component, bool doDispose, bool useDelayedDispose) {
    GamePart* gamePart = static_cast<GamePart*>(component->componentClassVoidP);

    // If useDelayedDispose is used, this function will be called again later with useDelayedDispose:false and the below reserved dispose is guaranteed to be executed.
    if (doDispose && !useDelayedDispose) {
        // On gw dispose, these are alreeady called in the correct order.
        gamePart->reservedDisposePrePreIfNeeded();
        gamePart->reservedDisposePreIfNeeded();
    }

    super::removeComponent(component, doDispose, useDelayedDispose);
}

base::TypedEventDispatcher1D<base::EntityEvent>* GWPartsBase::getEntityBubbledEventDispatcher() {
    return &entityBubbledEventDispatcher;
}

std::shared_ptr<IEventListener> GWPartsBase::addEntityBubbledEventListener(
    int type,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb)
{
    return entityBubbledEventDispatcher.getDispatcher(type, true)->addListener(cb);
}

void GWPartsBase::addEntityBubbledEventListener(
    int type,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    entityBubbledEventDispatcher.getDispatcher(type, true)->addListener(cb, &autoListenersList);
}

void GWPartsBase::addEntityBubbledEventListener(
	int type1, int type2,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    addEntityBubbledEventListener(type1, cb, autoListenersList);
    addEntityBubbledEventListener(type2, cb, autoListenersList);
}

void GWPartsBase::addEntityBubbledEventListener(
	int type1, int type2, int type3,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    addEntityBubbledEventListener(type1, cb, autoListenersList);
    addEntityBubbledEventListener(type2, cb, autoListenersList);
    addEntityBubbledEventListener(type3, cb, autoListenersList);
}

int GWPartsBase::dispatchEntityBubbledEvent(base::EntityEvent& event) {
	int ct = 0;

	base::EventDispatcher<base::EntityEvent>* dis = entityBubbledEventDispatcher.getDispatcher(event.type, false);
    if(dis != nullptr) {
        ct += dis->dispatch(event);
    }

    // This is called from the bubbling origin.
    //event.clearData();

    return ct;
}

void GWPartsBase::createFramework() {
    super::createFramework();

    viewComponentsHandler = new ComponentsHandler(IComponent_UIView::COMPONENT_CATEGORY, app/*delayedDisposeSupportApp*/);
    viewComponentsHandler->reservedCreate();
}

void GWPartsBase::createGPartsPost() {
    super::createGPartsPost();

    for (ListDL<IComponent*>::Entry* listEntry = getComponentsList().getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
        GamePart* gPart = static_cast<GamePart*>(listEntry->data->componentClassVoidP);

        gPart->reservedCreateB();
    }
	isInitialGamePartsPostCreated = true;
}

void GWPartsBase::disposeMainPre() {
	dispatchEvent(*GWEvent_dispose_views);

    for (ListDL<IComponent*>::Entry* listEntry = getComponentsList().getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
        GamePart* gPart = static_cast<GamePart*>(listEntry->data->componentClassVoidP);

        // While on this dispose game_parts can still access other game_parts.
        gPart->reservedDisposePrePre();
    }
    for (ListDL<IComponent*>::Entry* listEntry = getComponentsList().getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
        GamePart* gPart = static_cast<GamePart*>(listEntry->data->componentClassVoidP);

        // While on this dispose game_parts can still access other game_parts.
        gPart->reservedDisposePre();
    }

    super::disposeMainPre();
}

void GWPartsBase::dispose() {
    if (viewComponentsHandler != nullptr) {
        viewComponentsHandler->reservedDispose();
        delete viewComponentsHandler;
        viewComponentsHandler = nullptr;
    }

    super::dispose();
}

GWPartsBase::~GWPartsBase() {
    //void
}
