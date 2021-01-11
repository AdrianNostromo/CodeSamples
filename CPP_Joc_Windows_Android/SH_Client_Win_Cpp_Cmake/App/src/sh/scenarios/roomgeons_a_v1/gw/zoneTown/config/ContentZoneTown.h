#pragma once

#include <base/gh.h>
#include "../../zoneCommon/config/ContentZoneCommon.h"
#include <base/list/Array3D.h>
#include <base/list/Array2D.h>
#include <base/list/ArrayList.h>
#include <memory>
#include <base/list/ListDL.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

class Visual3DModelMaterial;
class EntityTemplate;
class IWorldEntity;
namespace graphics {
	class TilingInformation;
};
namespace rpg3D {
	class ILogicBlock;
	class EffectPayload;
	class IOGPConditionalEntities;
};

namespace roomgeons_a_v1 {
namespace Town {
class ContentZoneTown : public common::ContentZoneCommon {priv typedef common::ContentZoneCommon super;pub dCtor(ContentZoneTown);
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

	pub static std::string NAME;
	pub static std::shared_ptr<worldGame3D::ContentZone> FactoryFunction();
	
	priv int tileSizeCm = 60;

	priv std::shared_ptr<Array2D<signed char>> floorTilingGrid = nullptr;
	priv std::shared_ptr<Array2D<signed char>> wallsTilingGrid = nullptr;

	priv ListDL<StructureElement> structureElementsList{};

    pub explicit ContentZoneTown();
	
	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

	priv void createConfigEntity(
		Vector3& pos, Quaternion& rot,
		EntityTemplate* entityTemplate,
		std::shared_ptr<std::string> instanceId, std::shared_ptr<std::string> platformActivationScript,
		base::IGameWorld* gw);

	priv void populateFloorsEntities();
	priv void populateWallsEntities();

    pub ~ContentZoneTown() override;
};
};
};
