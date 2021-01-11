#pragma once

#include <base/gh.h>
#include "GPMLayers2D.h"
#include <base/list/ListDL.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <base/MM.h>

namespace base {
	class WrappedWorldEntity;
};

namespace base {
class GPMEntities : public GPMLayers2D, virtual public IEntityHandlerBase {priv typedef GPMLayers2D super;pub dCtor(GPMEntities);
	priv class MarkedForRemovalEntity {pub dCtor(MarkedForRemovalEntity);
		// Use the actual entity because the wrapper is cleared when a entity is markedForRemoval.
		pub IWorldEntity* entity;
		pub bool disposeAfterRemoval;

		pub explicit MarkedForRemovalEntity(IWorldEntity* entity, bool disposeAfterRemoval)
			: entity(entity), disposeAfterRemoval(disposeAfterRemoval)
		{}
	};
	priv class DrawableToUpdateEntry {pub dCtor(DrawableToUpdateEntry);
		// Use the actual entity because the wrapper is cleared when a entity is markedForRemoval.
		pub std::shared_ptr<base::WrappedWorldEntity> wEntity;
		pub IUpdatableEntityVisual* drawableModule;

		pub explicit DrawableToUpdateEntry(std::shared_ptr<base::WrappedWorldEntity> wEntity, IUpdatableEntityVisual* drawableModule)
			: wEntity(wEntity), drawableModule(drawableModule)
		{}
	};

	priv ArrayList<IWorldEntity*> entitiesList{};

    // Multiple entities may have the same instance id.
	priv Map1D<std::string, std::shared_ptr<ArrayList<IWorldEntity*>>> instancedEntitiesMap{};

	priv ListDL<DrawableToUpdateEntry> drawableEntityModulesToUpdateList{};

	// Save the wEntity in case it get's removed with different logic (eg. zone change).
	priv ListDL<MarkedForRemovalEntity> markedForRemovalEntities{LOC};

	priv bool isGamePartsEntityUsable = true;

	pub explicit GPMEntities(base::IGameWorld* gw);

	pub virtual void addDrawableModuleToVisualUpdate(IUpdatableEntityVisual* drawableModule) final;
	pub IWorldEntity* getEntityWithLocalEntityId(int localEntityId) override;
	pub void markEntityForRemoval(IWorldEntity* entity, EntityRemoveReason* removeReason, bool isRequestFromServer, bool disposeAfterRemoval=true)override;

	pub IWorldEntity* getEntityWithInstanceId(std::string& instanceId, bool mustExist = false) override;
	pub bool isAnyEntityWithInstanceId(std::string& instanceId) override;
	pub std::shared_ptr<ArrayList<IWorldEntity*>> getEntitiesListWithInstanceId(std::string& instanceId) override;

	pub int countChopAxesHeadingTowardsTreeWithLocalEntityId(int treeLocalEntityId) final;

	// This is used to disconnect entities from game parts when the gParts may change (on zone changes).
	pub bool getIsGamePartsEntityUsable() final;

	pub ArrayList<IWorldEntity*>* peekEntitiesList() final;

	pub IWorldEntity* getEntityWithSyncedEntityId(int syncedEntityId);
	pub void addWorldEntity(IWorldEntity* entity, std::shared_ptr<std::string> instanceId);

	priv void onGWEvent_gameLoop_visualsUpdate(IEventListener& eventListener, base::GWEvent& event);
	priv void onGWEvent_gameLoop_entitiesRemoval(IEventListener& eventListener, base::GWEvent& event);

	priv bool removeEntity(IWorldEntity* entity, EntityRemoveReason* removeReason, bool disposeAfterRemoval=true);

	priv void addEntityToInstanceIdTracking(IWorldEntity* entity);
	priv void removeEntityFromInstanceIdTracking(IWorldEntity* entity);

	pub void removeAllZoneOwnedEntities() final;

	pub void connectAllEntitiesToGameParts() final;
	pub void disconnectAllEntitiesFromGameParts() final;

	prot void disposePre() override;
	pub ~GPMEntities() override;

};
};
