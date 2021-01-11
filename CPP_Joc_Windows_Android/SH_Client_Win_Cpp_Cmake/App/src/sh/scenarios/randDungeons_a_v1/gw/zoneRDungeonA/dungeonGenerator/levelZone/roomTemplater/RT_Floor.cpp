#include "RT_Floor.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

RT_Floor::RT_Floor()
	: super()
{
	//void
}

void RT_Floor::bakeFloor() {
	std::shared_ptr<Array2D<signed char>> tilingGrid = std::make_shared<Array2D<signed char>>(in_roomElementsPopulatorResult->floorIndexedTilingGrid.getLengthA(), in_roomElementsPopulatorResult->floorIndexedTilingGrid.getLengthB());
	for (int xx = 0; xx < in_roomElementsPopulatorResult->floorIndexedTilingGrid.getLengthA(); xx++) {
		for (int yy = 0; yy < in_roomElementsPopulatorResult->floorIndexedTilingGrid.getLengthB(); yy++) {
			char floorType = in_roomElementsPopulatorResult->floorIndexedTilingGrid.getDirect(xx, yy);

			if (floorType < 0) {
				// No floor;
				tilingGrid->setDirect(xx, yy, -1);
			} else if (floorType == 0) {
				// General floor type.
				if (((xx % 2 == 0) && (yy % 2 == 0)) || ((xx % 2 == 1) && (yy % 2 == 1))) {
					tilingGrid->setDirect(xx, yy, 0);
				} else {
					tilingGrid->setDirect(xx, yy, 1);
				}
			} else {
				// Unhandled floor type.
				throw LogicException(LOC);
			}
		}
	}

	std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
		config->floor_tileVoxelsCount,
		-1/*enabledSidesTriangulationBitMask*/,
		config->floor_tilesConfigList,
		tilingGrid,
		nullptr/*gridGeometryOffset*/
	);

	float gCellSizeM = config->tileSize_cm / 100.0f;
	Vector3 elementPos{ 0.0f, 0.0f, 0.0f };

	result->roomContent->structureElementsList.append_emplace(
		elementPos, 0.0f,
		config->floor_tileableTemplate_gridXY->nSpace, config->floor_tileableTemplate_gridXY->id,
		config->floor_materialsList, config->floor_extraDataId_materialsList,
		tilingInformation/*tilingInformation*/
	);
}

RT_Floor::~RT_Floor() {
	//void
}
