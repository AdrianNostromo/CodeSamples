#pragma once

#include <base/gh.h>
#include "GPLCC_Base.h"
#include <memory>
#include <string>

class IWorldEntity;
namespace base {
	class GWEvent;
	class EntityEvent;
};
namespace rpg3D {
	class IGamePartCustomisedContent;
}

namespace startarea_a_v1 {
class GPLCC_PersistentEntities : public GPLCC_Base {priv typedef GPLCC_Base super; pub dCtor(GPLCC_PersistentEntities);
	priv class WrappedEntity {pub dCtor(WrappedEntity);
		pub IWorldEntity* entity;
		pub int localDataIndex;
		
		pub explicit WrappedEntity(IWorldEntity* entity, int localDataIndex)
			: entity(entity), localDataIndex(localDataIndex)
			{};
		pub virtual ~WrappedEntity() {};
	};

	priv ArrayList< IWorldEntity*> persistentEntities{};
	priv ArrayList<std::shared_ptr<IEventListener>> persistentEntitiesListenersList{};
	// This is initialised on entities loading from the save data.
	priv int persistentEntitiesIdCounter = 0;

	priv float cacheSaveCooldownS = 0.0f;
	priv bool isDirty_cachedEntitiesPositionals = false;
	priv ArrayList< IWorldEntity*> isDirty_entityTemplatesList{};

	priv rpg3D::IGamePartCustomisedContent* gpCustomisedContent = nullptr;

    pub explicit GPLCC_PersistentEntities(base::IGameWorld* gw);
	prot void createBMain() override;

	pub int getNewPersistentEntityId() override;
	pub void pushPersistentEntity(IWorldEntity* entity, bool isEntityAlreadyCached) override;
	pub void persistentEntityTemplateChanged(IWorldEntity* entity) override;
	pub void savePendingPersistentEntitiesChanges() override;
	priv void savePersistentEntity(IWorldEntity* entity);

	prot void overridePersistentEntitiesIdCounter(int persistentEntitiesIdCounter);
	priv void savePersistentEntitiesIndex();

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	priv void onEntityEvent(IEventListener& eventListener, base::EntityEvent& event);

	priv void onGWEvent_gameLoop_entityInteractions(IEventListener& eventListener, base::GWEvent& event);
	priv void managedSaveData(bool doSkipCooldown, float deltaS);

    pub ~GPLCC_PersistentEntities() override;
};
};
