#include "RG_AlgoRegionRoomsFinder.h"
#include <iostream>
#include <base/exceptions/LogicException.h>
#include <vendor/rlutil/rlutil.h>
#include <iostream>
#if defined(_MSC_VER)
#include <io.h>
#include <fcntl.h>
#endif
#include <base/math/Random.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

RG_AlgoRegionRoomsFinder::RG_AlgoRegionRoomsFinder()
	: super()
{
	//void
}

void RG_AlgoRegionRoomsFinder::initRoomWallDistancesGrid() {
	// Compute the algo square grid size.
	int algoSquareNonOriginRadius = preferredRoomSize_bigAxis - 1;

	roomsFinderAlgoZoneBounds.min.set(
		startZonePos.x - algoSquareNonOriginRadius,
		startZonePos.y - algoSquareNonOriginRadius
	);
	roomsFinderAlgoZoneBounds.max.set(
		roomsFinderAlgoZoneBounds.min.x + algoSquareNonOriginRadius * 2 + 1,
		roomsFinderAlgoZoneBounds.min.y + algoSquareNonOriginRadius * 2 + 1
	);
	roomsFinderAlgoZoneBounds.intersect(
		0/*xMin*/, zoneGrid->getLengthA()/*xMax*/,
		0/*yMin*/, zoneGrid->getLengthB()/*yMax*/
	);

	// Create a per-room grid that contains the intermediary data of: room-valid g_cells right and up of the target room grid cell.
	// The distances are until a un-pathed road or the algo square edge reached. The projections are horizontal_right and vertical_up.
	roomWallDistancesGrid = std::make_unique<Array2D<Vector2Int>>(roomsFinderAlgoZoneBounds.getLenX(), roomsFinderAlgoZoneBounds.getLenY());
	roomWallDistancesGrid->zeroSetData();

	tmp_roomRoadsValidationList = std::make_unique<Array1D<signed char>>(roomsFinderAlgoZoneBounds.getLenX() * roomsFinderAlgoZoneBounds.getLenY());

	roomGCellsToProcessMask = std::make_unique<Array2D<signed char>>(roomsFinderAlgoZoneBounds.getLenX(), roomsFinderAlgoZoneBounds.getLenY());
	roomGCellsToProcessMask->zeroSetData();

	Array1D<int> verticalXDistToEdgeList{ roomWallDistancesGrid->getLengthB() };
	verticalXDistToEdgeList.zeroSetData();

	for (int gX = roomWallDistancesGrid->getLengthA() - 1; gX >= 0; gX--) {
		int yDistToEdge = 0;
		for (int gY = roomWallDistancesGrid->getLengthB() - 1; gY >= 0; gY--) {
			ZoneGridCell& zoneGCell = zoneGrid->getReference(roomsFinderAlgoZoneBounds.min.x + gX, roomsFinderAlgoZoneBounds.min.y + gY);

			if ((zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) || (zoneGCell.roadDirBitMap != 0 && zoneGCell.pathingDists[0] == -1)) {
				// Different_roomIndex or unpathed_road;
				verticalXDistToEdgeList.getReference(gY) = 0;
				yDistToEdge = 0;
			} else {
				// (Same_roomIndex or no_roomIndex) and (no_road or pathed_road).
				verticalXDistToEdgeList.getReference(gY)++;
				yDistToEdge++;
			}

			int xDistToEdge = verticalXDistToEdgeList.getReference(gY);
			roomWallDistancesGrid->getReference(gX, gY).set(xDistToEdge, yDistToEdge);
		}
	}
}

void RG_AlgoRegionRoomsFinder::updateRoomWallDistancesGridForNewPathedRoad(Vector2Int& zoneRoadPos) {
	Vector2Int roomRoadPos{};
	zonePosToRoomAlgoSubZonePos(zoneRoadPos, roomRoadPos);

	// From new road gCell to the left (until algo_bounds or unpathed_road encountered), update the x distance in increasing value. The distance is taken from the right gCell (use 0 if outside of bounds) and increment it by 1 for each left movement.
	int nextGCellDistVal = 0;
	if (roomsFinderAlgoZoneBounds.contains(zoneRoadPos.x + 1, zoneRoadPos.y)) {
		ZoneGridCell& zoneGCell = zoneGrid->getReference(zoneRoadPos.x + 1, zoneRoadPos.y);
		if ((zoneGCell.roomIndex < 0 || zoneGCell.roomIndex == roomIndex) && (zoneGCell.roadDirBitMap == 0 || zoneGCell.pathingDists[0] >= 0)) {
			nextGCellDistVal = roomWallDistancesGrid->getReference(roomRoadPos.x + 1, roomRoadPos.y).x;
		}
	}
	for (int x = zoneRoadPos.x; x >= roomsFinderAlgoZoneBounds.min.x; x--) {
		ZoneGridCell& zoneGCell = zoneGrid->getReference(x, zoneRoadPos.y);

		if ((zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) || (zoneGCell.roadDirBitMap != 0 && zoneGCell.pathingDists[0] == -1)) {
			// Different_roomIndex or unpathed_road;
			break;
		}

		nextGCellDistVal++;
		Vector2Int roomPos{};
		zonePosToRoomAlgoSubZonePos(
			x, zoneRoadPos.y,
			roomPos
		);
		roomWallDistancesGrid->getReference(roomPos).x = nextGCellDistVal;
	}

	// Do the above algo for the y axis.
	nextGCellDistVal = 0;
	if (roomsFinderAlgoZoneBounds.contains(zoneRoadPos.x, zoneRoadPos.y + 1)) {
		ZoneGridCell& zoneGCell = zoneGrid->getReference(zoneRoadPos.x, zoneRoadPos.y + 1);
		if ((zoneGCell.roomIndex < 0 || zoneGCell.roomIndex == roomIndex) && (zoneGCell.roadDirBitMap == 0 || zoneGCell.pathingDists[0] >= 0)) {
			nextGCellDistVal = roomWallDistancesGrid->getReference(roomRoadPos.x, roomRoadPos.y + 1).y;
		}
	}
	for (int y = zoneRoadPos.y; y >= roomsFinderAlgoZoneBounds.min.y; y--) {
		ZoneGridCell& zoneGCell = zoneGrid->getReference(zoneRoadPos.x, y);

		if ((zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) || (zoneGCell.roadDirBitMap != 0 && zoneGCell.pathingDists[0] == -1)) {
			// Different_roomIndex or unpathed_road;
			break;
		}

		nextGCellDistVal++;
		Vector2Int roomPos{};
		zonePosToRoomAlgoSubZonePos(
			zoneRoadPos.x, y,
			roomPos
		);
		roomWallDistancesGrid->getReference(roomPos).y = nextGCellDistVal;
	}
}

int RG_AlgoRegionRoomsFinder::tickRoomsFinderAlgo(
	int& zoneRoomCandidatesDistanceFromOptimal, ArrayList<Bounds2DInt>& zoneRoomCandidatesList)
{
	int roadGCellsProcessedCount = prepareGCellsToProcessRegion();
	if (roadGCellsProcessedCount == 0) {
		return 0;
	}

	//logDistancesGrid();

	for (int roomX = roomGCellsToProcessBounds.min.x; roomX < roomGCellsToProcessBounds.max.x; roomX++) {
		for (int roomY = roomGCellsToProcessBounds.min.y; roomY < roomGCellsToProcessBounds.max.y; roomY++) {
			char localGCellMask = roomGCellsToProcessMask->getDirect(roomX, roomY);
			if (localGCellMask == 0) {
				continue;
			}

			findAndPushValidAllRoomsVariations(
				roomX, roomY,
				zoneRoomCandidatesDistanceFromOptimal, zoneRoomCandidatesList);
		}
	}

	return roadGCellsProcessedCount;
}

int RG_AlgoRegionRoomsFinder::prepareGCellsToProcessRegion() {
	int roadGCellsProcessedCount = 0;

	std::shared_ptr<ArrayList<Vector2Int>> currentPathingGCells = peekCurrentPathingGCells();

	if (currentPathingGCells->count() == 0) {
		throw LogicException(LOC);
	}

	// Take all modified roads that are inside the algo bounds and combine them to a to-process bounds.
	// Also use a temp char[algoArea_w][algoArea_h] that contains 1 where a g_cell entry to process exists.
	roomGCellsToProcessMask->zeroSetData();
	bool isBoundsInit = false;
	roomGCellsToProcessBounds.set(0, 0, 0, 0);

	for (int i = 0; i < currentPathingGCells->count(); i++) {
		Vector2Int& newRoadZoneGCellPos = currentPathingGCells->getReference(i);

		if (roomsFinderAlgoZoneBounds.contains(newRoadZoneGCellPos)) {
			roadGCellsProcessedCount++;

			ZoneGridCell& newRoadZoneGCell = zoneGrid->getReference(newRoadZoneGCellPos);
			// Ignore roads that enter into different rooms.
			if (newRoadZoneGCell.roomIndex < 0/*no_roomIndex*/ || newRoadZoneGCell.roomIndex == roomIndex/*same_roomIndex*/) {
				Bounds2DInt maskPartRoomBounds{};
				computMaskPartBoundsFromNewRoad(newRoadZoneGCellPos, maskPartRoomBounds);

				if (!isBoundsInit) {
					isBoundsInit = true;

					roomGCellsToProcessBounds.set(maskPartRoomBounds);
				} else {
					roomGCellsToProcessBounds.unionOther(maskPartRoomBounds);
				}
				roomGCellsToProcessMask->setRegionDirect(&maskPartRoomBounds, 1/*val*/);

				// Need to set some roomWallDistancesGrid entries.
				updateRoomWallDistancesGridForNewPathedRoad(newRoadZoneGCellPos);
			}
		}
	}

	return roadGCellsProcessedCount;
}

void RG_AlgoRegionRoomsFinder::computMaskPartBoundsFromNewRoad(Vector2Int& newRoadZoneGCellPos, Bounds2DInt& oMaskPartRoomBounds) {
	// Move to the left until algoBounds or different_roomIndex or unpathed_road detected.
	int maskAreaZonePosX = -1;
	for (int gX = newRoadZoneGCellPos.x; gX >= roomsFinderAlgoZoneBounds.min.x; gX--) {
		ZoneGridCell& zoneGCell = zoneGrid->getReference(gX, newRoadZoneGCellPos.y);

		if ((zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) || (zoneGCell.roadDirBitMap != 0 && zoneGCell.pathingDists[0] == -1)) {
			// Different_roomIndex or unpathed_road;
			break;
		}

		maskAreaZonePosX = gX;
	}
	if (maskAreaZonePosX < 0) {
		throw LogicException(LOC);
	}
	
	// Do the same for the y axis.
	int maskAreaZonePosY = -1;
	for (int gY = newRoadZoneGCellPos.y; gY >= roomsFinderAlgoZoneBounds.min.y; gY--) {
		ZoneGridCell& zoneGCell = zoneGrid->getReference(newRoadZoneGCellPos.x, gY);

		if ((zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) || (zoneGCell.roadDirBitMap != 0 && zoneGCell.pathingDists[0] == -1)) {
			// Different_roomIndex or unpathed_road;
			break;
		}

		maskAreaZonePosY = gY;
	}
	if (maskAreaZonePosY < 0) {
		throw LogicException(LOC);
	}

	// Set the bounds.
	Vector2Int maskAreaRoomPos{};
	zonePosToRoomAlgoSubZonePos(maskAreaZonePosX, maskAreaZonePosY, maskAreaRoomPos);
	oMaskPartRoomBounds.min.set(maskAreaRoomPos);

	Vector2Int newRoadRoomGCellPos{};
	zonePosToRoomAlgoSubZonePos(newRoadZoneGCellPos.x + 1, newRoadZoneGCellPos.y + 1, newRoadRoomGCellPos);
	oMaskPartRoomBounds.max.set(newRoadRoomGCellPos);
}

void RG_AlgoRegionRoomsFinder::findAndPushValidAllRoomsVariations(
	int srcRoomX, int srcRoomY,
	int& zoneRoomCandidatesDistanceFromOptimal, ArrayList<Bounds2DInt>& zoneRoomCandidatesList)
{
	// For each width group save a single room zone. Don't save each line as a possible room zone.

	Vector2Int srcZonePos{};
	roomAlgoSubZonePosToZonePos(srcRoomX, srcRoomY, srcZonePos);

	// Start at the src pos and enumerate up until bounds_end or different_roomIndex or unpathable_road reched.
	int minRoomWidth = -1;
	Vector2Int& srcRoomWallDistancesVal = roomWallDistancesGrid->getReference(srcRoomX, srcRoomY);
	for (int offsetY = 0; offsetY < srcRoomWallDistancesVal.y; offsetY++) {
		// Check the current offsetY.
		{
			Vector2Int zoneCPos{};
			roomAlgoSubZonePosToZonePos(srcRoomX, srcRoomY + offsetY, zoneCPos);

			ZoneGridCell& zoneGCell = zoneGrid->getReference(zoneCPos);
			if ((zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) || (zoneGCell.roadDirBitMap != 0 && zoneGCell.pathingDists[0] == -1)) {
				// Different_roomIndex or unpathed_road;
				break;
			}

			Vector2Int& roomWallDistancesVal = roomWallDistancesGrid->getReference(srcRoomX, srcRoomY + offsetY);

			if (minRoomWidth == -1 || roomWallDistancesVal.x < minRoomWidth) {
				minRoomWidth = roomWallDistancesVal.x;
			}
			if (minRoomWidth == 0) {
				// This should be caught by the above if.
				throw LogicException(LOC);
			}
		}

		// Try to increase the offsetY as long as minRoomWidth will not change (the gCells above have the same or bigger roomWallDistancesVal.x).
		while(true) {
			// Manually increase offsetY until the last line that has the same roomWallDistancesVal.x; Must also include all the above checks.
			// Check if there is a offsetY above.
			int nextOffsetY = offsetY + 1;
			if (nextOffsetY >= srcRoomWallDistancesVal.y) {
				break;
			}

			Vector2Int zoneCPos{};
			roomAlgoSubZonePosToZonePos(srcRoomX, srcRoomY + nextOffsetY, zoneCPos);

			ZoneGridCell& zoneGCell = zoneGrid->getReference(zoneCPos);
			if ((zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) || (zoneGCell.roadDirBitMap != 0 && zoneGCell.pathingDists[0] == -1)) {
				// Different_roomIndex or unpathed_road;
				break;
			}

			Vector2Int& roomWallDistancesVal = roomWallDistancesGrid->getReference(srcRoomX, srcRoomY + nextOffsetY);
			if (roomWallDistancesVal.x < minRoomWidth) {
				// The above gCell will make the room sizex smaller. don't include it in the current room_region, it will be included in the next room_region.
				break;
			}

			offsetY = nextOffsetY;
		}

		// Create a bounds.
		Bounds2DInt roomZoneBounds{
			srcZonePos.x, srcZonePos.x + minRoomWidth,
			srcZonePos.y, srcZonePos.y + (offsetY + 1)
		};

		// If room larger than required, clip the bounds. This is required to validate the room at the desired size because rooms cannot be shrinked without possibly loosing validation.
		if (roomZoneBounds.getLenX() > roomZoneBounds.getLenY() || (roomZoneBounds.getLenX() == roomZoneBounds.getLenY() && rng->nextFloat() < 0.5f)) {
			// X is the main axis or (x==y and 50% of the time).
			if (roomZoneBounds.getLenX() > preferredRoomSize_bigAxis) {
				roomZoneBounds.setLenX(preferredRoomSize_bigAxis);
			}
			if (roomZoneBounds.getLenY() > preferredRoomSize_smallAxis) {
				roomZoneBounds.setLenY(preferredRoomSize_smallAxis);
			}
		} else {
			// Y is the main axis or (x==y and 50% of the time).
			if (roomZoneBounds.getLenY() > preferredRoomSize_bigAxis) {
				roomZoneBounds.setLenY(preferredRoomSize_bigAxis);
			}
			if (roomZoneBounds.getLenX() > preferredRoomSize_smallAxis) {
				roomZoneBounds.setLenX(preferredRoomSize_smallAxis);
			}

		}

		// Room_zone must contain the start pos.
		if (!roomZoneBounds.contains(startZonePos)) {
			continue;
		}

		// Room_zone must contain at least 1 of each pathing distances from 0 to lates one.
		if (!validateRoomRoads(roomZoneBounds)) {
			continue;
		}

		// Handle room saving for possible selection.
		// These rooms are always less or equal to the desired size, they are never larger.
		// Use a list and a current list area. If a closer in size room is found, clear the list of previous entries.
		int areaDistanceFromOptimal;
		if ((roomZoneBounds.getLenX() == preferredRoomSize_smallAxis && roomZoneBounds.getLenY() == preferredRoomSize_bigAxis) || (roomZoneBounds.getLenX() == preferredRoomSize_bigAxis && roomZoneBounds.getLenY() == preferredRoomSize_smallAxis)) {
			areaDistanceFromOptimal = 0;
		} else {
			areaDistanceFromOptimal = preferredRoomSize_smallAxis * preferredRoomSize_bigAxis - roomZoneBounds.getArea();
			if (areaDistanceFromOptimal < 0) {
				// The room is larger than the requested size. This should never occur because the room sizes are clipped if larger.
				throw LogicException(LOC);
			}
		}

		if (zoneRoomCandidatesDistanceFromOptimal < 0 || areaDistanceFromOptimal < zoneRoomCandidatesDistanceFromOptimal) {
			// New group occured.
			zoneRoomCandidatesDistanceFromOptimal = areaDistanceFromOptimal;

			zoneRoomCandidatesList.clear();
			zoneRoomCandidatesList.append_emplace(roomZoneBounds);
		} else if (areaDistanceFromOptimal == zoneRoomCandidatesDistanceFromOptimal) {
			// Same group. Just append.

			zoneRoomCandidatesList.append_emplace(roomZoneBounds);
		}

		//std::cout << "Room bounds (x, y, w, h): " << roomZoneBounds.min.x << ", " << roomZoneBounds.min.y << ", " << roomZoneBounds.getLenX() << ", " << roomZoneBounds.getLenY() << std::endl;
	}
}

bool RG_AlgoRegionRoomsFinder::validateRoomRoads(Bounds2DInt& roomZoneBounds) {
	// Clear a temp array.
	tmp_roomRoadsValidationList->zeroSetData();

	// Eumerate each cell that contains a road and save the dist in the array.
	for (int zoneGX = roomZoneBounds.min.x; zoneGX < roomZoneBounds.max.x; zoneGX++) {
		for (int zoneGY = roomZoneBounds.min.y; zoneGY < roomZoneBounds.max.y; zoneGY++) {
			ZoneGridCell& zoneGCell = zoneGrid->getReference(zoneGX, zoneGY);

			if (zoneGCell.roadDirBitMap != 0 && zoneGCell.pathingDists[0] >= 0) {
				tmp_roomRoadsValidationList->setDirect(zoneGCell.pathingDists[0], 1);
			}
		}
	}

	// Enumerate the list, it must have at least one of each distance until the latest pathing distance.
	int currentPathingRoadDist = getCurrentPathingRoadDist();
	// Use <= to include the latest dist also (eg. incldue the dist 0 at start).
	for (int i = 0; i <= currentPathingRoadDist; i++) {
		char val = tmp_roomRoadsValidationList->getDirect(i);
		if (val == 0) {
			return false;
		}
	}

	return true;
}

void RG_AlgoRegionRoomsFinder::clear() {
	super::clear();

	roomsFinderAlgoZoneBounds.set(0, 0, 0, 0);
}

void RG_AlgoRegionRoomsFinder::zonePosToRoomAlgoSubZonePos(Vector2Int& zonePos, Vector2Int& algoSubZonePos) {
	algoSubZonePos.set(
		zonePos.x - roomsFinderAlgoZoneBounds.min.x,
		zonePos.y - roomsFinderAlgoZoneBounds.min.y
	);
}

void RG_AlgoRegionRoomsFinder::zonePosToRoomAlgoSubZonePos(int zonePosX, int zonePosY, Vector2Int& algoSubZonePos) {
	algoSubZonePos.set(
		zonePosX - roomsFinderAlgoZoneBounds.min.x,
		zonePosY - roomsFinderAlgoZoneBounds.min.y
	);
}

void RG_AlgoRegionRoomsFinder::roomAlgoSubZonePosToZonePos(Vector2Int& algoSubZonePos, Vector2Int& zonePos) {
	zonePos.set(
		roomsFinderAlgoZoneBounds.min.x + algoSubZonePos.x,
		roomsFinderAlgoZoneBounds.min.y + algoSubZonePos.y
	);
}

void RG_AlgoRegionRoomsFinder::roomAlgoSubZonePosToZonePos(int algoSubZonePosX, int algoSubZonePosY, Vector2Int& zonePos) {
	zonePos.set(
		roomsFinderAlgoZoneBounds.min.x + algoSubZonePosX,
		roomsFinderAlgoZoneBounds.min.y + algoSubZonePosY
	);
}
void RG_AlgoRegionRoomsFinder::logDistancesGrid() {
#if defined(_MSC_VER)
	int prevMode = _setmode(_fileno(stdout), _O_U8TEXT);
#endif
	rlutil::saveDefaultColor();

	std::wcout << L"log roomWallDistancesGrid: ";
	if (roomWallDistancesGrid != nullptr) {
		std::wcout << roomWallDistancesGrid->getLengthA() << L", " << roomWallDistancesGrid->getLengthB() << std::endl;
	} else {
		std::wcout << L"nullptr" << std::endl;

		return;
	}

	std::wcout << L"X";
	for (int gX = 1; gX < roomWallDistancesGrid->getLengthA() * 2 - 1 + 2; gX++) {
		std::wcout << L"\u2591";
	}
	std::wcout << L" ";
	std::wcout << L"Y";
	for (int gX = 1; gX < roomWallDistancesGrid->getLengthA() * 2 - 1 + 2; gX++) {
		std::wcout << L"\u2591";
	}
	std::wcout << std::endl;

	for (int gY = roomWallDistancesGrid->getLengthB() - 1; gY >= 0; gY--) {
		// X Value
		for (int gX = 0; gX < roomWallDistancesGrid->getLengthA(); gX++) {
			Vector2Int& gCell = roomWallDistancesGrid->getReference(gX, gY);

			if (gX == 0) {
				rlutil::setColor(rlutil::WHITE);
				rlutil::setBackgroundColor(rlutil::BLACK);

				std::wcout << L"\u2591";
			}

			if (roomGCellsToProcessMask->getDirect(gX, gY) != 0) {
				rlutil::setBackgroundColor(rlutil::DARKGREY);
			} else {
				rlutil::setBackgroundColor(rlutil::BLACK);
			}

			Vector2Int zoneGPos{};
			roomAlgoSubZonePosToZonePos(gX, gY, zoneGPos);
			ZoneGridCell& zoneGCell = zoneGrid->getReference(zoneGPos);
			if (zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) {
				rlutil::setColor(rlutil::YELLOW);
			} else if (zoneGCell.roadDirBitMap != 0) {
				if (zoneGCell.pathingDists[0] >= 0) {
					// Pathed road.
					rlutil::setColor(rlutil::GREEN);
				} else {
					rlutil::setColor(rlutil::RED);
				}
			} else {
				rlutil::setColor(rlutil::WHITE);
			}

			std::wcout << Math::encodeNumberToSingleChar(gCell.x);
			if (gX < roomWallDistancesGrid->getLengthA() - 1) {
				std::wcout << L" ";
			}

			if (gX == roomWallDistancesGrid->getLengthA() - 1) {
				rlutil::setColor(rlutil::WHITE);
				rlutil::setBackgroundColor(rlutil::BLACK);

				std::wcout << L"\u2591";
			}
		}

		// Y Value
		std::wcout << L" ";
		for (int gX = 0; gX < roomWallDistancesGrid->getLengthA(); gX++) {
			Vector2Int& gCell = roomWallDistancesGrid->getReference(gX, gY);

			if (gX == 0) {
				rlutil::setColor(rlutil::WHITE);
				rlutil::setBackgroundColor(rlutil::BLACK);

				std::wcout << L"\u2591";
			}

			if (roomGCellsToProcessMask->getDirect(gX, gY) != 0) {
				rlutil::setBackgroundColor(rlutil::DARKGREY);
			} else {
				rlutil::setBackgroundColor(rlutil::BLACK);
			}

			Vector2Int zoneGPos{};
			roomAlgoSubZonePosToZonePos(gX, gY, zoneGPos);
			ZoneGridCell& zoneGCell = zoneGrid->getReference(zoneGPos);
			if (zoneGCell.roomIndex >= 0 && zoneGCell.roomIndex != roomIndex) {
				rlutil::setColor(rlutil::YELLOW);
			} else if (zoneGCell.roadDirBitMap != 0) {
				if (zoneGCell.pathingDists[0] >= 0) {
					// Pathed road.
					rlutil::setColor(rlutil::GREEN);
				} else {
					rlutil::setColor(rlutil::RED);
				}
			} else {
				rlutil::setColor(rlutil::WHITE);
			}

			std::wcout << Math::encodeNumberToSingleChar(gCell.y);
			if (gX < roomWallDistancesGrid->getLengthA() - 1) {
				std::wcout << L" ";
			}

			if (gX == roomWallDistancesGrid->getLengthA() - 1) {
				rlutil::setColor(rlutil::WHITE);
				rlutil::setBackgroundColor(rlutil::BLACK);

				std::wcout << L"\u2591";
			}
		}

		std::wcout << std::endl;
	}

	for (int gX = 0; gX < roomWallDistancesGrid->getLengthA() * 2 - 1 + 2; gX++) {
		std::wcout << L"\u2591";
	}
	std::wcout << L" ";
	for (int gX = 0; gX < roomWallDistancesGrid->getLengthA() * 2 - 1 + 2; gX++) {
		std::wcout << L"\u2591";
	}
	std::wcout << std::endl;

	rlutil::resetColor();
#if defined(_MSC_VER)
	_setmode(_fileno(stdout), prevMode);
#endif
}

RG_AlgoRegionRoomsFinder::~RG_AlgoRegionRoomsFinder() {
	//void
}
