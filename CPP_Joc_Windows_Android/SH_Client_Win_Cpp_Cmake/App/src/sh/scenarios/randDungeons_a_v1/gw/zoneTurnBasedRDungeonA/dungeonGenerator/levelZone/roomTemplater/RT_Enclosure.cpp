#include "RT_Enclosure.h"
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_LinearX.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

RT_Enclosure::RT_Enclosure()
	: super()
{
	//void
}

void RT_Enclosure::bakeEnclosure() {
	float gCellSizeM = config->tileSize_cm / 100.0f;

	// Create a wall or door for each tile len for now.
	for (int i = 0; i < in_roomElementsPopulatorResult->finalSideElementOrigins.count(); i++) {
		Result_RoomElementsPopulator::CombinedSideElementInfo& combinedSideElementInfo = in_roomElementsPopulatorResult->finalSideElementOrigins.getReference(i);

		Vector2Int elementRoomGPos{ combinedSideElementInfo.tilesGridPos };
		float rotZDeg = 0;
		if (combinedSideElementInfo.sideIndex == Result_RoomElementsPopulator::RoomGCellSides::xNegative) {
			// Left wall is top anchored and expands to the bottom.
			elementRoomGPos.add(
				0,
				1
			);
			rotZDeg = -90.0f;
		} else if (combinedSideElementInfo.sideIndex == Result_RoomElementsPopulator::RoomGCellSides::xPositive) {
			// Right wall is bottom anchored and expands to the top.
			elementRoomGPos.add(
				0 + 1,
				0
			);
			rotZDeg = 90.0f;
		} else if (combinedSideElementInfo.sideIndex == Result_RoomElementsPopulator::RoomGCellSides::yNegative) {
			// Bottom wall is left anchored and expands to the right.
			elementRoomGPos.add(
				0,
				0
			);
			rotZDeg = 0.0f;
		} else if (combinedSideElementInfo.sideIndex == Result_RoomElementsPopulator::RoomGCellSides::yPositive) {
			// Top wall is right anchored and expands to the left.
			elementRoomGPos.add(
				1,
				0 + 1
			);
			rotZDeg = 180.0f;
		} else {
			throw LogicException(LOC);
		}
		Vector3 elementPos{
			elementRoomGPos.x * gCellSizeM,
			elementRoomGPos.y * gCellSizeM,
			0.0f * gCellSizeM
		};

		if (combinedSideElementInfo.type == Result_RoomElementsPopulator::SideElementType::Door) {
			if (combinedSideElementInfo.tilesLen != 1 && combinedSideElementInfo.tilesLen != 2) {
				// Doors must be 1x or 2x tiles long currently.
				throw LogicException(LOC);
			}

			EntityTemplate* t = config->templateFactory_door(Math::cm_to_dm_exact(config->tileSize_cm * combinedSideElementInfo.tilesLen));

			result->roomContent->doorsList.append_emplace(
				elementPos, rotZDeg,
				t->nSpace, t->id,
				combinedSideElementInfo.doorOtherSideRoomIndex
			);
		} else if (combinedSideElementInfo.type == Result_RoomElementsPopulator::SideElementType::Wall) {
			std::shared_ptr<graphics::TilingInformation_LinearX> tilingInformation = std::make_shared<graphics::TilingInformation_LinearX>(
				config->wall_tileVoxelsCount,
				-1/*enabledSidesTriangulationBitMask*/,
				config->wall_tileConfig,
				combinedSideElementInfo.tilesLen
			);

			result->roomContent->structureElementsList.append_emplace(
				elementPos, rotZDeg,
				config->wall_tileableTemplate_linearX->nSpace, config->wall_tileableTemplate_linearX->id,
				config->wall_materialsList, config->wall_extraDataId_materialsList,
				tilingInformation
			);

			// Currently a tile is drawn for each wall part. The below offset handles this.
			if (combinedSideElementInfo.sideIndex == Result_RoomElementsPopulator::RoomGCellSides::xNegative) {
				// Left wall is top anchored and expands to the bottom.
				elementPos.y -= gCellSizeM;
			} else if (combinedSideElementInfo.sideIndex == Result_RoomElementsPopulator::RoomGCellSides::xPositive) {
				// Right wall is bottom anchored and expands to the top.
				elementPos.y += gCellSizeM;
			} else if (combinedSideElementInfo.sideIndex == Result_RoomElementsPopulator::RoomGCellSides::yNegative) {
				// Bottom wall is left anchored and expands to the right.
				elementPos.x += gCellSizeM;
			} else if (combinedSideElementInfo.sideIndex == Result_RoomElementsPopulator::RoomGCellSides::yPositive) {
				// Top wall is right anchored and expands to the left.
				elementPos.x -= gCellSizeM;
			} else {
				throw LogicException(LOC);
			}
		} else {
			throw LogicException(LOC);
		}
	}
}

RT_Enclosure::~RT_Enclosure() {
	//void
}
