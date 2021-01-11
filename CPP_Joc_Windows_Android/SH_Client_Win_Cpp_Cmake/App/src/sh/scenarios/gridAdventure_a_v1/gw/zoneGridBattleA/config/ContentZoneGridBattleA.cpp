#include "ContentZoneGridBattleA.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/config/entity/EntityTemplatesList.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <base/statics/StaticsInit.h>
#include "../../../gw/config/entity/EntityIds.h"
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include <rpg3D/gw/manager/IGPManager.h>
#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include <rpg3D/gw/entity/template/LivingEntityTemplate.h>
#include <rpg3D/gw/entity/template/physics3D/Physics3DTemplate.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <base/opengl/util/GLUtil.h>
#include "../../zoneMain/config/ContentZoneMain.h"
#include <rpg3D/gw/config/entity/platform/T_Platform_X.h>
#include <rpg3D/gw/config/entity/platform/T_Platform_NextUp.h>
#include <rpg3D/gw/config/entity/mob/T_Mob_General_A_V1.h>
#include <rpg3D/gw/config/entity/mineral/T_StoneRock_A_V1.h>
#include <rpg3D/gw/config/entity/structure/T_GenericFloor_A_TiledX6dmY6dm.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <rpg3D/gw/turnBasedPlay/GPTurnBasedPlay.h>
#include <rpg3D/gw/grid2DContent/GPGrid2DContent.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/playerUnit/gridTurnBasedActiveTools/ILBGridTurnBasedActiveTools.h>
#include <rpg3D/gw/sim/GamePartSim.h>
#include <worldGame3D/gw/content/IGPContent.h>
#include <rpg3D/gw/entity/module/worldItem/IWorldItemModule.h>
#include <rpg3D/gw/config/entity/worldItem/T_Item_Coin_Gold_A_V1.h>
#include <rpg3D/gw/config/entity/worldItem/T_Item_HearthHeal_A_V1.h>
#include <rpg3D/gw/config/entity/worldItem/T_Item_DamageHearth_A_V1.h>
#include <rpg3D/gw/config/entity/doodad/T_Chest_General_A_V1.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <memory>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/userEntitiesController/IGPUserEntitiesController.h>
#include <rpg3D/gw/userEntitiesController/util/EntityControlType.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include "../gw/playLogicHandler/GamePartPlayLogicHandler.h"
#include <sh/scenarios/gridAdventure_a_v1/gw/zoneGridBattleA/config/util/ZoneStartExtraData.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/zoneGridBattleA/config/logicBlock/mob/chicken/LB_Mob_Chicken_TB_A_V1.h>
#include <base/MM.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/melleArea/ToolMelleArea.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <rpg3D/gw/config/entity/tool/Cfg_Tool_ChickenBeak_A_V1.h>

using namespace gridAdventure_a_v1;
using namespace gridAdventure_a_v1::GridBattleA;

std::string ContentZoneGridBattleA::NAME = "GridBattleA";

std::shared_ptr<worldGame3D::ContentZone> ContentZoneGridBattleA::FactoryFunction() {
	std::shared_ptr<worldGame3D::ContentZone> ret = std::make_shared<ContentZoneGridBattleA>();

	return ret;
}

ContentZoneGridBattleA::ContentZoneGridBattleA()
	: super(NAME)
{
	//void
}

void ContentZoneGridBattleA::onZoneActivatedPre(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivatedPre(zoneStartExtraData, gw);

	// Create the zone specific game parts.
	sp<ArrayList<int>> turnBasedTeamsList = new ArrayList<int>();
	turnBasedTeamsList->appendDirect(rpg3D::TeamsIndexesList::PLAYER_UNIT);
	turnBasedTeamsList->appendDirect(rpg3D::TeamsIndexesList::MOB);

	gw->addGamePartUninitialised(new rpg3D::GPTurnBasedPlay(
		gw,
		rpg3D::GPTurnBasedPlay::TurnMode::SingleActionTurn,
		turnBasedTeamsList
	));

	gw->addGamePartUninitialised(new rpg3D::GPGrid2DContent(
		gw, Vector2(0.6f, 0.6f), 
		false/*allowMultipleEntitiesPerGCell*/, false/*allowEntitiesOutsideOfTheGridArea*/));

	// Create the zone specific game parts.
	gw->addGamePartUninitialised(new GamePartPlayLogicHandler(gw));
}

void ContentZoneGridBattleA::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	ZoneStartExtraData* cZoneStartExtraData;
	if (zoneStartExtraData != nullptr) {
		cZoneStartExtraData = zoneStartExtraData->getAs<ZoneStartExtraData*>();
	} else {
		cZoneStartExtraData = nullptr;
	}

	float gridCellSize = 0.6f;

	{
		int eId = 0;
		signed char O = -1;// Nothing
		signed char A = eId++;
		signed char B = eId++;

		// Need to set the gBounds here because the player entity will connect to it after this functio call.
		std::shared_ptr<Array2D<signed char>>tilingGrid = std::make_shared<Array2D<signed char>>(
			4, 10,
			false, true,
			new signed char[4 * 10]{
				B,A,B,A,
				A,B,A,B,
				B,A,B,A,
				A,B,A,B,
				B,A,B,A,

				A,B,A,B,
				B,A,B,A,
				A,B,A,B,
				B,A,B,A,
				A,B,A,B
			}
		);

		Bounds2DInt gridBounds{
			0/*xMin*/, tilingGrid->getLengthA()/*xMax*/,
			0/*yMin*/, tilingGrid->getLengthB()/*yMax*/
		};
		if (gridBounds.getLenX() <= 0 || gridBounds.getLenY() <= 0) {
			// Grid bounds were not initialised correctly.
			throw LogicException(LOC);
		}
		gw->getComponentAs<rpg3D::IGPGrid2DContent*>(true/*mustExist*/)->initTrackingRegion_gridBounds(gridBounds);

		std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
		tilesConfigList->append_emplace(rpg3D::T_GenericFloor_A_TiledX6dmY6dm::gridGrassA);
		tilesConfigList->append_emplace(rpg3D::T_GenericFloor_A_TiledX6dmY6dm::gridGrassB);

		std::shared_ptr<ExtraData> extraData = std::make_shared<ExtraData>();
		
		std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
			rpg3D::T_GenericFloor_A_TiledX6dmY6dm::TileVoxelsCount,
			-1/*enabledSidesTriangulationBitMask*/,
			tilesConfigList,
			tilingGrid,
			nullptr/*gridGeometryOffset*/
		);
		extraData->setData(rpg3D::T_GenericFloor_A_TiledX6dmY6dm::DATA_KEY_materials, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(rpg3D::T_GenericFloor_A_TiledX6dmY6dm::materialsList, -1));
		extraData->setData(EntityTemplate::ExtraDataID_tilingInformation, base::IWrappedValue::new_CustomB<std::shared_ptr<graphics::TilingInformation>>(tilingInformation, "graphics::TilingInformation"));

		Vector3 pos{ 0, 0, 0 };
		Quaternion rot{ Vector3::Z, 0.0f };
		IWorldEntity* entity = gw->getGPMain()->createEntity(
			rpg3D::T_GenericFloor_A_TiledX6dmY6dm::TEMPLATE,
			pos,
			&rot/*rot*/, nullptr/*cardinalRotation*/,
			nullptr,
			0,
			extraData/*extraData*/,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			nullptr
		);
	}

	float goldAmount1 = 1;
	float healAmount1 = 1;
	float damageAmount1 = 1;

	IWorldEntity* homeTeleporterPlatform = nullptr;
	if (cZoneStartExtraData == nullptr || cZoneStartExtraData->zoneLevelIndex == 0) {
		// Show the exit platform only on the first zoneLevel.
		homeTeleporterPlatform = createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (0.0f + 0.5f) * gridCellSize, 0.0f, 0.0f, rpg3D::T_Platform_X::TEMPLATE, nullptr/*instanceId*/, 
			std::make_shared<std::string>(
				worldGame3D::ScriptCommands::cmd_setZone + worldGame3D::ScriptCommands::commandPartsDelimiter
				+ Main::ContentZoneMain::NAME
			)/*platformActivationScript*/, gw, 
			nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/);
	}

	createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (2.0f + 0.5f) * gridCellSize, rpg3D::T_Item_HearthHeal_A_V1::GROUND_PLACEMENT_OFFSET_Z, -90.0f, rpg3D::T_Item_HearthHeal_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
		nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, &healAmount1/*itemHealAmount*/, nullptr/*itemDamageAmount*/);
	createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (3.0f + 0.5f) * gridCellSize, rpg3D::T_Item_DamageHearth_A_V1::GROUND_PLACEMENT_OFFSET_Z, -90.0f, rpg3D::T_Item_DamageHearth_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
		nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, &damageAmount1/*itemDamageAmount*/);
	createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (4.0f + 0.5f) * gridCellSize, 0.1f, -90.0f, rpg3D::T_Chest_General_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
		nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/);

	{
		LB_Mob_Chicken_TB_A_V1* logicBlock = new LB_Mob_Chicken_TB_A_V1();
		IWorldEntity* entity = createConfigEntity((Math::Rand::nextInt(1, 2) + 0.5f) * gridCellSize, (5.0f + 0.5f) * gridCellSize, 0.1f, -90.0f, rpg3D::T_Mob_General_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
			&rpg3D::TeamsIndexesList::MOB/*teamId*/, nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/, nullptr/*projectileDamage*/,
			logicBlock/*logicBlock*/, static_cast<ILB_Mob_Chicken_TB_A_V1*>(logicBlock)/*logicBlockInterfaceRawPtr*/);

		rpg3D::IToolsHandlerModule* entity_toolsHandler = entity->getComponentAs<rpg3D::IToolsHandlerModule*>(true/*mustExist*/);

		sp<rpg3D::Tool> toolBeak = rpg3D::Cfg_Tool_ChickenBeak_A_V1::getOrCreateTemplate()->createNewTool(
			gw->getAppAssets()
		);
		sp<rpg3D::ToolMelleArea> cToolBeak = toolBeak.dCast<rpg3D::ToolMelleArea>(true/*mustExtend*/);
		cToolBeak->addPayload(std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f));
		
		entity_toolsHandler->addTool(
			cToolBeak
		);

		entity_toolsHandler->selectTool(
			cToolBeak,
			0/*slotIndex*/
		);
	}
	
	createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (6.0f + 0.5f) * gridCellSize, rpg3D::T_Item_HearthHeal_A_V1::GROUND_PLACEMENT_OFFSET_Z, -90.0f, rpg3D::T_Item_HearthHeal_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
		nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, &healAmount1/*itemHealAmount*/, nullptr/*itemDamageAmount*/);
	createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (7.0f + 0.5f) * gridCellSize, rpg3D::T_Item_DamageHearth_A_V1::GROUND_PLACEMENT_OFFSET_Z, -90.0f, rpg3D::T_Item_DamageHearth_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
		nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, &damageAmount1/*itemDamageAmount*/);
	createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (8.0f + 0.5f) * gridCellSize, 0.1f, -90.0f, rpg3D::T_Chest_General_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
		nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/);
	createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (8.0f + 0.5f) * gridCellSize, rpg3D::T_Item_Coin_Gold_A_V1::GROUND_PLACEMENT_OFFSET_Z, -90.0f, rpg3D::T_Item_Coin_Gold_A_V1::TEMPLATE, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
		nullptr/*teamId*/, &goldAmount1/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/);

	IWorldEntity* nextLevelTeleporterPlatform = createConfigEntity((Math::Rand::nextInt(0, 3) + 0.5f) * gridCellSize, (9.0f + 0.5f) * gridCellSize, 0.0f, 0.0f, rpg3D::T_Platform_NextUp::TEMPLATE, EntityIds::enterNextGridBattleLevel/*instanceId*/, nullptr/*platformActivationScript*/, gw,
		nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/);

	Vector3 playerSpawnPos{};
	if (zoneStartExtraData != nullptr && zoneStartExtraData->teleportTargetEntityInstanceID != nullptr) {
		IWorldEntity* spawnLocationEntity = gw->getGPMain()->getEntityWithInstanceId(*zoneStartExtraData->teleportTargetEntityInstanceID);
		if (spawnLocationEntity == nullptr) {
			throw LogicException(LOC);
		}
		playerSpawnPos.set(spawnLocationEntity->getPos());

		if (spawnLocationEntity->hasComponent(rpg3D::IPlatformModule::TYPE)) {
			// Ignore the platform activation for a few frames if the player was spawned on it.
			gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(spawnLocationEntity->getInstanceId());
		}
	} else if(homeTeleporterPlatform != nullptr) {
		playerSpawnPos.set(homeTeleporterPlatform->getPos());

		if (homeTeleporterPlatform->hasComponent(rpg3D::IPlatformModule::TYPE)) {
			// Ignore the platform activation for a few frames if the player was spawned on it.
			gw->getManagerAs<rpg3D::IGPManager*>()->addPlayerSpawnIgnoredPlatformsActivation(homeTeleporterPlatform->getInstanceId());
		}
	}else{
		// Spawn the player randomly on the first row.
		playerSpawnPos.set((Math::Rand::nextInt(0, 3) + 0.5f)* gridCellSize, (0.0f + 0.5f)* gridCellSize, 0.0f);
	}

	// Give the player full hp. This is also done in case he loses all hp.
	rpg3D::ILivingEntityModule* livingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::ILivingEntityModule*>(true/*mustExist*/);
	livingEntity->setCurrentHP(livingEntity->getT()->maxHP);

	gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->setPos(playerSpawnPos);

	rpg3D::IControllerModule* entity_controller = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);
	entity_controller->setSingleActiveGroupLogicBlock(rpg3D::playerUnit::ILBGridTurnBasedActiveTools::GroupID, rpg3D::playerUnit::ILBGridTurnBasedActiveTools::ID);

	IMovingEntityModule* movingEntity = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	movingEntity->addAction_stopMovement(true/*stopPhysicsLinearVelocitiesAlso*/);
	movingEntity->addAction_teleportToDestination(playerSpawnPos);
	movingEntity->addAction_ActionDirectRotateZ(worldGame3D::EntityAction::Priorities::NormalPlus1, 90.0f);

	Color col{ "#101010FF" };
	GLUtil::gl->setCachedClearColor(col);

	{
		Vector3 offsetDir{ 0.0f * 0.8f, -1.0f * 0.8f, 3.0f * 0.8f };
		offsetDir.nor();
		Vector3 focusPointOffset{ 0.0f, 0.0f, 0.0f };
		gw->getGPSim()->configCamera_followerThirdPersonCamera(
			offsetDir,
			4.0f, 6.0f,
			6.0f, 11.0f,
			focusPointOffset
		);

		gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(rpg3D::EntityControlType::GridTurnBased);
		gw->getGPSim()->setCameraMode(GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation, true);
	}
}

void ContentZoneGridBattleA::onZoneActivatedB(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivatedB(zoneStartExtraData, gw);

	gw->getComponentAs<rpg3D::IGPTurnBasedPlay*>(true/*mustExist*/)->setIsTurnBasedPlayEnabled(true);
}

IWorldEntity* ContentZoneGridBattleA::createConfigEntity(
	float x, float y, float z,
	float rotZ,
	EntityTemplate* entityTemplate,
	std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript,
	base::IGameWorld* gw,
	const int* teamId,
	float* worldItemGoldAmount, float* itemHealAmount, float* itemDamageAmount, float* projectileDamage,
	rpg3D::ILogicBlock* logicBlock, void* logicBlockInterfaceRawPtr	)
{
	Vector3 pos{ x, y, z };
	Quaternion rot{ Vector3::Z, rotZ };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		entityTemplate,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		instanceId,
		0,
		nullptr,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[teamId, worldItemGoldAmount, itemHealAmount, itemDamageAmount, projectileDamage, logicBlock, logicBlockInterfaceRawPtr, platformActivationScript](IWorldEntity* entity) -> void {
			if (platformActivationScript != nullptr) {
				entity->getComponentAs<rpg3D::IPlatformModule*>(true/*mustExist*/)->setActivation_script(platformActivationScript);
			}
			
			if (teamId != nullptr) {
				rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
				mob_team->setTeamIndex(*teamId);
			}
			if (worldItemGoldAmount != nullptr) {
				rpg3D::IWorldItemModule* entity_worldItem = entity->getComponentAs<rpg3D::IWorldItemModule*>(true/*mustExist*/);
				entity_worldItem->addPayload(std::make_shared<rpg3D::EffectPayload>(
					rpg3D::EffectPayload::Types::giveGoldAmount, rpg3D::EffectPayload::Target::OTHER_ENTITY, *worldItemGoldAmount
				));
			}
			if (itemHealAmount != nullptr) {
				rpg3D::IWorldItemModule* entity_worldItem = entity->getComponentAs<rpg3D::IWorldItemModule*>(true/*mustExist*/);
				entity_worldItem->addPayload(std::make_shared<rpg3D::EffectPayload>(
					rpg3D::EffectPayload::Types::fixedValueHeal, rpg3D::EffectPayload::Target::OTHER_ENTITY, *itemHealAmount
				));
			}
			if (itemDamageAmount != nullptr) {
				rpg3D::IWorldItemModule* entity_worldItem = entity->getComponentAs<rpg3D::IWorldItemModule*>(true/*mustExist*/);
				entity_worldItem->addPayload(std::make_shared<rpg3D::EffectPayload>(
					rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, *itemDamageAmount
				));
			}
			if (projectileDamage != nullptr) {
				entity->getComponentAs<rpg3D::IProjectileModule*>(true/*mustExist*/)->addPayload(
					std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, *projectileDamage)
				);
			}

			if (logicBlock != nullptr) {
				rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);
				entity_controller->registerLogicBlock(logicBlock, logicBlock->getId(), logicBlockInterfaceRawPtr);
				entity_controller->setSingleActiveGroupLogicBlock(logicBlock->getGroupID(), logicBlock->getId());
			}
		}
	);
	
	return entity;
}

void ContentZoneGridBattleA::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);
	
	rpg3D::IControllerModule* entity_controller = gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);
	entity_controller->setSingleActiveGroupLogicBlock(rpg3D::playerUnit::ILBGridTurnBasedActiveTools::GroupID, -1);

	gw->disposeGamePart<rpg3D::IGPTurnBasedPlay*>();
	gw->disposeGamePart<rpg3D::IGPGrid2DContent*>();

	gw->disposeGamePart<IGamePartPlayLogicHandler*>();
}

ContentZoneGridBattleA::~ContentZoneGridBattleA() {
	//void
}
