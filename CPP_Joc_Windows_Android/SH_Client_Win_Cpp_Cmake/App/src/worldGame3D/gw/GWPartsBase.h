#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/content/GamePartContent.h>
#include <worldGame3D/gw/interaction/GPInteraction.h>
#include <worldGame3D/gw/main/GamePartMain.h>
#include "GWBase.h"
#include <base/list/ArrayList.h>
#include <base/event/dispatcher/EventDispatcher.h>

class GWPartsBase : public base::GWBase {priv typedef base::GWBase super;pub dCtor(GWPartsBase);
	// Use shared_ptr to avoid large map block memory usage on change.
	priv base::TypedEventDispatcher1D<base::EntityEvent> entityBubbledEventDispatcher{};

	priv ComponentsHandler* viewComponentsHandler = nullptr;

	priv bool isInitialGamePartsPostCreated = false;
    
	pub explicit GWPartsBase(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);
	prot void createFramework() override;
	prot void createGPartsPost() override;

	pub IComponentsHandler* getViewComponentsHandler() final;

	pub void addGamePartUninitialised(IGamePart* gamePart) final;
	pub void disposeGamePart(IGamePart* gamePart) final;

	pub void addComponent(IComponent* component, bool doCreate, int disposeOrderDefault5 = 5) override;
	pub void removeComponent(IComponent* component, bool doDispose, bool useDelayedDispose = false) override;

	pub base::TypedEventDispatcher1D<base::EntityEvent>* getEntityBubbledEventDispatcher() override;
	pub std::shared_ptr<IEventListener> addEntityBubbledEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb) override;
	pub void addEntityBubbledEventListener(
		int type,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	pub void addEntityBubbledEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	pub void addEntityBubbledEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList) override;
	pub int dispatchEntityBubbledEvent(base::EntityEvent& event) override;

	prot void disposeMainPre() override;
	prot void dispose() override;
	pub ~GWPartsBase() override;
};
