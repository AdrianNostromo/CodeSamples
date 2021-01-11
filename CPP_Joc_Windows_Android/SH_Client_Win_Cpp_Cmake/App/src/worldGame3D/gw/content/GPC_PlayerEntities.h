#pragma once

#include <base/gh.h>
#include "GPC_Base.h"
#include <base/object/WrappedValue.h>

namespace base {
	class IDebugEntryUser;
	class EntityEvent;
};
namespace worldGame3D {
	class IGCEntityGroups;
	class IEntsGroup;
	class EntGroupsEvent;
};

namespace base {
class GPC_PlayerEntities : public GPC_Base {priv typedef GPC_Base super;pub dCtor(GPC_PlayerEntities);
	priv static bool IS_DEBUG_VIEW_LOG;
	
	priv IWorldEntity* selectedPlayerEntity = nullptr;

	pub ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList_selectedPlayerEntity{};
	pub ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList_entsGroup{};

	// These are for the selected entity only.
	priv std::shared_ptr<base::IDebugEntryUser> dynamicDebugEntry_pos = nullptr;
	priv std::shared_ptr<base::WrappedValue<float>> dynamicDebugEntry_pos_playerPosX = nullptr;
	priv std::shared_ptr<base::WrappedValue<float>> dynamicDebugEntry_pos_playerPosY = nullptr;
	priv std::shared_ptr<base::WrappedValue<float>> dynamicDebugEntry_pos_playerPosZ = nullptr;

	priv std::shared_ptr<base::IDebugEntryUser> dynamicDebugEntry_gPos = nullptr;
	priv std::shared_ptr<base::WrappedValue<float>> dynamicDebugEntry_g1Pos_playerPosX = nullptr;
	priv std::shared_ptr<base::WrappedValue<float>> dynamicDebugEntry_g1Pos_playerPosY = nullptr;
	priv std::shared_ptr<base::WrappedValue<float>> dynamicDebugEntry_g06Pos_playerPosX = nullptr;
	priv std::shared_ptr<base::WrappedValue<float>> dynamicDebugEntry_g06Pos_playerPosY = nullptr;

	priv worldGame3D::IGCEntityGroups* gpEntsGroups = nullptr;
	prot worldGame3D::IEntsGroup* playerEntsGroup = nullptr;

    pub explicit GPC_PlayerEntities(IGameWorld* gw);
	prot void createBMain() override;

	pub IWorldEntity* getSelectedPlayerEntity(bool mustExist) final;
	pub void setSelectManagedPlayerEntity(IWorldEntity* entity) final;
	pub void setSelectManagedPlayerEntity(std::shared_ptr<base::WrappedWorldEntity> wEntity, bool handleBadEntity) final;

	pub worldGame3D::IEntsGroup* getPlayerEntitiesGroup() final;

	priv void entsGroup_OnEntitiesCountChanged(IEventListener& eventListener, worldGame3D::EntGroupsEvent& event);
	priv void selectedPlayerEntity_onSyncPos(IEventListener& eventListener, base::EntityEvent& event);

	prot virtual void onPlayerEntityAdded();
	prot virtual void onPlayerEntityRemovePre();
	prot virtual void onPlayerEntityRemoved();

    prot void disposePre() override;
    pub ~GPC_PlayerEntities() override;

};
};
