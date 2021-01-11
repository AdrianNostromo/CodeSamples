#include "RoomGenerator.h"
#include "../roomContent/RoomContent.h"
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation.h>
#include <base/map/ExtraData.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <graphics/visual3d/util/VoxelSides.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include "config/CfgRoomGenerator.h"
#include <worldGame3D/gw/entity/util/ConditionalWorldEntitySpawnConfig.h>
#include <rpg3D/gw/mobsSpawnSystem/IOGPMobsSpawnSystem.h>
#include <rpg3D/gw/config/entity/platform/TC_TeleporterPlatform_Up.h>
#include <rpg3D/gw/config/entity/platform/TC_BrokenTeleporterPlatform_Up.h>
#include "../../../gw/config/entity/EntityIds.h"
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <rpg3D/gw/config/entity/platform/TC_Platform_Teleporter_Home.h>
#include <worldGame3D/scripts/ScriptCommands.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include <base/list/ArrayList.h>
#include <base/object/WrappedValue.h>
#include <rpg3D/gw/config/logicBlock/mob/LB_Mob_ImpactChaser_A_V1.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectKnockback.h>
#include <rpg3D/gw/entity/module/statusEffects/statusEffect/config/CfgStatusEffectStun.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/gw/config/entity/mob/T_Mob_Chicken_A_V1.h>
#include <rpg3D/gw/config/entity/mob/T_Mob_ZombieChickent_A_V1.h>
#include <rpg3D/gw/config/entity/mob/T_Mob_Frog_A_V1.h>
#include <rpg3D/gw/config/entity/worldItem/T_Item_Coin_Gold_A_V1.h>
#include <rpg3D/gw/config/entity/worldItem/T_Item_HearthHeal_A_V1.h>
#include <rpg3D/gw/config/entity/worldItem/T_Item_DamageHearth_A_V1.h>
#include <rpg3D/gw/config/entity/doodad/T_Chest_General_A_V1.h>
#include <worldGame3D/gw/entity/util/WorldEntitySpawnConfig.h>
#include <sh/scenarios/roomgeons_a_v1/gw/zoneArenaRoom/config/entity/T_TownTeleporter_Wood.h>
#include <sh/scenarios/roomgeons_a_v1/gw/zoneArenaRoom/config/entity/T_BrokenTownTeleporter_Stone.h>

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

RoomGenerator::RoomGenerator(sp<CfgRoomGenerator> config, int seed)
	: super(config, seed)
{
	//void
}

void RoomGenerator::generate() {
	super::generate();

	initTilesGrid(prefabTilesGSize);

	// Place prefabs.
	placePrefabs();

	generateWallsAndDoors();

	// Build entities.
	populateFloorsEntities();
	populateWallsEntities();

	Vector3 pos{};

	placeItems();
	placeMobs();
}

void RoomGenerator::placePrefabs() {
	// Place center prefabs.
	//asd_01;// for testing, set the entire room to the center area to test the floor tiling.
	if (roomPrefabsCount_withExitHallways.x != 5 || roomPrefabsCount_withExitHallways.y != 5) {
		// CUrrently only this size is manually implemented for testing.
		throw LogicException(LOC);
	}

	struct E {
		Array2D<PrefabTile>* prefab;

		bool preFlipX;
		bool preFlipY;
		int postRotDeg;
	};

	Array2D<PrefabTile>* O = &prefabGrid_empty;
	Array2D<PrefabTile>* V = &prefabGrid_room_edge_wall;
	Array2D<PrefabTile>* C = &prefabGrid_room_center;
	Array2D<PrefabTile>* L = &prefabGrid_room_corner;
	Array2D<PrefabTile>* D = &prefabGrid_room_edge_door;
	Array2D<PrefabTile>* H = &prefabGrid_exitHallway;
	Array2D<E> prefabsPlacementsArray{
		5/*_lengthA*/, 5/*_lengthB*/,
		false/*invertA*/, true/*invertB*/,
		new E[5 * 5] {
			E{O,false,false,0},E{O,false,false,0},E{H,false,false,-90},E{O,false,false,0},E{O,false,false,0},
			E{O,false,false,0},E{L,false,false,-90},E{D,false,false,-90},E{L,false,false,180},E{O,false,false,0},
			E{H,false,false,0},E{D,false,false,0},E{C,false,false,0},E{D,false,false,180},E{H,false,false,180},
			E{O,false,false,0},E{L,false,false,0},E{D,false,false,90},E{L,false,false,90},E{O,false,false,0},
			E{O,false,false,0},E{O,false,false,0},E{H,false,false,90},E{O,false,false,0},E{O,false,false,0}
		}/*data*/
	};

	for (int i = 0; i < prefabsPlacementsArray.getLengthA(); i++) {
		for (int j = 0; j < prefabsPlacementsArray.getLengthB(); j++) {
			E& entry = prefabsPlacementsArray.getReference(i, j);

			Vector2Int placementPrefabsGPos{ i, j };
			placePrefabInGrid(
				placementPrefabsGPos,
				*entry.prefab,
				entry.preFlipX/*flipX*/, entry.preFlipY/*flipY*/, entry.postRotDeg/*postRotDeg*/
			);
		}
	}

	Vector3 pos{};
	Quaternion rot{};

	{
		pos.set(
			2.0f * (config->tileSizeCm / 100.0f),
			(tilesGridSize.y / 2.0f)* (config->tileSizeCm / 100.0f),
			0.0f
		);
		rot.set(Vector3::Z, 0.0f);

		// Prev room or town always broken teleporter.
		EntityTemplate* entityTemplate = nullptr;
		if (config->isFirstRoomInSequence) {
			// Show a broken town teleporter platform.
			entityTemplate = T_BrokenTownTeleporter_Stone::getOrCreateTemplate();
		} else {
			// Show a broken prev room teleporter platform.
			entityTemplate = rpg3D::TC_BrokenTeleporterPlatform_Up::getOrCreateTemplate(8/*voxelSizeCM*/);
		}

		roomContent->teleportersList.appendEmplace(
			entityTemplate, pos, rot,
			EntityIds::homeTeleporter/*instanceId*/,
			nullptr/*platformActivationScript*/
		);
	}

	{
		// Next room or town conditional teleporter.
		EntityTemplate* entityTemplate_active = nullptr;
		EntityTemplate* entityTemplate_broken = nullptr;
		if (config->isLastRoomInSequence) {
			// Show a conditional town teleporter.
			entityTemplate_active = T_TownTeleporter_Wood::getOrCreateTemplate();
			entityTemplate_broken = T_BrokenTownTeleporter_Stone::getOrCreateTemplate();
		} else {
			// Show a conditional next room teleporter.
			entityTemplate_active = rpg3D::TC_TeleporterPlatform_Up::getOrCreateTemplate(8/*voxelSizeCM*/);
			entityTemplate_broken = rpg3D::TC_BrokenTeleporterPlatform_Up::getOrCreateTemplate(8/*voxelSizeCM*/);
		}

		createConditionalZoneChangeTeleporter(
			*pos.set(
				(tilesGridSize.x - 2.0f)* (config->tileSizeCm / 100.0f),
				(tilesGridSize.y / 2.0f)* (config->tileSizeCm / 100.0f),
				0.0f
			),
			-90.0f,
			entityTemplate_active, EntityIds::nextZoneInSequenceTeleporter/*instanceId*/,
			entityTemplate_broken
		);
	}
}


void RoomGenerator::placeItems() {
	// Create items.
	Vector3 pos{};
	Quaternion rot{};

	sp<float> goldAmount1 = msp<float>(1);
	sp<float> healAmount1 = msp<float>(1);
	sp<float> damageAmount1 = msp<float>(1);

	Vector3 roomCenterPos{
		(tilesGridSize.x / 2.0f) * (config->tileSizeCm / 100.0f),
		(tilesGridSize.y / 2.0f) * (config->tileSizeCm / 100.0f),
		0.0f
	};

	roomContent->worldItemsList.appendEmplace(
		*pos.set(
			roomCenterPos.x + 1.0f * (config->tileSizeCm / 100.0f),
			roomCenterPos.y + 0.0f * (config->tileSizeCm / 100.0f),
			roomCenterPos.z + rpg3D::T_Item_HearthHeal_A_V1::GROUND_PLACEMENT_OFFSET_Z
		),
		*rot.set(Vector3::Z, -90.0f),
		rpg3D::T_Item_HearthHeal_A_V1::TEMPLATE,
		nullptr/*worldItemGoldAmount*/, healAmount1/*itemHealAmount*/, nullptr/*itemDamageAmount*/
	);
	roomContent->worldItemsList.appendEmplace(
		*pos.set(
			roomCenterPos.x + -1.0f * (config->tileSizeCm / 100.0f),
			roomCenterPos.y + 0.0f * (config->tileSizeCm / 100.0f),
			roomCenterPos.z + rpg3D::T_Item_DamageHearth_A_V1::GROUND_PLACEMENT_OFFSET_Z
		),
		*rot.set(Vector3::Z, -90.0f),
		rpg3D::T_Item_DamageHearth_A_V1::TEMPLATE,
		nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, damageAmount1/*itemDamageAmount*/
	);
	roomContent->worldItemsList.appendEmplace(
		*pos.set(
			roomCenterPos.x + 0.0f * (config->tileSizeCm / 100.0f),
			roomCenterPos.y + 1.0f * (config->tileSizeCm / 100.0f),
			roomCenterPos.z + 0.1f
		),
		*rot.set(Vector3::Z, -90.0f),
		rpg3D::T_Chest_General_A_V1::TEMPLATE,
		nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/
	);
	roomContent->worldItemsList.appendEmplace(
		*pos.set(
			roomCenterPos.x + 0.0f * (config->tileSizeCm / 100.0f),
			roomCenterPos.y + -1.0f * (config->tileSizeCm / 100.0f),
			roomCenterPos.z + rpg3D::T_Item_Coin_Gold_A_V1::GROUND_PLACEMENT_OFFSET_Z
		),
		*rot.set(Vector3::Z, -90.0f),
		rpg3D::T_Item_Coin_Gold_A_V1::TEMPLATE,
		goldAmount1/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/
	);
}

void RoomGenerator::placeMobs() {
	Vector3 pos{};
	Quaternion rot{};

	Vector3 roomCenterPos{
		(tilesGridSize.x / 2.0f) * (config->tileSizeCm / 100.0f),
		(tilesGridSize.y / 2.0f) * (config->tileSizeCm / 100.0f),
		0.0f
	};

	roomContent->mobsSpawnList = std::make_shared<ArrayList<worldGame3D::WorldEntitySpawnConfig>>();

	std::shared_ptr<ArrayList<std::shared_ptr<rpg3D::EffectPayload>>> impactPayloadsList = std::make_shared<ArrayList<std::shared_ptr<rpg3D::EffectPayload>>>();
	{
		impactPayloadsList->appendDirect(std::make_shared<rpg3D::EffectPayload>(
			rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, 1.0f
		));

		std::shared_ptr<rpg3D::CfgStatusEffectKnockback> seKnockback = std::make_shared<rpg3D::CfgStatusEffectKnockback>(
			0.8f, //durationS
			0.9f, //distance
			0.0f //jumpHeight
		);
		std::shared_ptr<rpg3D::CfgStatusEffectStun> seStun = std::make_shared<rpg3D::CfgStatusEffectStun>(-1.0f);
		seKnockback->addSubEffect(std::static_pointer_cast<rpg3D::CfgStatusEffect>(seStun), true);
		impactPayloadsList->appendDirect(std::make_shared<rpg3D::EffectPayload>(
			rpg3D::EffectPayload::Types::statusEffect, rpg3D::EffectPayload::Target::SELF_ENTITY, std::static_pointer_cast<rpg3D::CfgStatusEffect>(seKnockback)
		));
	}

	{
		rpg3D::LB_Mob_ImpactChaser_A_V1* lBlock = new rpg3D::LB_Mob_ImpactChaser_A_V1(3.0f/*aggroDistance*/, 6.0f/*aggroLossDistance*/);

		roomContent->mobsSpawnList->append_emplace(
			*pos.set(
				roomCenterPos.x + 1.0f * (config->tileSizeCm / 100.0f),
				roomCenterPos.y + 1.0f * (config->tileSizeCm / 100.0f),
				roomCenterPos.z
			),
			*rot.idt(),
			rpg3D::T_Mob_Chicken_A_V1::TEMPLATE->nSpace/*entityTemplateNSpace*/, rpg3D::T_Mob_Chicken_A_V1::TEMPLATE->id/*entityTemplateId*/,
			msp<int>(rpg3D::TeamsIndexesList::MOB)/*team*/,
			impactPayloadsList,
			lBlock/*logicBlock*/, static_cast<rpg3D::ILB_Mob_ImpactChaser_A_V1*>(lBlock)/*logicBlockInterfaceRawPtr*/
		);
	}
	{
		rpg3D::LB_Mob_ImpactChaser_A_V1* lBlock = new rpg3D::LB_Mob_ImpactChaser_A_V1();

		roomContent->mobsSpawnList->append_emplace(
			*pos.set(
				roomCenterPos.x + 1.0f * (config->tileSizeCm / 100.0f),
				roomCenterPos.y + -1.0f * (config->tileSizeCm / 100.0f),
				roomCenterPos.z
			),
			*rot.idt(),
			rpg3D::T_Mob_ZombieChickent_A_V1::TEMPLATE->nSpace/*entityTemplateNSpace*/, rpg3D::T_Mob_ZombieChickent_A_V1::TEMPLATE->id/*entityTemplateId*/,
			msp<int>(rpg3D::TeamsIndexesList::MOB)/*team*/,
			impactPayloadsList,
			lBlock/*logicBlock*/, static_cast<rpg3D::ILB_Mob_ImpactChaser_A_V1*>(lBlock)/*logicBlockInterfaceRawPtr*/
		);
	}
	{
		rpg3D::LB_Mob_ImpactChaser_A_V1* lBlock = new rpg3D::LB_Mob_ImpactChaser_A_V1();

		roomContent->mobsSpawnList->append_emplace(
			*pos.set(
				roomCenterPos.x + -1.0f * (config->tileSizeCm / 100.0f),
				roomCenterPos.y + 1.0f * (config->tileSizeCm / 100.0f),
				roomCenterPos.z
			),
			*rot.idt(),
			rpg3D::T_Mob_Frog_A_V1::TEMPLATE->nSpace/*entityTemplateNSpace*/, rpg3D::T_Mob_Frog_A_V1::TEMPLATE->id/*entityTemplateId*/,
			msp<int>(rpg3D::TeamsIndexesList::MOB)/*team*/,
			impactPayloadsList,
			lBlock/*logicBlock*/, static_cast<rpg3D::ILB_Mob_ImpactChaser_A_V1*>(lBlock)/*logicBlockInterfaceRawPtr*/
		);
	}
}

void RoomGenerator::createConditionalZoneChangeTeleporter(
	Vector3& pos, float rotZDeg,
	EntityTemplate* teleporterPlatformTemplate, std::shared_ptr<std::string> instanceId,
	EntityTemplate* brokenTeleporterPlatformTemplate)
{
	Quaternion rot{ Vector3::Z, rotZDeg };

	{
		sp<worldGame3D::ConditionalWorldEntitySpawnConfig> entitySpawnInfo(new worldGame3D::ConditionalWorldEntitySpawnConfig(
			teleporterPlatformTemplate->nSpace, teleporterPlatformTemplate->id,
			pos, rot,
			instanceId
		));
		entitySpawnInfo->spawnTestFlags.setFlag(rpg3D::IOGPMobsSpawnSystem::FLAG_isZoneCleared/*name*/, true/*value*/, false/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

		roomContent->conditionalEntitiesList.appendEmplace(entitySpawnInfo);
	}

	{
		sp<worldGame3D::ConditionalWorldEntitySpawnConfig> entitySpawnInfo(new worldGame3D::ConditionalWorldEntitySpawnConfig(
			brokenTeleporterPlatformTemplate->nSpace, brokenTeleporterPlatformTemplate->id,
			pos, rot,
			nullptr/*instanceId*/
		));
		entitySpawnInfo->spawnTestFlags.setFlag(rpg3D::IOGPMobsSpawnSystem::FLAG_isZoneCleared/*name*/, false/*value*/, true/*alsoPassOnNoValue*/, false/*entryValueMustBeSet*/);

		roomContent->conditionalEntitiesList.appendEmplace(entitySpawnInfo);
	}
}

RoomGenerator::~RoomGenerator() {
	//void
}
