#include <worldGame3D/gw/main/GPMEntities.h>
#include "GPManAZConditionalEntities.h"
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>

using namespace battlezones_a_v1;

EntityRemoveReason *const GPManAZConditionalEntities::ENT_REM_REASON_PURPOSE_FINISHED = new EntityRemoveReason("ENT_REM_REASON_PURPOSE_FINISHED", true, true);

GPManAZConditionalEntities::GPManAZConditionalEntities(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPManAZConditionalEntities::addConditionalEntitySpawnInfo(ZoneTemplate::EntitySpawnInfo* spawnInfo) {
	conditionalEntitiesSpawnInfosList.append_emplace((spawnInfo));
}

void GPManAZConditionalEntities::onActiveZoneDeactivate_pre() {
	super::onActiveZoneDeactivate_pre();

	conditionalEntitiesSpawnInfosList.clear();
}

void GPManAZConditionalEntities::onZoneFlagsChange() {
	super::onZoneFlagsChange();
	
	for(int i=0;i<conditionalEntitiesSpawnInfosList.count();i++) {
		ConditionalEntitySpawnInfo* conditionalEntitySpawnInfo = conditionalEntitiesSpawnInfosList.getPointer(i);

		if(conditionalEntitySpawnInfo->activeEntity != nullptr && !activeZoneFlags.test(conditionalEntitySpawnInfo->spawnInfo->spawnTestFlags)) {
			// Remove the entity.
			gw->getMainAs<base::GPMEntities*>()->markEntityForRemoval(conditionalEntitySpawnInfo->activeEntity, ENT_REM_REASON_PURPOSE_FINISHED, false);
			conditionalEntitySpawnInfo->activeEntity = nullptr;
		}
	}

	for(int i=0;i<conditionalEntitiesSpawnInfosList.count();i++) {
		ConditionalEntitySpawnInfo* conditionalEntitySpawnInfo = conditionalEntitiesSpawnInfosList.getPointer(i);

		if(conditionalEntitySpawnInfo->activeEntity == nullptr && activeZoneFlags.test(conditionalEntitySpawnInfo->spawnInfo->spawnTestFlags)) {
			// Create the entity.
			EntityTemplate* entityTemplate = EntityTemplate::fromId(
				conditionalEntitySpawnInfo->spawnInfo->scenarioNamespace,
				conditionalEntitySpawnInfo->spawnInfo->id
			);

			IWorldEntity *entity = gw->getGPMain()->createEntity(
				entityTemplate,
				conditionalEntitySpawnInfo->spawnInfo->pos,
				&conditionalEntitySpawnInfo->spawnInfo->rot/*rot*/, nullptr/*cardinalRotation*/,
				conditionalEntitySpawnInfo->spawnInfo->instanceId,
				0,
				nullptr,
				true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
				nullptr
			);
			conditionalEntitySpawnInfo->activeEntity = entity;
		}
	}
}

GPManAZConditionalEntities::~GPManAZConditionalEntities() {
	//void
}
