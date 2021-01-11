#pragma once

#include <base/gh.h>
#include "GPC_PlayerEntities.h"
#include <base/list/ArrayList.h>
#include <memory>
#include <base/event/dispatcher/EventDispatcher.h>

class IEventListener;
class IWorldEntity;
namespace base {
    class GWEvent;
    class IGPContent;
    class WrappedWorldEntity;
};
namespace rpg3D {
    class WorldOrEntityInteractionEvent;
    class IGPUserEntitiesController;
    class WorldOrEntityInteractionFilter;
};

namespace base {
class GPC_PlayerEntitySelector : public GPC_PlayerEntities {priv typedef GPC_PlayerEntities super;pub dCtor(GPC_PlayerEntitySelector);
    priv static const bool IS_DEBUG_LOCAL;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};
    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList_filtersDispatcher{};

    // The entity may be removed from the gw or may be removed from the players list.
    priv std::shared_ptr<base::WrappedWorldEntity> selectedPlayerWEntitySwitchRequest = nullptr;

    priv bool isSelectorEnabled = false;

    priv base::IGPContent* gpContent = nullptr;
    priv rpg3D::IGPUserEntitiesController* gpUserEntitiesController = nullptr;
    priv base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcherGlobal = nullptr;

    priv rpg3D::WorldOrEntityInteractionFilter* entityFilter = nullptr;

    pub explicit GPC_PlayerEntitySelector(IGameWorld* gw);
    prot void createBMain() override;

    priv bool filterTest_entityCustom(IWorldEntity* entity);

    priv void onGWEvent_gameLoop_localInputPost(IEventListener& eventListener, base::GWEvent& event);

    priv void onUserWorldInteraction(IEventListener& eventListener, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction);
	
    pub bool getIsSelectorEnabled() final;
    pub void setIsSelectorEnabled(bool isSelectorEnabled) final;

    prot void checkSelectorActivationChange();

    prot void onPlayerEntityAdded() override;
    prot void onPlayerEntityRemoved() override;

    prot void disposePre() override;
    pub ~GPC_PlayerEntitySelector() override;
};
};
