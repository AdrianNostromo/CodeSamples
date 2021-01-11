#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <memory>
#include <string>
#include <base/list/ArrayList.h>
#include <base/sp.h>

class EntityTemplate;
class Visual3DModelMaterial;
class IWorldEntity;
namespace base {
	class IGameWorld;
};
namespace graphics {
	class TilingInformation;
};

namespace rpg3D {
class ZoneContentData {pub dCtor(ZoneContentData);
	pub class EntityCreateInfo_Base {pub dCtor(EntityCreateInfo_Base);
		//The pos is in room_space, not zone_space.
		pub EntityTemplate* entityTemplate;
		pub Vector3 posInRoom;
		pub Quaternion rot;

		pub std::shared_ptr<std::string> instanceId;

		pub explicit EntityCreateInfo_Base(
			EntityTemplate* entityTemplate,
			Vector3& posInRoom, Quaternion& rot,
			std::shared_ptr<std::string> instanceId)
			: entityTemplate(entityTemplate),
			posInRoom(posInRoom), rot(rot),
			instanceId(instanceId)
		{
			//void
		}

		pub virtual ~EntityCreateInfo_Base() {};
	};
	pub class EntitiesCreateInfo : public EntityCreateInfo_Base {priv typedef EntityCreateInfo_Base super; pub dCtor(EntitiesCreateInfo);
		pub ArrayList<Visual3DModelMaterial*>* materialsList;
		pub std::string extraDataId_materialsList;

		pub std::shared_ptr<graphics::TilingInformation> tilingInformation;

		pub std::shared_ptr<std::string> platformActivationScript;

		pub explicit EntitiesCreateInfo(
			EntityTemplate* entityTemplate,
			Vector3& posInRoom, Quaternion& rot,
			std::shared_ptr<std::string> instanceId,
			ArrayList<Visual3DModelMaterial*>* materialsList, std::string extraDataId_materialsList,
			std::shared_ptr<graphics::TilingInformation> tilingInformation,
			std::shared_ptr<std::string> platformActivationScript)
			: super(
				entityTemplate,
				posInRoom, rot,
				instanceId),
			materialsList(materialsList), extraDataId_materialsList(extraDataId_materialsList),
			tilingInformation(tilingInformation),
			platformActivationScript(platformActivationScript)
		{
			//void
		}
	};

	pub enum GroundType { Grass_1Layer, GrassDirt_2Layers };

	pub int tileCellSizeCm = 60;
	pub int zoneEditorAssetInstanceIdCounter = 0;

    pub ListDL<EntitiesCreateInfo> entitiesInfosList{};

    pub explicit ZoneContentData();

	pub void createContent(base::IGameWorld* gw);

	//asd_01;// call these;
	pub static std::shared_ptr<std::stringstream> Serialise(base::IGameWorld* gw);
	pub static sp<ZoneContentData> Parse(std::string& linesString);

	// Use this to get a basic zone that can be edited.
	/// Add extra platforms manually to the returned object.
	pub static sp<ZoneContentData> GetNewBasicZone(
		GroundType groundType, bool createTallUnderground,
		bool createWallsTiledXY,
		bool createStarterAreaTeleporter
	);

    pub virtual ~ZoneContentData();
};
};
