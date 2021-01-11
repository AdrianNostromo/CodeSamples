#include "RoomContent.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/conditionalEntities/IOGPConditionalEntities.h>
#include <rpg3D/gw/entity/module/TeamModule.h>
#include <rpg3D/gw/entity/module/worldItem/IWorldItemModule.h>
#include <rpg3D/gw/entity/module/projectile/ProjectileModule.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <worldGame3D/gw/entity/util/WorldEntitySpawnConfig.h>
#include <worldGame3D/gw/entity/util/ConditionalWorldEntitySpawnConfig.h>
#include <rpg3D/gw/entity/module/controller/IControllerModule.h>
#include <rpg3D/gw/mobsSpawnSystem/OGPMobsSpawnSystem.h>
#include <rpg3D/gw/config/team/TeamsIndexesList.h>
#include <rpg3D/gw/projectile/EffectPayload.h>

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

RoomContent::RoomContent(int tileSizeCm)
	: tileSizeCm(tileSizeCm)
{
	//void
}

void RoomContent::createWorldContent(base::IGameWorld* gw, rpg3D::IOGPConditionalEntities* gpConditionalEntities, rpg3D::IOGPMobsSpawnSystem* gpMobsSpawnSystem) {
	for (ListDL<StructureElement>::Entry* entry = structureElementsList.getFirst(); entry != nullptr; entry = entry->next) {
		StructureElement& entityCreateInfo = entry->data;

		std::shared_ptr<ExtraData> extraData = nullptr;

		if (entityCreateInfo.materialsList != nullptr) {
			if (extraData == nullptr) {
				extraData = std::make_shared<ExtraData>();
			}

			extraData->setData(entityCreateInfo.extraDataId_materialsList, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(entityCreateInfo.materialsList, -1));
		}

		if (entityCreateInfo.tilingInformation != nullptr) {
			if (extraData == nullptr) {
				extraData = std::make_shared<ExtraData>();
			}
			
			extraData->setData(EntityTemplate::ExtraDataID_tilingInformation, base::IWrappedValue::new_CustomB<std::shared_ptr<graphics::TilingInformation>>(entityCreateInfo.tilingInformation, "graphics::TilingInformation"));
		}

		IWorldEntity* entity = gw->getGPMain()->createEntity(
			entityCreateInfo.entityTemplate,
			entityCreateInfo.posInRoom,
			&entityCreateInfo.rot/*rot*/, nullptr/*cardinalRotation*/,
			nullptr,
			0,
			extraData/*extraData*/,
			true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
			nullptr
		);
	}

	for (ListDL<Teleporter>::Entry* entry = teleportersList.getFirst(); entry != nullptr; entry = entry->next) {
		Teleporter& teleporterCreateInfo = entry->data;

		IWorldEntity* homeTeleporterPlatform = createConfigEntity(
			teleporterCreateInfo.posInRoom, teleporterCreateInfo.rot,
			teleporterCreateInfo.entityTemplate,
			teleporterCreateInfo.instanceId/*instanceId*/,
			teleporterCreateInfo.platformActivationScript/*platformActivationScript*/, gw,
			nullptr/*teamId*/, nullptr/*worldItemGoldAmount*/, nullptr/*itemHealAmount*/, nullptr/*itemDamageAmount*/);
	}

	for (ListDL<ConditionalEntity>::Entry* entry = conditionalEntitiesList.getFirst(); entry != nullptr; entry = entry->next) {
		ConditionalEntity& conditionalEntity = entry->data;

		//asd_01;// create instantly if condition passed;
		gpConditionalEntities->addConditionalEntitySpawnInfo(conditionalEntity.entitySpawnInfo);
	}

	for (ListDL<WorldItem>::Entry* entry = worldItemsList.getFirst(); entry != nullptr; entry = entry->next) {
		WorldItem& conditionalEntity = entry->data;

		createConfigEntity(
			conditionalEntity.pos, conditionalEntity.rot,
			conditionalEntity.entityTemplate, nullptr/*instanceId*/, nullptr/*platformActivationScript*/, gw,
			nullptr/*teamId*/, conditionalEntity.worldItemGoldAmount/*worldItemGoldAmount*/, conditionalEntity.itemHealAmount/*itemHealAmount*/, conditionalEntity.itemDamageAmount/*itemDamageAmount*/
		);
	}

	if (mobsSpawnList != nullptr) {
		//rpg3D::IOGPMobsSpawnSystem* gpMobsSpawnSystem = gw->getComponentAs<rpg3D::IOGPMobsSpawnSystem*>(true/*mustExist*/);
		gpMobsSpawnSystem->initMobsSpawnInformation(rpg3D::TeamsIndexesList::MOB, mobsSpawnList);
	}
}

IWorldEntity* RoomContent::createConfigEntity(
	Vector3& pos, Quaternion& rot,
	EntityTemplate* entityTemplate,
	std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript,
	base::IGameWorld* gw,
	const int* teamId,
	sp<float> worldItemGoldAmount, sp<float> itemHealAmount, sp<float> itemDamageAmount, float* projectileDamage,
	rpg3D::ILogicBlock* logicBlock, void* logicBlockInterfaceRawPtr,
	ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* impactPayloadsList)
{
	IWorldEntity* entity = gw->getGPMain()->createEntity(
		entityTemplate,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		instanceId,
		0,
		nullptr,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		[teamId, worldItemGoldAmount, itemHealAmount, itemDamageAmount, projectileDamage, logicBlock, logicBlockInterfaceRawPtr, impactPayloadsList, platformActivationScript](IWorldEntity* entity) -> void {
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
					rpg3D::EffectPayload::Types::giveGoldAmount, rpg3D::EffectPayload::Target::OTHER_ENTITY, *worldItemGoldAmount.get()
				));
			}
			if (itemHealAmount != nullptr) {
				rpg3D::IWorldItemModule* entity_worldItem = entity->getComponentAs<rpg3D::IWorldItemModule*>(true/*mustExist*/);
				entity_worldItem->addPayload(std::make_shared<rpg3D::EffectPayload>(
					rpg3D::EffectPayload::Types::fixedValueHeal, rpg3D::EffectPayload::Target::OTHER_ENTITY, *itemHealAmount.get()
				));
			}
			if (itemDamageAmount != nullptr) {
				rpg3D::IWorldItemModule* entity_worldItem = entity->getComponentAs<rpg3D::IWorldItemModule*>(true/*mustExist*/);
				entity_worldItem->addPayload(std::make_shared<rpg3D::EffectPayload>(
					rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, *itemDamageAmount.get()
				));
			}
			if (projectileDamage != nullptr) {
				entity->getComponentAs<rpg3D::IProjectileModule*>(true/*mustExist*/)->addPayload(
					std::make_shared<rpg3D::EffectPayload>(rpg3D::EffectPayload::Types::physicalDamage, rpg3D::EffectPayload::Target::OTHER_ENTITY, *projectileDamage)
				);
			}

			if (impactPayloadsList != nullptr) {
				entity->getComponentAs<rpg3D::IProjectileModule*>(true/*mustExist*/)->addPayloadsList(
					impactPayloadsList
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

RoomContent::~RoomContent() {
	//void
}
