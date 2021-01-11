#include "RG_Prefabs.h"
#include <base/exceptions/LogicException.h>

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

RG_Prefabs::RG_Prefabs(sp<CfgRoomGenerator> config, int seed)
	: super(config, seed),
	prefabTilesGSize( 6, 6 )
{
	PrefabTile X{ Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*roomInfo*/, -1/*tilingTileIndex*/};
	PrefabTile V{ Tile::SpecialRoomIds::OutsideOfRoom_NoWall/*roomInfo*/, -1/*tilingTileIndex*/ };// void (no wall)
	// Checker (2 stiles) tiled.
	PrefabTile A{ 0/*roomInfo*/, -2/*tilingTileIndex*/ };
	PrefabTile B{ 1/*roomInfo*/, -2/*tilingTileIndex*/ };// Different room than A;

	prefabGrid_room_edge_wall.resizeNoElementsMove(
		prefabTilesGSize.x, prefabTilesGSize.y,
		false/*invertA*/, true/*invertB*/,
		new PrefabTile[prefabTilesGSize.x * prefabTilesGSize.y] {
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A
		}
	);

	prefabGrid_room_edge_door.resizeNoElementsMove(
		prefabTilesGSize.x, prefabTilesGSize.y,
		false/*invertA*/, true/*invertB*/,
		new PrefabTile[prefabTilesGSize.x * prefabTilesGSize.y] {
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A
		}
	);
	
	prefabGrid_room_corner.resizeNoElementsMove(
		prefabTilesGSize.x, prefabTilesGSize.y,
		false/*invertA*/, true/*invertB*/,
		new PrefabTile[prefabTilesGSize.x * prefabTilesGSize.y] {
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			X,A,A,A,A,A
		}
	);
	
	prefabGrid_room_center.resizeNoElementsMove(
		prefabTilesGSize.x, prefabTilesGSize.y,
		false/*invertA*/, true/*invertB*/,
		new PrefabTile[prefabTilesGSize.x * prefabTilesGSize.y] {
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A,
			A,A,A,A,A,A
		}
	);
	
	prefabGrid_exitHallway.resizeNoElementsMove(
		prefabTilesGSize.x, prefabTilesGSize.y,
		false/*invertA*/, true/*invertB*/,
		new PrefabTile[prefabTilesGSize.x * prefabTilesGSize.y] {
			X,X,X,X,X,X,
			V,B,B,B,B,B,
			V,B,B,B,B,B,
			V,B,B,B,B,B,
			V,B,B,B,B,B,
			X,X,X,X,X,X
		}
	);
	prefabGrid_empty.resizeNoElementsMove(
		prefabTilesGSize.x, prefabTilesGSize.y,
		false/*invertA*/, true/*invertB*/,
		new PrefabTile[prefabTilesGSize.x * prefabTilesGSize.y] {
			X,X,X,X,X,X,
			X,X,X,X,X,X,
			X,X,X,X,X,X,
			X,X,X,X,X,X,
			X,X,X,X,X,X,
			X,X,X,X,X,X
		}
	);
}

void RG_Prefabs::placePrefabInGrid(
	Vector2Int& placementPrefabsGPos,
	Array2D<PrefabTile>& prefabGrid,
	bool preFlipX, bool preFlipY, int postRotDeg)
{
	int outTilesGPos_x = placementPrefabsGPos.x * prefabTilesGSize.x;
	int outTilesGPos_y = placementPrefabsGPos.y * prefabTilesGSize.y;

	PrefabTile* prefabGridData = prefabGrid.getData();

	// This will be used as the floor tiling grid (grassA, grassB).
	signed char* floorTilingTilesGrid = floorTilingGrid->getData();

	int prefabTilesGSize_x = prefabGrid.getLengthA();
	int prefabTilesGSize_y = prefabGrid.getLengthB();

	float prefabsTilesGCenterX = static_cast<float>(prefabTilesGSize_x - 1) / 2.0f;
	float prefabsTilesGCenterY = static_cast<float>(prefabTilesGSize_y - 1) / 2.0f;

	for (int i = 0; i < prefabTilesGSize_x; i++) {
		for (int j = 0; j < prefabTilesGSize_y; j++) {
			int inPrefabTilesGPos_x = i;
			int inPrefabTilesGPos_y = j;

			int outGPosX = outTilesGPos_x + i;
			int outGPosY = outTilesGPos_y + j;

			if (postRotDeg == 0) {
				if (preFlipX) {
					inPrefabTilesGPos_x = (prefabTilesGSize_x - 1) - inPrefabTilesGPos_x;
				}
				if (preFlipY) {
					inPrefabTilesGPos_y = (prefabTilesGSize_y - 1) - inPrefabTilesGPos_y;
				}
			} else {
				// Use imaginary number coords and multiply by 'i' to a power to get the rotation.
				// The rotation center is the grid center.
				float fCellPosX = inPrefabTilesGPos_x - prefabsTilesGCenterX;
				float fCellPosY = inPrefabTilesGPos_y - prefabsTilesGCenterY;
				if (preFlipX) {
					fCellPosX *= -1;
				}
				if (preFlipY) {
					fCellPosY *= -1;
				}

				float newCellPosX;
				float newCellPosY;

				// The rotations code is inverted so that the input_cell is rotated and not the output_cell.
				if (postRotDeg == 90) {
					// This is imaginary coords multiplied by 'i^3=-i';
					newCellPosX = fCellPosY;
					newCellPosY = -fCellPosX;
				} else if (postRotDeg == 180) {
					// This is imaginary coords multiplied by 'i^2=-1';
					newCellPosX = -fCellPosX;
					newCellPosY = -fCellPosY;
				} else if (postRotDeg == 270 || postRotDeg == -90) {
					// This is imaginary coords multiplied by 'i';
					newCellPosX = -fCellPosY;
					newCellPosY = fCellPosX;
				} else {
					throw LogicException(LOC);
				}

				// This does a floor.
				inPrefabTilesGPos_x = static_cast<int>(newCellPosX + prefabsTilesGCenterX + 0.1f);
				inPrefabTilesGPos_y = static_cast<int>(newCellPosY + prefabsTilesGCenterY + 0.1f);
			}
			
			tilesGrid[outGPosY * tilesGridSize.x + outGPosX].roomId = prefabGridData[inPrefabTilesGPos_y * prefabTilesGSize_x + inPrefabTilesGPos_x].roomInfo;

			signed char tilingTileIndex = prefabGridData[inPrefabTilesGPos_y * prefabTilesGSize_x + inPrefabTilesGPos_x].tilingTileIndex;
			if (tilingTileIndex == -2) {
				tilingTileIndex = (outGPosX + outGPosY) % 2;
			}
			floorTilingTilesGrid[outGPosY * tilesGridSize.x + outGPosX] = tilingTileIndex;
		}
	}
}

RG_Prefabs::~RG_Prefabs() {
	//void
}
