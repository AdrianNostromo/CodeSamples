#include "GPG2DC_PathableGCellsIndicator.h"
#include <rpg3D/gw/entity/module/grid2DContent/IGrid2DContentModule.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/list/Array2D.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <rpg3D/gw/config/entity/structure/T_GridPathingIndicatorFloor_A_TiledX6dmY6dm.h>
#include <worldGame3D/gw/entity/util/WrappedWorldEntity.h>

using namespace rpg3D;

GPG2DC_PathableGCellsIndicator::GPG2DC_PathableGCellsIndicator(
    base::IGameWorld* gw, 
    Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea)
	: super(gw, gCellSize, allowMultipleEntitiesPerGCell, allowEntitiesOutsideOfTheGridArea)
{
	//void
}

void GPG2DC_PathableGCellsIndicator::hidePathableGCellsIndicator() {
	if(indicatorWEntity != nullptr) {
		if (indicatorWEntity->entity != nullptr && !indicatorWEntity->entity->getIsMarkedForRemoval()) {
			indicatorWEntity->entity->markEntityForRemoval(IWorldEntity::ENT_REM_REASON_PURPOSE_FINISHED, false);
		}

		indicatorWEntity = nullptr;
	}
}

void GPG2DC_PathableGCellsIndicator::showPathableGCellsIndicator(Vector2Int& startGPos, int drawRadius, bool showStartGPosAsValidDestination) {
	if (indicatorWEntity != nullptr) {
		hidePathableGCellsIndicator();
	}

	Bounds2DInt drawRegion{};
	if (drawRadius < 0) {
		drawRegion.set(getGridTrackingRegionBounds());
	}else {
		drawRegion.set(
			startGPos.x - drawRadius/*xMin*/, startGPos.x + drawRadius + 1/*xMax*/,
			startGPos.y - drawRadius/*yMin*/, startGPos.y + drawRadius + 1/*yMax*/
		);
		
		drawRegion.intersect(getGridTrackingRegionBounds());
	}
	
	std::shared_ptr<Array2D<signed char>> tilingGrid = std::make_shared<Array2D<signed char>>(
		drawRegion.getLenX(), drawRegion.getLenY()
	);

	int eId = 0;
	signed char NOT_DRAWN = -1;// Nothing
	signed char START_GCELL = eId++;// Self tile
	signed char MOVE_DEST_CANDIDATE = eId++;// Move tile

	// Set this here because the above eId++ must always occur.
	if (showStartGPosAsValidDestination) {
		START_GCELL = MOVE_DEST_CANDIDATE;// Nothing;
	}

	for (int gPosX = drawRegion.min.x; gPosX < drawRegion.max.x; gPosX++) {
		for (int gPosY = drawRegion.min.y; gPosY < drawRegion.max.y; gPosY++) {
			signed char tyleType;

			// If custom grid cells mode is used, not all gCells enumerated here are inside the actual grid.
			if (startGPos.equals(gPosX, gPosY)) {
				// Occupied (self);

				tyleType = START_GCELL;
			} else if(getIsGCellOccupied(gPosX, gPosY)) {
				// Occupied (other);

				tyleType = NOT_DRAWN;
			} else {
				// Empty.

				if (isGPosInTrackingRegion(gPosX, gPosY)) {
					tyleType = MOVE_DEST_CANDIDATE;
				} else {
					tyleType = NOT_DRAWN;
				}
			}

			tilingGrid->setDirect(gPosX - drawRegion.min.x, gPosY - drawRegion.min.y, tyleType);
		}
	}

	std::shared_ptr<ArrayList<graphics::TilingInformation::TileConfig>> tilesConfigList = std::make_shared<ArrayList<graphics::TilingInformation::TileConfig>>();
	tilesConfigList->append_emplace(rpg3D::T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::gridSelfTile);
	tilesConfigList->append_emplace(rpg3D::T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::gridPathableTiles);

	std::shared_ptr<ExtraData> extraData = std::make_shared<ExtraData>();
		
	std::shared_ptr<graphics::TilingInformation_GridXY> tilingInformation = std::make_shared<graphics::TilingInformation_GridXY>(
		rpg3D::T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::TileVoxelsCount,
		-1/*enabledSidesTriangulationBitMask*/,
		tilesConfigList,
		tilingGrid,
		nullptr/*gridGeometryOffset*/
	);
	extraData->setData(rpg3D::T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::DATA_KEY_materials, base::IWrappedValue::new_Custom<ArrayList<Visual3DModelMaterial*>*>(rpg3D::T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::materialsList, -1));
	extraData->setData(EntityTemplate::ExtraDataID_tilingInformation, base::IWrappedValue::new_CustomB<std::shared_ptr<graphics::TilingInformation>>(tilingInformation, "graphics::TilingInformation"));

	Vector2Int indicatorGPos{ drawRegion.min.x, drawRegion.min.y };
	Vector3 pos{};
	gPosToWorldPos(
		indicatorGPos, 0.0f/*anchorX*/, 0.0f/*anchorY*/,
		pos
	);
	Quaternion rot{ Vector3::Z, 0.0f };
	IWorldEntity* indicatorEntity = gw->getGPMain()->createEntity(
		rpg3D::T_GridPathingIndicatorFloor_A_TiledX6dmY6dm::TEMPLATE,
		pos,
		&rot/*rot*/, nullptr/*cardinalRotation*/,
		nullptr,
		0,
		extraData/*extraData*/,
		true/*isZoneOwnedEntity*/, nullptr/*manualContentGroupsList*/, nullptr/*creatorEntity*/,
		nullptr
	);
	indicatorWEntity = indicatorEntity->getWrapper();
}

GPG2DC_PathableGCellsIndicator::~GPG2DC_PathableGCellsIndicator() {
	//void
}
