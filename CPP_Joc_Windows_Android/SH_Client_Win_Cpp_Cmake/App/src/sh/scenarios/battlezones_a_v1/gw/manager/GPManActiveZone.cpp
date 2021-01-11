#include <base/list/Array2D.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include "GPManActiveZone.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/module/platform/IPlatformModule.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <sh/scenarios/battlezones_a_v1/gw/zone/ZoneTemplate_Battle_WESN_A_V1.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/rangeInertialProjectile/ToolRangeInertialProjectile.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/projectile/ProjectileConfigsList.h>
#include <rpg3D/gw/entity/module/toolsHandler/ToolsHandlerModule.h>
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/scenariohandler/customisedContentHandler/ICustomisedContentHandler.h>
#include <rpg3D/gw/customisedContent/IGamePartCustomisedContent.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <base/app/IAppAssets.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_Bow_Basic_A_V1.h>
#include <base/util/StringUtil.h>

#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveDPadLocalFrontBack/ToolMDPadLocalFrontBack.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDestination_TMoveDirection/ToolMGlobalDestination_TMoveDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDirection_TMoveDirection/ToolMGlobalDirection_TMoveDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveGlobalDirection/ToolMGlobalDirection.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/moveLocalStrafe/ToolMLocalStrafe.h>

#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnDPASSY_LMOSP/ToolTDPASSY_LMOSP.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnGlobalFocusDirectionYaw/ToolTGlobalFocusDirectionYaw.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/playerUnit/turnMouseOrSensorPitchYaw/ToolTMouseOrSensorPitchYaw.h>

#include <rpg3D/gw/entity/module/controller/logicBlock/playerUnit/generalActiveTool/LBGeneralActiveTools.h>
#include <worldGame3D/gw/entityGroups/util/IEntsGroup.h>

using namespace battlezones_a_v1;

GPManActiveZone::GPManActiveZone(base::IGameWorld* gw)
    : super(gw)
{
	gw->addEventListener(
		GWEvent_onZonesGroupFinished->type,
		std::bind(&GPManActiveZone::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_post->type,
		std::bind(&GPManActiveZone::onGWEvent_gameLoop_post, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

ZoneConfig* GPManActiveZone::getActiveZoneConfig() {
	return activeZoneConfig;
}

void GPManActiveZone::setActiveZone(
	std::string& zonesGroupId, std::string& zoneName,
	std::shared_ptr<std::string> spawnTargetEntityId,
	bool isZonePrecleared,
	bool activateOnLoopTick)
{
	if(activateOnLoopTick) {
		delayedZoneActivation_zonesGroupId = zonesGroupId;
		delayedZoneActivation_zoneName = zoneName;
		delayedZoneActivation_spawnTargetEntityId = spawnTargetEntityId;
		delayedZoneActivation_isZonePrecleared = isZonePrecleared;

		isDelayedZoneActivation = true;

		return ;
	}else {
		isDelayedZoneActivation = false;
	}

	// Clear old zone content.
	if(activeZoneConfig != nullptr) {
		onActiveZoneDeactivate_pre();

		GlobalAccessLogHandler::devLogHandler->postDebug(std::string("GW zone. Disabled: ") + activeZoneConfig->groupId + ", " + activeZoneConfig->zoneName + ".");

		gw->getGPMain()->removeAllZoneOwnedEntities();

		activeZoneConfig = nullptr;
		activeZoneFlags.clear();
	}

	if (zonesGroupId.length() == 0) {
		// No active zone required.
		return;
	}

	activeZoneConfig = getZoneConfig(zonesGroupId, zoneName);
	if(activeZoneConfig == nullptr) {
		// Zone util not found.
		throw LogicException(LOC);
	}
	GlobalAccessLogHandler::devLogHandler->postDebug(std::string("GW zone. Enabled: ") + activeZoneConfig->groupId + ", " + activeZoneConfig->zoneName + ".");

    // Get zone template.
	ZoneTemplate* zoneTemplate = ZoneTemplate::entriesMap.getDirect(activeZoneConfig->zoneTemplateId);
	if(zoneTemplate == nullptr) {
		throw LogicException(LOC);
	}

	// Create content.
	for(int i=0;i<zoneTemplate->groundPatchesList.size();i++) {
        ZoneTemplate::GroundPatchSpawnInfo* spawnInfo = zoneTemplate->groundPatchesList.getPointer(i);

	    EntityTemplate* entityTemplate = EntityTemplate::fromId(
		    spawnInfo->scenarioNamespace,
		    spawnInfo->id
        );

		IWorldEntity* entity = gw->getGPMain()->createEntity(
			entityTemplate,
			spawnInfo->pos,
			&spawnInfo->rot/*rot*/, nullptr/*cardinalRotation*/,
			nullptr,
			0,
			nullptr,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			nullptr
		);

		IVoxelisedStructureModule* voxelisedStructure = entity->getComponentAs<IVoxelisedStructureModule*>(true/*mustExist*/);
		voxelisedStructure->setStructure(
			spawnInfo->voxelsGrid,
			spawnInfo->materialsList
		);
    }
	for(int i=0;i<zoneTemplate->environmentEntitiesList.size();i++) {
        ZoneTemplate::EntitySpawnInfo* spawnInfo = zoneTemplate->environmentEntitiesList.getPointer(i);

		if(spawnInfo->spawnTestFlags.count() > 0) {
			// Entities with conditional flags are handled by the zone flags change system.
			// Add to a special list.
			addConditionalEntitySpawnInfo(spawnInfo);
		}else {
			EntityTemplate* entityTemplate = EntityTemplate::fromId(
				spawnInfo->scenarioNamespace,
				spawnInfo->id
			);

			IWorldEntity *entity = gw->getGPMain()->createEntity(
				entityTemplate,
				spawnInfo->pos,
				&spawnInfo->rot/*rot*/, nullptr/*cardinalRotation*/,
				spawnInfo->instanceId,
				0,
				nullptr,
				true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
				nullptr
			);
			(void) entity;
		}
	}

	IWorldEntity* playerEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/);
	if(playerEntity == nullptr) {
		createPlayerUnit(
			0.0f,
			0.0f,
			0.0f
		);

		playerEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/);
	}

	// Stop the player movement if any.
	if(playerEntity == nullptr) {
		throw LogicException(LOC);
	}

	activeZoneFlags.setFlags(activeZoneConfig->flags);
	activeZoneFlags.setFlag(ZoneTemplate_Battle_WESN_A_V1::FLAG_isZoneCleared, isZonePrecleared);
	onZoneFlagsChange();

	// Move player unit to spawn location.
	// Get entity with id: spawnTargetEntityId;
	IWorldEntity* spawnLocationEntity = gw->getGPMain()->getEntityWithInstanceId(*spawnTargetEntityId);
	if(spawnLocationEntity == nullptr) {
		throw LogicException(LOC);
	}

	// Move the player unit to the target location.
	// Do a extra set pos to avoid the teleportation on the first frame_tick.
	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(*spawnLocationEntity->getPos());
	IMovingEntityModule* movingEntity = playerEntity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_teleportToDestination(*spawnLocationEntity->getPos());

	// Check if the entity has a platform module before calling this.
	if(spawnLocationEntity->hasComponent(rpg3D::IPlatformModule::TYPE)) {
		// Ignore the platform activation for a few frames if the player was spawned on it.
		addPlayerSpawnIgnoredPlatformsActivation(spawnTargetEntityId);
	}

	if(isRegenerateZonesGroupFlag) {
		isRegenerateZonesGroupFlag = false;

		generateNewZonesGroup();
	}

	onActiveZoneChanged();
}

void GPManActiveZone::setRegenerateZonesGroupFlag() {
	isRegenerateZonesGroupFlag = true;
}

void GPManActiveZone::onGWEvent_gameLoop_post(IEventListener& eventListener, base::GWEvent& event) {
	if(isDelayedZoneActivation) {
		setActiveZone(
			delayedZoneActivation_zonesGroupId, delayedZoneActivation_zoneName,
			delayedZoneActivation_spawnTargetEntityId,
			delayedZoneActivation_isZonePrecleared,
			false
		);

		isDelayedZoneActivation = false;
	}
}

void GPManActiveZone::createPlayerUnit(float x, float y, float z) {
	std::shared_ptr<ExtraData> extraData = std::make_shared<ExtraData>();

	rpg3D::IGamePartCustomisedContent* gpCustomisedContent = gw->getComponentAs<rpg3D::IGamePartCustomisedContent*>(true/*mustExist*/);
	std::shared_ptr<rpg3D::ICustomisedContentHandler::EntityMaterialsAndVoxelsGrid> playerUnitMaterialsAndVoxelsGrid = gpCustomisedContent->getCustomisedContentHandler()->getPlayerUnitMaterialsAndVoxelsGrid();
	extraData->setData(rpg3D::UnitTemplates_PlayerA::DATA_KEY_voxelGridZXY, base::IWrappedValue::new_Custom<Array3D<signed char>*>(playerUnitMaterialsAndVoxelsGrid->voxelGrid, -1));
	extraData->setData(rpg3D::UnitTemplates_PlayerA::DATA_KEY_materials, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(playerUnitMaterialsAndVoxelsGrid->materialsList, -1));

	Vector3 pos{x, y, z};
	Quaternion rot{Vector3::Z, -90.0f};
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		EntityTemplatesList::Unit_PlayerA,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		EntityIds::playerUnit,
		0,
		extraData,
		false/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[this](IWorldEntity* entity) {
			rpg3D::ITeamModule* entity_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
			entity_team->setTeamIndex(rpg3D::TeamsIndexesList::PLAYER_UNIT);

			rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);

			rpg3D::IToolsHandlerModule* entity_toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
			
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMDPadLocalFrontBack>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDestination_TMoveDirection>(
				-1/*movementTypeOverride*/, false/*usesGridMovement*/, rpg3D::playerUnit::ToolMGlobalDestination_TMoveDirection::DestinationType::GroundAndSomeEntities/*destinationType*/
			), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDirection_TMoveDirection>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMGlobalDirection>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolMLocalStrafe>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTDPASSY_LMOSP>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTGlobalFocusDirectionYaw>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			entity_toolsHandler->addTool(msp<rpg3D::playerUnit::ToolTMouseOrSensorPitchYaw>(), gw->getAppAssets()->getEmbeddedAssetsManager());
			
			rpg3D::playerUnit::LBGeneralActiveTools* lBGeneralActiveTools = new rpg3D::playerUnit::LBGeneralActiveTools();
			entity_controller->registerLogicBlock(lBGeneralActiveTools, rpg3D::playerUnit::ILBGeneralActiveTools::ID, static_cast<rpg3D::playerUnit::ILBGeneralActiveTools*>(lBGeneralActiveTools));

			entity_controller->setSingleActiveGroupLogicBlock(rpg3D::playerUnit::ILBGeneralActiveTools::GroupID, rpg3D::playerUnit::ILBGeneralActiveTools::ID);
		}
	);

	rpg3D::IToolsHandlerModule* entity_toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);
	
	sp<rpg3D::Tool> tool1 = rpg3D::Cfg_Tool_Bow_Basic_A_V1::getOrCreateTemplate()->createNewTool(
		gw->getAppAssets()
	);
	sp<rpg3D::ToolRangeInertialProjectile> cTool1 = tool1.dCast<rpg3D::ToolRangeInertialProjectile>(true/*mustExtend*/);
	cTool1->setProjectileConfig(ProjectileConfigsList::Arrow_Basic_A_V1);
	entity_toolsHandler->addTool(
		cTool1
	);
	entity_toolsHandler->selectTool(
		cTool1,
		0
	);

	gw->getGPContent()->getPlayerEntitiesGroup()->addEntity(entity);
	gw->getGPContent()->setIsSelectorEnabled(true);
	gw->getGPContent()->setSelectManagedPlayerEntity(entity);
}

void GPManActiveZone::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == GWEvent_onZonesGroupFinished->type) {
		isRegenerateZonesGroupFlag = true;
	}else {
		throw LogicException(LOC);
	}
}

void GPManActiveZone::disposePrePre() {
	setActiveZone(
		StringUtil::EMPTY/*zonesGroupId*/, StringUtil::EMPTY/*zoneName*/,
		nullptr/*spawnTargetEntityId*/,
		false/*isZonePrecleared*/,
		false/*activateOnLoopTick*/
	);
	
	super::disposePrePre();
}

GPManActiveZone::~GPManActiveZone() {
    //void
}
