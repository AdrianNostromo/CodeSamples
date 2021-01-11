#include "GPER_RoomConfigs.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/util/ContentGroup.h>
#include <base/visual3d/Visual3DUtil.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/config/logicBlock/mob/LB_Mob_ImpactChaser_A_V1.h>
#include <worldGame3D/gw/main/IGamePartMain.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::PrefabRoomsDungeonA;

GPER_RoomConfigs::GPER_RoomConfigs(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPER_RoomConfigs::initLevelZoneRooms(std::shared_ptr<DungeonContent::LevelZone> levelZoneConfig) {
	/*asd_g_t;
	if (levelZoneConfig->levelZoneRoomIndexesGrid == nullptr) {
		throw LogicException(LOC);
	}*/

	this->levelZoneConfig = levelZoneConfig;
	//asd_g_t; this->levelZoneRoomIndexesGrid = levelZoneConfig->levelZoneRoomIndexesGrid;

	for (int roomIndex = 0; roomIndex < levelZoneConfig->roomsList.count(); roomIndex++) {
		std::shared_ptr<DungeonContent::LevelZone::Room>& roomConfig = levelZoneConfig->roomsList.getReference(roomIndex);

		Room* room = roomsList.append_emplace(roomIndex, roomConfig);

		// Handle doors.
		for (int i = 0; i < roomConfig->doorsList.count(); i++) {
			DungeonContent::LevelZone::Room::Door* doorConfig = roomConfig->doorsList.getPointer(i);

			room->doorsList.append_emplace(doorConfig);
		}
		room->doorsList.lock();
	}

	// Init the doors other_side room pointers.
	for (int roomIndex = 0; roomIndex < roomsList.count(); roomIndex++) {
		Room& room = roomsList.getReference(roomIndex);

		for (int i = 0; i < room.doorsList.count(); i++) {
			Room::Door* door = room.doorsList.getPointer(i);

			door->otherSideRoom = roomsList.getPointer(door->config->otherSideRoomIndex);
		}
	}

	// This will make sure entries are not modified after init.
	roomsList.lock();

	// Create all dungeon level content.
	for (int roomIndex = 0; roomIndex < roomsList.count(); roomIndex++) {
		Room& room = roomsList.getReference(roomIndex);

		createRoomContent(room, false);
	}
}

void GPER_RoomConfigs::createRoomContent(Room& room, bool isContentEnabled) {
	if (room.isContentCreated) {
		throw LogicException(LOC);
	}

	room.isContentCreated = true;
	if (isContentEnabled) {
		room.isContentEnabled = true;
	}

	// Create room content.
	if (room.contentGroupsList.count() == 0) {
		room.contentGroupsList.appendDirect(std::make_shared<worldGame3D::ContentGroup>());
	}

	for (int i = 0; i < room.roomConfig->structureElementsList.count(); i++) {
		DungeonContent::LevelZone::Room::StructureElement* structureElement = room.roomConfig->structureElementsList.getPointer(i);

		Vector3 pos{ room.roomConfig->zoneSpacePos };
		pos.add(structureElement->pos);

		createEnvironmentEntity(
			pos,
			structureElement->rotZDeg,
			EntityTemplate::fromId(structureElement->templateNamespace, structureElement->templateId),
			nullptr,
			gw, room, isContentEnabled,
			structureElement->materialsList, structureElement->extraDataId_materialsList,
			structureElement->tilingInformation
		);
	}

	for (int i = 0; i < room.roomConfig->doorsList.count(); i++) {
		DungeonContent::LevelZone::Room::Door* structureElement = room.roomConfig->doorsList.getPointer(i);

		Vector3 pos{ room.roomConfig->zoneSpacePos };
		pos.add(structureElement->pos);

		createEnvironmentEntity(
			pos,
			structureElement->rotZDeg,
			EntityTemplate::fromId(structureElement->templateNamespace, structureElement->templateId),
			nullptr,
			gw, room, isContentEnabled,
			structureElement->materialsList, structureElement->extraDataId_materialsList,
			structureElement->tilingInformation
		);
	}

	for (int i = 0; i < room.roomConfig->teleportersList.count(); i++) {
		DungeonContent::LevelZone::Room::Teleporter* teleporter = room.roomConfig->teleportersList.getPointer(i);

		Vector3 pos{ room.roomConfig->zoneSpacePos };
		pos.add(teleporter->pos);

		IWorldEntity* entity = createEnvironmentEntity(
			pos,
			teleporter->rotZDeg,
			EntityTemplate::fromId(teleporter->templateNamespace, teleporter->templateId),
			teleporter->instanceId,
			gw, room, isContentEnabled,
			nullptr/*materialsList*/, ""/*extraDataId_materialsList*/,
			nullptr/*tilingInformation*/
		);
		entity->setExtraDataB("teleportTargetZoneLevelIndex", base::IWrappedValue::new_int(teleporter->teleportTargetLevelZoneIndex));
		entity->setExtraDataB("teleportTargetEntityInstanceID", base::IWrappedValue::new_String(teleporter->teleportTargetEntityInstanceID));
	}

	for (int i = 0; i < room.roomConfig->mobsList.count(); i++) {
		DungeonContent::LevelZone::Room::Mob* mob = room.roomConfig->mobsList.getPointer(i);

		Vector3 pos{ room.roomConfig->zoneSpacePos };
		pos.add(mob->pos);

		createMobEntity(
			pos,
			mob->rotZDeg,
			EntityTemplate::fromId(mob->templateNamespace, mob->templateId),
			&mob->impactPayloadsList,
			gw, room, isContentEnabled
		);
	}
}

IWorldEntity* GPER_RoomConfigs::createEnvironmentEntity(
	Vector3& pos,
	float rotZ,
	EntityTemplate* entityTemplate,
	std::shared_ptr<std::string> instanceId,
	base::IGameWorld* gw, Room& room, bool isContentEnabled,
	ArrayList<Visual3DModelMaterial*>* materialsList, std::string extraDataId_materialsList,
	std::shared_ptr<graphics::TilingInformation> tilingInformation)
{
	std::shared_ptr<ExtraData> extraData = nullptr;

	if (tilingInformation != nullptr) {
		extraData = std::make_shared<ExtraData>();
		if (materialsList == nullptr) {
			throw LogicException(LOC);
		}
		extraData->setData(extraDataId_materialsList, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(materialsList, -1));
		extraData->setData(EntityTemplate::ExtraDataID_tilingInformation, base::IWrappedValue::new_CustomB<std::shared_ptr<graphics::TilingInformation>>(tilingInformation, "graphics::TilingInformation"));
	}

	Quaternion rot{ Vector3::Z, rotZ };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		entityTemplate,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		instanceId,
		0,
		extraData,
		true/*isZoneOwnedEntity*/, &room.contentGroupsList/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		nullptr, isContentEnabled
	);

	return entity;
}

IWorldEntity* GPER_RoomConfigs::createMobEntity(
	Vector3& pos,
	float rotZ,
	EntityTemplate* entityTemplate,
	ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* impactPayloadsList,
	base::IGameWorld* gw, Room& room, bool isContentEnabled)
{
	Quaternion rot{ Vector3::Z, rotZ };
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		entityTemplate,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr/*instanceId*/,
		0,
		nullptr/*extraData*/,
		true/*isZoneOwnedEntity*/, &room.contentGroupsList/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[impactPayloadsList](IWorldEntity* entity) {
			rpg3D::ITeamModule* mob_team = entity->getComponentAs<rpg3D::ITeamModule*>(true/*mustExist*/);
			mob_team->setTeamIndex(rpg3D::TeamsIndexesList::MOB);

			entity->getComponentAs<rpg3D::IProjectileModule*>(true/*mustExist*/)->addPayloadsList(
				impactPayloadsList
			);

			rpg3D::IControllerModule* entity_controller = entity->getComponentAs<rpg3D::IControllerModule*>(true/*mustExist*/);

			rpg3D::LB_Mob_ImpactChaser_A_V1* lB_Mob_ImpactChaser_A_V1 = new rpg3D::LB_Mob_ImpactChaser_A_V1();
			entity_controller->registerLogicBlock(lB_Mob_ImpactChaser_A_V1, rpg3D::ILB_Mob_ImpactChaser_A_V1::ID, static_cast<rpg3D::ILB_Mob_ImpactChaser_A_V1*>(lB_Mob_ImpactChaser_A_V1));
			entity_controller->setSingleActiveGroupLogicBlock(rpg3D::LB_Mob_ImpactChaser_A_V1::GroupID, rpg3D::LB_Mob_ImpactChaser_A_V1::ID);
		}, isContentEnabled
	);

	return entity;
}

GPER_RoomConfigs::~GPER_RoomConfigs() {
	//void
}
