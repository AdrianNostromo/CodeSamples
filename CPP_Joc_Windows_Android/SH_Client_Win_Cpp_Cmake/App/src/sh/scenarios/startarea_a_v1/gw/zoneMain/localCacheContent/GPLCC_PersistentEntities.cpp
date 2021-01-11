#include "GPLCC_PersistentEntities.h"
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <rpg3D/gw/util/Util_VoxlesGrid3DGeometry.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/app/IAppAssets.h>
#include <base/assets/IAssetsManager.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <base/app/event/AppEvent.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityIds.h>
#include <sh/scenarios/startarea_a_v1/gw/cache/CacheIds.h>
#include <rpg3D/scenariohandler/customisedContentHandler/ICustomisedContentHandler.h>
#include <rpg3D/gw/customisedContent/IGamePartCustomisedContent.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace startarea_a_v1;

GPLCC_PersistentEntities::GPLCC_PersistentEntities(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGamePartMain::GWEvent_onEntityRemoved_Pre->type, base::IGameWorld::GWEvent_saveData->type, base::IGameWorld::GWEvent_disposePre_contentA->type,
		std::bind(&GPLCC_PersistentEntities::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
	gw->addEventListener(
		base::IGameWorld::GWEvent_zoneDeactivate_pre->type,
		std::bind(&GPLCC_PersistentEntities::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_simulationB->type,
		std::bind(&GPLCC_PersistentEntities::onGWEvent_gameLoop_entityInteractions, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPLCC_PersistentEntities::createBMain() {
	super::createBMain();

	gpCustomisedContent = gw->getComponentAs<rpg3D::IGamePartCustomisedContent*>(true/*mustExist*/);
}

int GPLCC_PersistentEntities::getNewPersistentEntityId() {
	persistentEntitiesIdCounter++;

	return persistentEntitiesIdCounter;
}

void GPLCC_PersistentEntities::pushPersistentEntity(IWorldEntity* entity, bool isEntityAlreadyCached) {
	if (persistentEntities.contains(entity)) {
		throw LogicException(LOC);
	}

	persistentEntities.appendDirect(entity);

	// Hook entities pickedUp and thrown events
	entity->addEventListener(
		rpg3D::IStackableModule::EVENT_haulerChanged->type,
		std::bind(&GPLCC_PersistentEntities::onEntityEvent, this, std::placeholders::_1, std::placeholders::_2),
		persistentEntitiesListenersList
	);

	if(!isEntityAlreadyCached) {
		// Do a entity save.
		savePersistentEntity(entity);
		savePersistentEntitiesIndex();
	}
}

void GPLCC_PersistentEntities::persistentEntityTemplateChanged(IWorldEntity* entity) {
	if (!persistentEntities.containsDirect(entity)) {
		// It is not a registenred entity.
		throw LogicException(LOC);
	}

	if (!isDirty_entityTemplatesList.containsDirect(entity)) {
		isDirty_entityTemplatesList.appendDirect(entity);
	}
}

void GPLCC_PersistentEntities::savePendingPersistentEntitiesChanges() {
	managedSaveData(true, 0);
}

void GPLCC_PersistentEntities::savePersistentEntity(IWorldEntity* entity) {
	// Get the visual3D module.
	worldGame3D::IVisual3DModule* entity_visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
	
	// Get the voxels grid drawable or handler.
	VoxelGridDrawable3D* entity_visual3D_editingVoxelsVisual = entity_visual3D->getDrawableMustExistAs< VoxelGridDrawable3D*>("_VoxelGridGroup_");

	// Serialise the materials list and voxels grid.
	bool saveMaterials = true;
	bool saveVoxelsGrid = true;
	if (entity->getInstanceId() == EntityIds::player_hero_dummy) {
		saveMaterials = false;
		saveVoxelsGrid = false;

		gpCustomisedContent->getCustomisedContentHandler()->updatePlayerUnitMaterialsAndVoxelsGrid(
			entity_visual3D_editingVoxelsVisual->peekVoxelsGrid(),
			false/*dispatchGWUpdateEvent*/
		);
	}

	std::shared_ptr<std::stringstream> eTemplateDataSS = rpg3D::Util_VoxlesGrid3DGeometry::serialize(
		entity, entity_visual3D_editingVoxelsVisual->peekVoxelsGrid(),
		true/*saveEntityGenerals*/, saveMaterials, saveVoxelsGrid
	);
	
	// Write the entity template data to a a cache file.
	std::string cachePathId = CacheIds::persistentEntity_prefix + *entity->getInstanceId() + CacheIds::persistentEntity_sufix;
	gw->getAppAssets()->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(gw->getScenarioTemplate()->id)
		->setCache(cachePathId, eTemplateDataSS);
}

void GPLCC_PersistentEntities::overridePersistentEntitiesIdCounter(int persistentEntitiesIdCounter) {
	this->persistentEntitiesIdCounter = persistentEntitiesIdCounter;
}

void GPLCC_PersistentEntities::savePersistentEntitiesIndex() {
	std::shared_ptr<std::stringstream> dataSS = std::make_shared<std::stringstream>();

	// persistentEntitiesIdCounter;
	*dataSS << "persistentEntitiesIdCounter " << persistentEntitiesIdCounter << "\r\n";

	// Entries count.
	*dataSS << "entitiesCount " << persistentEntities.count() << "\r\n";

	// Entries list (1 entry per line).
	for (int i = 0; i < persistentEntities.count(); i++) {
		IWorldEntity* entity = persistentEntities.getDirect(i);

		rpg3D::IStackableModule* entity_stackable = entity->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
		rpg3D::IStackableModule* haulerEntity_stackable = entity_stackable->getHauler();

		Vector3* entityPos;
		float entityRotZDeg;
		std::shared_ptr<base::IWrappedValue> haulerEntity_persistentEntityId;
		if (haulerEntity_stackable != nullptr && haulerEntity_stackable->getEntity()->getInstanceId() != nullptr) {
			// The entity is hauled but maybe not by a persistenEntity (eg. the player unit). Save the stack base pos.
			// Set the pos and rot of the stack_base. This is used as a fallback in case the hauler dissapears from the config file.
			rpg3D::IStackableModule* stackBaseEntity_stackable = entity_stackable->getBottomStackable();
			IWorldEntity* stackBaseEntity = stackBaseEntity_stackable->getEntity();

			entityPos = stackBaseEntity->getPos();
			entityRotZDeg = stackBaseEntity->getRot()->getAngleAround(Vector3::Z);

			std::string haulerPersistentEntityId = *haulerEntity_stackable->getEntity()->getInstanceId();

			*dataSS << "entity instanceId " << *entity->getInstanceId() << " haulerPersistentEntityId " << haulerPersistentEntityId << " x " << entityPos->x << " y " << entityPos->y << " z " << entityPos->z << " angZDeg " << entityRotZDeg << "\r\n";
		} else {
			entityPos = entity->getPos();
			entityRotZDeg = entity->getRot()->getAngleAround(Vector3::Z);

			*dataSS << "entity instanceId " << *entity->getInstanceId() << " x " << entityPos->x << " y " << entityPos->y << " z " << entityPos->z << " angZDeg " << entityRotZDeg << "\r\n";
		}
	}

	// Save the cache data.
	gw->getAppAssets()->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(gw->getScenarioTemplate()->id)
		->setCache(CacheIds::persistentEntititesList, dataSS);
}

void GPLCC_PersistentEntities::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == base::IGamePartMain::GWEvent_onEntityRemoved_Pre->type) {
		if (event.entityData == nullptr) {
			throw LogicException(LOC);
		}

		if (persistentEntities.containsDirect(event.entityData)) {
			// This logic may cause errors. The list needs to be pre-cleared when removing these entities.
			throw LogicException(LOC);
		}
	} else if (event.type == base::IGameWorld::GWEvent_saveData->type) {
		managedSaveData(true, 0);
	} else if (event.type == base::IGameWorld::GWEvent_disposePre_contentA->type || event.type == base::IGameWorld::GWEvent_zoneDeactivate_pre->type) {
		// There should be no dirty persistent entities data here.
		if (isDirty_cachedEntitiesPositionals || isDirty_entityTemplatesList.count() > 0) {
			throw LogicException(LOC);
		}

		// Unload the persistent entities.
		persistentEntitiesListenersList.clear();
		persistentEntities.clear();
	} else {
		throw LogicException(LOC);
	}
}

void GPLCC_PersistentEntities::onEntityEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == rpg3D::IStackableModule::EVENT_haulerChanged->type) {
		//IWorldEntity* entity = event.getEntityData();

		if (!isDirty_cachedEntitiesPositionals) {
			isDirty_cachedEntitiesPositionals = true;
		}
	} else {
		throw LogicException(LOC);
	}
}

void GPLCC_PersistentEntities::onGWEvent_gameLoop_entityInteractions(IEventListener& eventListener, base::GWEvent& event) {
	managedSaveData(false, event.deltaS);
}

void GPLCC_PersistentEntities::managedSaveData(bool doSkipCooldown, float deltaS) {
	if (cacheSaveCooldownS > 0) {
		if (doSkipCooldown) {
			cacheSaveCooldownS = 0.0f;
		} else {
			cacheSaveCooldownS = Math::max(0.0f, cacheSaveCooldownS - deltaS);
		}
	}

	if (cacheSaveCooldownS <= 0) {
		bool isValidation = false;

		if (isDirty_entityTemplatesList.count() > 0) {
			isValidation = true;

			for (int i = 0; i < isDirty_entityTemplatesList.count(); i++) {
				IWorldEntity* entity = isDirty_entityTemplatesList.getDirect(i);

				savePersistentEntity(entity);
			}

			isDirty_entityTemplatesList.clear();
		}

		if (isDirty_cachedEntitiesPositionals) {
			isValidation = true;
			isDirty_cachedEntitiesPositionals = false;

			savePersistentEntitiesIndex();
		}

		if (isValidation) {
			cacheSaveCooldownS = 1.0f;
		}
	}
}

GPLCC_PersistentEntities::~GPLCC_PersistentEntities() {
	//void
}
