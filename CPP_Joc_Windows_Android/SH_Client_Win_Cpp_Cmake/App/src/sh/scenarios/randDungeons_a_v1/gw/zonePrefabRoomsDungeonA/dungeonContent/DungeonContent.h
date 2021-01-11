#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/math/Vector3.h>
#include <string>
#include <base/list/Array2D.h>

class Visual3DModelMaterial;
namespace rpg3D {
	class EffectPayload;
};
namespace graphics {
	class TilingInformation;
};

namespace randDungeons_a_v1 {
namespace PrefabRoomsDungeonA {
class DungeonContent {pub dCtor(DungeonContent);
	pub class LevelZone {pub dCtor(LevelZone);
		pub class Room {pub dCtor(Room);
			pub class StructureElement {pub dCtor(StructureElement);
				//The pos is in room_space, not zone_space.
				pub Vector3 pos;
				pub float rotZDeg;
				pub std::string templateNamespace;
				pub std::string templateId;

				pub ArrayList<Visual3DModelMaterial*>* materialsList = nullptr;
				pub std::string extraDataId_materialsList = "";

				pub std::shared_ptr<graphics::TilingInformation> tilingInformation;

				pub explicit StructureElement(
					Vector3& pos, float rotZDeg, 
					std::string& templateNamespace, std::string& templateId,
					ArrayList<Visual3DModelMaterial*>* materialsList, std::string extraDataId_materialsList,
					std::shared_ptr<graphics::TilingInformation> tilingInformation)
					: pos(pos), rotZDeg(rotZDeg), templateNamespace(templateNamespace), templateId(templateId),
					materialsList(materialsList), extraDataId_materialsList(extraDataId_materialsList),
					tilingInformation(tilingInformation)
				{
					//void
				}

				pub virtual ~StructureElement() {};
			};
			pub class Door : public StructureElement {priv typedef StructureElement super;pub dCtor(Door);
				pub int otherSideRoomIndex;
				
				pub explicit Door(Vector3& pos, float rotZDeg, std::string& templateNamespace, std::string& templateId, int otherSideRoomIndex)
					:super(
						pos, rotZDeg, templateNamespace, templateId, 
						nullptr/*materialsList*/, ""/*extraDataId_materialsList*/,
						nullptr/*tilingInformation*/), otherSideRoomIndex(otherSideRoomIndex)
				{}
			};
			pub class Teleporter {pub dCtor(Teleporter);
				pub Vector3 pos;
				pub float rotZDeg;
				pub std::string templateNamespace;
				pub std::string templateId;

				pub std::shared_ptr<std::string> instanceId;

				// -1; Exit the dungeon and go to the main zone.
				pub int teleportTargetLevelZoneIndex;
				pub std::string teleportTargetEntityInstanceID;

				pub explicit Teleporter(
					Vector3& pos, float rotZDeg, std::string& templateNamespace, std::string& templateId,
					std::shared_ptr<std::string>& instanceId,
					int teleportTargetLevelZoneIndex, std::string teleportTargetEntityInstanceID)
					: pos(pos), rotZDeg(rotZDeg), templateNamespace(templateNamespace), templateId(templateId),
					instanceId(instanceId),
					teleportTargetLevelZoneIndex(teleportTargetLevelZoneIndex), teleportTargetEntityInstanceID(teleportTargetEntityInstanceID)
				{
					//void
				}

				pub virtual ~Teleporter() {}
			};
			pub class Mob {pub dCtor(Mob);
				pub Vector3 pos;
				pub float rotZDeg;
				pub std::string templateNamespace;
				pub std::string templateId;

				ArrayList<std::shared_ptr<rpg3D::EffectPayload>> impactPayloadsList{};

				pub explicit Mob(
					Vector3& pos, float rotZDeg, std::string& templateNamespace, std::string& templateId)
					: pos(pos), rotZDeg(rotZDeg), templateNamespace(templateNamespace), templateId(templateId)
				{
					//void
				}

				pub virtual ~Mob() {}
			};

			pub Vector3 zoneSpacePos;

			// Use a separate list for floor elements for the preview mechanism where the floor is visible only.
			pub ArrayList<StructureElement> structureElementsList{};
			pub ArrayList<Door> doorsList{};
			pub ArrayList<Teleporter> teleportersList{};
			pub ArrayList<Mob> mobsList{};
			

			pub explicit Room(Vector3& zoneSpacePos)
				: zoneSpacePos(zoneSpacePos)
			{
				//void
			}
			
			pub virtual ~Room() {}
		};

		pub ArrayList<std::shared_ptr<Room>> roomsList{};

		pub explicit LevelZone() {
			//void
		}

		pub virtual ~LevelZone() {};
	};

	pub ArrayList<std::shared_ptr<LevelZone>> levelZonesList{};

	pub int seed = -1;
	pub int entryPointLevelZonIndex = -1;
	pub std::shared_ptr<std::string> entryPointPosEntityInstanceId = nullptr;

	pub explicit DungeonContent() {
		//void
	}

	pub virtual ~DungeonContent() {}
};
};
};
