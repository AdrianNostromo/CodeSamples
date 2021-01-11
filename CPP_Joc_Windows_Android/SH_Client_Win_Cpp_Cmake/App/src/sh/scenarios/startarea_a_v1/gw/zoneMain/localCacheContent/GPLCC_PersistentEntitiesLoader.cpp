#include "GPLCC_PersistentEntitiesLoader.h"
#include <base/fileSystem/fileHandler/async/IFileHandlerAsync.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/app/IAppAssets.h>
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <base/assets/IAssetsManager.h>
#include <base/util/StringUtil.h>
#include <base/exceptions/DataException.h>
#include <base/exceptions/DataFormatException.h>
#include <rpg3D/gw/util/Util_VoxlesGrid3DGeometry.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <rpg3D/gw/config/entity/unit/T_Unit_EditorDefaultCreature_A_V1.h>
#include <rpg3D/gw/config/entity/unit/Templates_Unit_PlayerUnitDummy_A_V1.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityIds.h>
#include <sh/scenarios/startarea_a_v1/gw/cache/CacheIds.h>
#include <rpg3D/scenariohandler/customisedContentHandler/ICustomisedContentHandler.h>
#include <rpg3D/gw/customisedContent/IGamePartCustomisedContent.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace startarea_a_v1;

const int GPLCC_PersistentEntitiesLoader::LoadPhase::Idle = 0;
const int GPLCC_PersistentEntitiesLoader::LoadPhase::StartRequested = 1;
const int GPLCC_PersistentEntitiesLoader::LoadPhase::LoadEntitiesList = 2;
const int GPLCC_PersistentEntitiesLoader::LoadPhase::LoadEntitiesTemplates = 3;
const int GPLCC_PersistentEntitiesLoader::LoadPhase::CreateEntities = 4;
const int GPLCC_PersistentEntitiesLoader::LoadPhase::Finished = 5;

GPLCC_PersistentEntitiesLoader::GPLCC_PersistentEntitiesLoader(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_contentCreation->type,
		std::bind(&GPLCC_PersistentEntitiesLoader::onGWEvent_gameLoop_contentCreation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPLCC_PersistentEntitiesLoader::loadAllPersistentEntities() {
	if (loadPhase != LoadPhase::Idle) {
		throw LogicException(LOC);
	}

	isLoadPhaseInit = false;
	loadPhase = LoadPhase::StartRequested;
}

void GPLCC_PersistentEntitiesLoader::onGWEvent_gameLoop_contentCreation(IEventListener& eventListener, base::GWEvent& event) {
	int newLoadPhase;
	try {
		newLoadPhase = tickLoadPhase();
	}catch (DataException& x) {
		// Cache is invalid.
		gw->getAppAssets()->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(gw->getScenarioTemplate()->id)
			->cacheIntegrityErrorDetected();

		newLoadPhase = LoadPhase::Finished;
	}catch (DataFormatException& x) {
		// Cache is invalid.
		gw->getAppAssets()->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(gw->getScenarioTemplate()->id)
			->cacheIntegrityErrorDetected();

		newLoadPhase = LoadPhase::Finished;
	}catch (OutOfRangeException& x) {
		// Cache is invalid.
		gw->getAppAssets()->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(gw->getScenarioTemplate()->id)
			->cacheIntegrityErrorDetected();

		newLoadPhase = LoadPhase::Finished;
	}
	if (loadPhase != newLoadPhase) {
		isLoadPhaseInit = false;
		loadPhase = newLoadPhase;
	} else if (!isLoadPhaseInit) {
		isLoadPhaseInit = true;
	}
}

int GPLCC_PersistentEntitiesLoader::tickLoadPhase() {
	switch (loadPhase) {
	case LoadPhase::Idle: {
		if (!isLoadPhaseInit) {
			//Init state.
		} else {
			//Wait for state result.
		}

		//Always process.

		break;
	}
	case LoadPhase::StartRequested: {
		if (!isLoadPhaseInit) {
			//Init state.
			return LoadPhase::LoadEntitiesList;
		} else {
			//Wait for state result.
		}

		//Always process.

		break;
	}
	case LoadPhase::LoadEntitiesList: {
		if (!isLoadPhaseInit) {
			//Init state.
			if (!initLoadPhase_LoadEntitiesList()) {
				// Nothing to load.
				return LoadPhase::Finished;
			}
		} else {
			//Wait for state result.
			if (tickLoadPhase_LoadEntitiesList()) {
				return LoadPhase::LoadEntitiesTemplates;
			}
		}

		//Always process.

		break;
	}
	case LoadPhase::LoadEntitiesTemplates: {
		if (!isLoadPhaseInit) {
			//Init state.
			initLoadPhase_LoadEntitiesTemplates();
		} else {
			//Wait for state result.
			if (tickLoadPhase_LoadEntitiesTemplates()) {
				return LoadPhase::CreateEntities;
			}
		}

		//Always process.

		break;
	}
	case LoadPhase::CreateEntities: {
		if (!isLoadPhaseInit) {
			//Init state.
			initLoadPhase_CreateEntities();

			return LoadPhase::Finished;
		} else {
			//Wait for state result.
		}

		//Always process.

		break;
	}
	case LoadPhase::Finished: {
		if (!isLoadPhaseInit) {
			//Init state.
			onPersistentEntitiesLoadFinished();
		} else {
			//Wait for state result.
		}

		//Always process.

		break;
	}
	default: {
		throw LogicException(LOC);
    }}

	return loadPhase;
}

bool GPLCC_PersistentEntitiesLoader::initLoadPhase_LoadEntitiesList() {
	// Read the entities list file data (async mode) from cache.
	// Get the cache item file handler async.
	if (!readCachedStringFileAsync(CacheIds::persistentEntititesList)) {
		return false;
	}

	return true;
}

bool GPLCC_PersistentEntitiesLoader::tickLoadPhase_LoadEntitiesList() {
	if (data_waitingFileRead == nullptr) {
		return false;
	}

	// Make sure there is some data.
	if (data_waitingFileRead->length() <= 0) {
		throw LogicException(LOC);
	}

	// Split input into strings.
	std::shared_ptr<ArrayList<std::string>> linesList = StringUtil::split2(*data_waitingFileRead, "\n", "\r", false);
	
	// Make sure there are enough entries.
	if (linesList->count() < 2) {
		//Cache integrity error occured.
		throw DataException();
	}

	// Parse the data to a structure.
	int lineIndex = 0;
	std::string& line = linesList->getReference(lineIndex++);
	parseData_EntitiesList_persistentEntitiesIdCounter(&parseData_EntitiesList, line);

	line = linesList->getReference(lineIndex++);
	parseData_EntitiesList_entitiesCount(&parseData_EntitiesList, line);

	if (linesList->count() < lineIndex + parseData_EntitiesList.entitiesCount) {
		// Not enought lines to contains the entities list.
		throw DataException();
	}

	for (int i = 0; i < parseData_EntitiesList.entitiesCount; i++) {
		line = linesList->getReference(lineIndex++);
		
		parseData_EntitiesList_entity(&parseData_EntitiesList, line);
	}

	return true;
}

void GPLCC_PersistentEntitiesLoader::parseData_EntitiesList_persistentEntitiesIdCounter(ParseData_EntitiesList* parseData_EntitiesList, std::string& line) {
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
	if (partsList->count() != 2) {
		throw DataException();
	}

	int partIndex = 0;
	std::string& part = partsList->getReference(partIndex++);
	if (part != "persistentEntitiesIdCounter") {
		throw DataException();
	}

	part = partsList->getReference(partIndex++);
	parseData_EntitiesList->persistentEntitiesIdCounter = StringUtil::parseInt(part);
}

void GPLCC_PersistentEntitiesLoader::parseData_EntitiesList_entitiesCount(ParseData_EntitiesList* parseData_EntitiesList, std::string& line) {
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
	if (partsList->count() != 2) {
		throw DataException();
	}

	int partIndex = 0;
	std::string& part = partsList->getReference(partIndex++);
	if (part != "entitiesCount") {
		throw DataException();
	}

	part = partsList->getReference(partIndex++);
	parseData_EntitiesList->entitiesCount = StringUtil::parseInt(part);
}

void GPLCC_PersistentEntitiesLoader::parseData_EntitiesList_entity(ParseData_EntitiesList* parseData_EntitiesList, std::string& line) {
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
	if (partsList->count() < 4) {
		// At least for parts are needed for now.
		throw DataException();
	}
	
	int partIndex = 0;
	std::string& part = partsList->getReference(partIndex++);
	if (part != "entity") {
		throw DataException();
	}
	
	ParseData_EntitiesList::CfgEntity* cfgEntity = parseData_EntitiesList->entitiesList.append_emplace();

	part = partsList->getReference(partIndex++);
	if (part != "instanceId") {
		throw DataException();
	}
	part = partsList->getReference(partIndex++);
	cfgEntity->instanceId = part;
	
	part = partsList->getReference(partIndex++);
	if (part == "haulerPersistentEntityId") {
		if (partsList->count() != 13) {
			throw DataException();
		}

		part = partsList->getReference(partIndex++);
		cfgEntity->haulerEntityInstanceId = part;
		
		part = partsList->getReference(partIndex++);
	} else {
		cfgEntity->haulerEntityInstanceId = StringUtil::empty;

		if (partsList->count() != 11) {
			throw DataException();
		}
	}

	if (part != "x") {
		throw DataException();
	}
	part = partsList->getReference(partIndex++);
	cfgEntity->x = StringUtil::parseFloat(part);

	part = partsList->getReference(partIndex++);
	if (part != "y") {
		throw DataException();
	}
	part = partsList->getReference(partIndex++);
	cfgEntity->y = StringUtil::parseFloat(part);

	part = partsList->getReference(partIndex++);
	if (part != "z") {
		throw DataException();
	}
	part = partsList->getReference(partIndex++);
	cfgEntity->z = StringUtil::parseFloat(part);

	part = partsList->getReference(partIndex++);
	if (part != "angZDeg") {
		throw DataException();
	}
	part = partsList->getReference(partIndex++);
	cfgEntity->angZDeg = StringUtil::parseFloat(part);
}

void GPLCC_PersistentEntitiesLoader::initLoadPhase_LoadEntitiesTemplates() {
	//void
}

bool GPLCC_PersistentEntitiesLoader::tickLoadPhase_LoadEntitiesTemplates() {
	// Load entities in order and parse them.
	bool isGotoNextEntity = false;
	if (parseData_EntitiesList.loadingTemplate_entityIndex < 0) {
		isGotoNextEntity = true;
	} else if (data_waitingFileRead != nullptr) {
		// Parse the template data and go to the next one if any.
		ParseData_EntitiesList::CfgEntity* parseData_cfgEntity = parseData_EntitiesList.entitiesList.getPointer(parseData_EntitiesList.loadingTemplate_entityIndex);
		if (parseData_cfgEntity == nullptr) {
			throw LogicException(LOC);
		}

		bool loadMaterials = true;
		bool loadVoxelsGrid = true;
		if (parseData_cfgEntity->instanceId == *EntityIds::player_hero_dummy) {
			loadMaterials = false;
			loadVoxelsGrid = false;
		}
		parseData_cfgEntity->entityTemplate = rpg3D::Util_VoxlesGrid3DGeometry::parse(
			*data_waitingFileRead,
			true/*loadEntityGenerals*/, loadMaterials, loadVoxelsGrid
		);

		isGotoNextEntity = true;
	}

	if (isGotoNextEntity) {
		parseData_EntitiesList.loadingTemplate_entityIndex++;

		if (parseData_EntitiesList.loadingTemplate_entityIndex >= parseData_EntitiesList.entitiesCount) {
			return true;
		}

		ParseData_EntitiesList::CfgEntity* parseData_cfgEntity = parseData_EntitiesList.entitiesList.getPointer(parseData_EntitiesList.loadingTemplate_entityIndex);
		if (parseData_cfgEntity == nullptr) {
			throw LogicException(LOC);
		}

		// Make the cache read request.
		std::string cachePathId = CacheIds::persistentEntity_prefix + parseData_cfgEntity->instanceId + CacheIds::persistentEntity_sufix;
		if (!readCachedStringFileAsync(cachePathId)) {
			// Missing entity template cache entry.
			throw DataException();
		}
	}

	return false;
}

void GPLCC_PersistentEntitiesLoader::initLoadPhase_CreateEntities() {
	overridePersistentEntitiesIdCounter(parseData_EntitiesList.persistentEntitiesIdCounter);

	rpg3D::IGamePartCustomisedContent* gpCustomisedContent = gw->getComponentAs<rpg3D::IGamePartCustomisedContent*>(true/*mustExist*/);

	for (int i = 0; i < parseData_EntitiesList.entitiesList.count(); i++) {
		ParseData_EntitiesList::CfgEntity* parseData_cfgEntity = parseData_EntitiesList.entitiesList.getPointer(i);

		std::shared_ptr<ExtraData> extraData = std::make_shared<ExtraData>();
		EntityTemplate* entityTemplate;

		if (parseData_cfgEntity->entityTemplate->unit_type == rpg3D::Util_VoxlesGrid3DGeometry::UnitType::creature) {
			//asd2;// Save the id in the cache_data and make the custom configs as extraData. Also make the template static again.
			entityTemplate = rpg3D:: T_Unit_EditorDefaultCreature_A_V1::getNew(
				parseData_cfgEntity->entityTemplate->voxel_size, parseData_cfgEntity->entityTemplate->grid_offset,
				parseData_cfgEntity->entityTemplate->touchable_AABBShape_min, parseData_cfgEntity->entityTemplate->touchable_AABBShape_max,
				parseData_cfgEntity->entityTemplate->stackable_haulOffsetZM
			);

			extraData->setData(rpg3D::T_Unit_EditorDefaultCreature_A_V1::DATA_KEY_voxelGridZXY, base::IWrappedValue::new_Custom<Array3D<signed char>*>(parseData_cfgEntity->entityTemplate->grid.get(), -1));
			extraData->setData(rpg3D::T_Unit_EditorDefaultCreature_A_V1::DATA_KEY_materials, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(parseData_cfgEntity->entityTemplate->materialsList.get(), -1));
		} else if (parseData_cfgEntity->entityTemplate->unit_type == rpg3D::Util_VoxlesGrid3DGeometry::UnitType::hero_dummy) {
			//asd2;// Save the id in the cache_data and make the custom configs as extraData. Also make the template static again.
			entityTemplate = rpg3D::Templates_Unit_PlayerUnitDummy_A_V1::getNew(
				parseData_cfgEntity->entityTemplate->voxel_size, parseData_cfgEntity->entityTemplate->grid_offset,
				parseData_cfgEntity->entityTemplate->touchable_AABBShape_min, parseData_cfgEntity->entityTemplate->touchable_AABBShape_max,
				parseData_cfgEntity->entityTemplate->stackable_haulOffsetZM
			);

			std::shared_ptr<rpg3D::ICustomisedContentHandler::EntityMaterialsAndVoxelsGrid> unitMaterialsAndVoxelsGrid = gpCustomisedContent->getCustomisedContentHandler()->getPlayerUnitMaterialsAndVoxelsGrid();
			extraData->setData(rpg3D::UnitTemplates_PlayerA::DATA_KEY_voxelGridZXY, base::IWrappedValue::new_Custom<Array3D<signed char>*>(unitMaterialsAndVoxelsGrid->voxelGrid, -1));
			extraData->setData(rpg3D::UnitTemplates_PlayerA::DATA_KEY_materials, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(unitMaterialsAndVoxelsGrid->materialsList, -1));
		} else {
			throw LogicException(LOC);
		}

		Vector3 pos{ parseData_cfgEntity->x, parseData_cfgEntity->y, parseData_cfgEntity->z };
		float angZDeg = parseData_cfgEntity->angZDeg;
		Quaternion rot{ Vector3::Z, angZDeg };
		std::shared_ptr<std::string> instanceId = nullptr;
		if (!parseData_cfgEntity->instanceId.empty()) {
			if (parseData_cfgEntity->instanceId == *EntityIds::player_hero_dummy) {
				instanceId = EntityIds::player_hero_dummy;
			} else {
				instanceId = std::make_shared<std::string>(parseData_cfgEntity->instanceId);
			}
		}

		IWorldEntity* entity = gw->getGPMain()->createEntity(
			entityTemplate,
			pos,
			&rot/*rot*/, nullptr/*cardinalRotation*/,
			instanceId/*instanceId*/,
			0/*ownerPlayerId*/,
			extraData,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			[this](IWorldEntity* cfgEntity) -> void {
				rpg3D::ITeamModule* editorEntity_team = gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);

				rpg3D::ITeamModule* entity_team = cfgEntity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
				entity_team->setTeamIndex(editorEntity_team->getTeamIndex());
			}
		);

		parseData_cfgEntity->tmpCreatedEntity = entity;
	}

	// Handle haulers.
	for (int i = 0; i < parseData_EntitiesList.entitiesList.count(); i++) {
		ParseData_EntitiesList::CfgEntity* parseData_cfgEntity = parseData_EntitiesList.entitiesList.getPointer(i);

		IWorldEntity* entity = parseData_cfgEntity->tmpCreatedEntity;
		if (entity == nullptr) {
			// For some reason this entity wa ignorred. Just continue.
			continue;
		}

		if (!parseData_cfgEntity->haulerEntityInstanceId.empty()) {
			IWorldEntity* haulerEntity = gw->getGPMain()->getEntityWithInstanceId(parseData_cfgEntity->haulerEntityInstanceId);

			if (haulerEntity == nullptr) {
				// Ignore this and just let the entity where it is. It should have the1 stack_base pos already set.
				continue;
			}

			rpg3D::IStackableModule* haulerEntity_stackable = haulerEntity->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
			if (haulerEntity_stackable->getIsCarrying()) {
				// Ignore this and just let the entity where it is. It should have the1 stack_base pos already set.
				continue;
			}
			
			rpg3D::IStackableModule* entity_stackable = entity->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);

			haulerEntity_stackable->setCarrying(entity_stackable);
		}

		// Do this here so it won't receive the above hauler changed events on start (that causes an un-needed save on start to occur).
		pushPersistentEntity(entity, true);
	}
}

void GPLCC_PersistentEntitiesLoader::onPersistentEntitiesLoadFinished() {
	//void
}

void GPLCC_PersistentEntitiesLoader::onAsyncFHEvent(IEventListener& eventListener, base::AsyncFHEvent& event) {
	if (base::IFileHandlerAsync::EVENT_IO_failed->type == event.type) {
		// For now, just do a error.
		throw LogicException(LOC);
	} else if (base::IFileHandlerAsync::EVENT_read_string_success->type == event.type) {
		// Save the data for further processing.
		if (event.data_string == nullptr) {
			throw LogicException(LOC);
		}
		if (event.data_string->length() <= 0) {
			// Cache error occured.
			throw DataException();
		}

		data_waitingFileRead = event.data_string;
	} else {
		throw LogicException(LOC);
	}
}

bool GPLCC_PersistentEntitiesLoader::readCachedStringFileAsync(std::string& cacheId) {
	std::shared_ptr<base::IFileHandlerAsync> fh = gw->getAppAssets()->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(gw->getScenarioTemplate()->id)
		->getCacheFileHandlerAsync(cacheId);

	// Check if cache doesn't exist.
	if (fh == nullptr) {
		return false;
	}

	// Add the needed events listeners.
	fhListenersList.clear();
	
	fh->addEventListener(
		base::IFileHandlerAsync::EVENT_read_string_success->type, base::IFileHandlerAsync::EVENT_IO_failed->type,
		std::bind(&GPLCC_PersistentEntitiesLoader::onAsyncFHEvent, this, std::placeholders::_1, std::placeholders::_2),
		fhListenersList
	);

	// Start the async reads.
	data_waitingFileRead = nullptr;
	fh->readStringSPAsync();

	return true;
}

GPLCC_PersistentEntitiesLoader::~GPLCC_PersistentEntitiesLoader() {
	//void
}
