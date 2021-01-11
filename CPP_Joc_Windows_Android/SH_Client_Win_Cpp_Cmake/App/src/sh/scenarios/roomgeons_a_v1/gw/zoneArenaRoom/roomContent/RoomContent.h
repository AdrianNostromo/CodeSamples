#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>
#include <base/math/Vector3.h>
#include <memory>
#include <string>
#include <base/memory/SharedPointer.h>
#include "../../../gw/config/entity/EntityIds.h"
#include <base/map/Map1D.h>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;
class ExtraData;
namespace base {
	class IGameWorld;
};
namespace rpg3D {
	class EffectPayload;
	class ILogicBlock;
	class IOGPConditionalEntities;
	class IOGPMobsSpawnSystem;
};
namespace graphics {
	class TilingInformation;
};
namespace worldGame3D {
	class ConditionalWorldEntitySpawnConfig;
	class WorldEntitySpawnConfig;
};

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class RoomContent {pub dCtor(RoomContent);
	pub class EntityCreateInfo {pub dCtor(EntityCreateInfo);
		//The pos is in room_space, not zone_space.
		pub EntityTemplate* entityTemplate;
		pub Vector3 posInRoom;
		pub Quaternion rot;

		pub explicit EntityCreateInfo(
			EntityTemplate* entityTemplate,
			Vector3& posInRoom, Quaternion& rot)
			: entityTemplate(entityTemplate),
			posInRoom(posInRoom), rot(rot)
		{
			//void
		}

		pub virtual ~EntityCreateInfo() {};
	};
	pub class StructureElement : public EntityCreateInfo {priv typedef EntityCreateInfo super;pub dCtor(StructureElement);
		pub ArrayList<Visual3DModelMaterial*>* materialsList;
		pub std::string extraDataId_materialsList;

		pub std::shared_ptr<graphics::TilingInformation> tilingInformation;

		pub explicit StructureElement(
			EntityTemplate* entityTemplate,
			Vector3& posInRoom, Quaternion& rot,
			ArrayList<Visual3DModelMaterial*>* materialsList, std::string extraDataId_materialsList,
			std::shared_ptr<graphics::TilingInformation> tilingInformation)
			: super(
				entityTemplate,
				posInRoom, rot),
			materialsList(materialsList), extraDataId_materialsList(extraDataId_materialsList),
			tilingInformation(tilingInformation)
		{
			//void
		}
	};
	pub class Teleporter : public EntityCreateInfo {priv typedef EntityCreateInfo super;pub dCtor(Teleporter);
		pub std::shared_ptr<std::string> instanceId;
		pub std::shared_ptr<std::string> platformActivationScript;
				
		pub explicit Teleporter(
			EntityTemplate* entityTemplate,
			Vector3& posInRoom, Quaternion& rot, 
			std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript)
			:super(
				entityTemplate,
				posInRoom, rot),
			instanceId(instanceId), platformActivationScript(platformActivationScript)
		{}
	};
	pub class ConditionalEntity {pub dCtor(ConditionalEntity);
		pub sp<worldGame3D::ConditionalWorldEntitySpawnConfig> entitySpawnInfo;

		pub explicit ConditionalEntity(sp<worldGame3D::ConditionalWorldEntitySpawnConfig> entitySpawnInfo)
			: entitySpawnInfo(entitySpawnInfo)
		{
			//void
		}

		pub virtual ~ConditionalEntity() {}
	};
	pub class WorldItem {pub dCtor(WorldItem);
		pub Vector3 pos;
		pub Quaternion rot;

		pub EntityTemplate* entityTemplate;

		pub sp<float> worldItemGoldAmount;
		pub sp<float> itemHealAmount;
		pub sp<float> itemDamageAmount;

		pub explicit WorldItem(
			Vector3& pos, Quaternion& rot,
			EntityTemplate* entityTemplate,
			sp<float> worldItemGoldAmount, sp<float> itemHealAmount, sp<float> itemDamageAmount)
			: pos(pos), rot(rot),
			entityTemplate(entityTemplate),
			worldItemGoldAmount(worldItemGoldAmount), itemHealAmount(itemHealAmount), itemDamageAmount(itemDamageAmount)
		{
			//void
		}

		pub virtual ~WorldItem() {}
	};
	pub class ZoneEnteranceConfig {// Map1D requires a constructor from these (uggo). pub dCtor(ZoneEnteranceConfig);
		//EntityIds::ZoneEnterLocation
		pub int zoneEnterLocation;
		// This is optional because the teleporter is a optional entity.
		pub std::shared_ptr<std::string> platformInstanceId;
		// A separate platform pos is used because the teleporter is a optional entity.
		pub Vector3 platformPos;

		pub explicit ZoneEnteranceConfig(
			int zoneEnterLocation, std::shared_ptr<std::string> platformInstanceId, Vector3& platformPos)
			: zoneEnterLocation(zoneEnterLocation), platformInstanceId(platformInstanceId), platformPos(platformPos)
		{
			//void
		}

		pub virtual ~ZoneEnteranceConfig() {}
	};

	pub int tileSizeCm;

	// Use a separate list for floor elements for the preview mechanism where the floor is visible only.
	pub ListDL<StructureElement> structureElementsList{};
	pub ListDL<Teleporter> teleportersList{};
	// This is currently used by teleporters so they are activated when all mobs are gone.
	pub ListDL<ConditionalEntity> conditionalEntitiesList{};
	pub ListDL<WorldItem> worldItemsList{};

	pub std::shared_ptr<ArrayList<worldGame3D::WorldEntitySpawnConfig>> mobsSpawnList = nullptr;

	pub explicit RoomContent(int tileSizeCm);

	pub void createWorldContent(base::IGameWorld* gw, rpg3D::IOGPConditionalEntities* gpConditionalEntities, rpg3D::IOGPMobsSpawnSystem* gpMobsSpawnSystem);

	priv IWorldEntity* createConfigEntity(
		Vector3& pos, Quaternion& rot,
		EntityTemplate* entityTemplate,
		std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript,
		base::IGameWorld* gw,
		const int* teamId,
		sp<float> worldItemGoldAmount, sp<float> itemHealAmount, sp<float> itemDamageAmount, float* projectileDamage = nullptr,
		rpg3D::ILogicBlock* logicBlock = nullptr, void* logicBlockInterfaceRawPtr = nullptr,
		ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* impactPayloadsList = nullptr);

	pub virtual ~RoomContent();

};
};
};
