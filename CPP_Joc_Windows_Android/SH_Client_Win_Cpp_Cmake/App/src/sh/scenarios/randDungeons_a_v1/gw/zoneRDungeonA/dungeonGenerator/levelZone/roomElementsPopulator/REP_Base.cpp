#include "REP_Base.h"
#include <iostream>
#include <vendor/rlutil/rlutil.h>
#if defined(_MSC_VER)
#include <io.h>
#include <fcntl.h>
#endif

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

REP_Base::REP_Base()
	: super()
{
	//void
}

void REP_Base::generate(
	int roomIndex, 
	int gridCellTilesSize, int doorsVisualTilesSize,
	Bounds2DInt& roomZoneSoftBounds,
	Array2D<ZoneGridCell>* zoneGrid,
	bool hasEntryPointStairs, bool hasExitStairs, bool hasStairsUp, bool hasStairsDown,
	int mobsCount,
	std::shared_ptr<Array2D<short>>& oLevelZoneRoomIndexesGrid)
{
	this->roomIndex = roomIndex;
	this->gridCellTilesSize = gridCellTilesSize;
	this->doorsVisualTilesSize = doorsVisualTilesSize;
	this->roomZoneSoftBounds.set(roomZoneSoftBounds);
	this->zoneGrid = zoneGrid;
	this->hasEntryPointStairs = hasEntryPointStairs;
	this->hasExitStairs = hasExitStairs;
	this->hasStairsUp = hasStairsUp;
	this->hasStairsDown = hasStairsDown;
	this->mobsCount = mobsCount;
	this->oLevelZoneRoomIndexesGrid = oLevelZoneRoomIndexesGrid;

	result = std::make_shared<Result_RoomElementsPopulator>();

	// This is used to combine walls, doors into a start_pos and a tiles count.
	roomTilesInfoGrid = std::make_unique<Array2D<SideEsementsInfo>>(
		roomZoneSoftBounds.getLenX() * gridCellTilesSize,
		roomZoneSoftBounds.getLenY() * gridCellTilesSize
	);
}

void REP_Base::logTilesGrid() {
#if defined(_MSC_VER)
	int prevMode = _setmode(_fileno(stdout), _O_U8TEXT);
#endif
	rlutil::saveDefaultColor();

	for (int sideIndex = 0; sideIndex < 4; sideIndex++) {
		rlutil::setColor(rlutil::WHITE);
		rlutil::setBackgroundColor(rlutil::BLACK);
		std::wcout << L"Log tiles grid. sideIndex: " << sideIndex << std::endl;

		for (int tilesGY = roomTilesInfoGrid->getLengthB() - 1; tilesGY >= 0; tilesGY--) {
			for (int tilesGX = 0; tilesGX < roomTilesInfoGrid->getLengthA(); tilesGX++) {
				SideEsementsInfo& sideEsementsInfo = roomTilesInfoGrid->getReference(tilesGX, tilesGY);

				// Blue doors. Grey walls. Black rest.
				if (sideEsementsInfo.sidesList[sideIndex].type == Result_RoomElementsPopulator::SideElementType::Door) {
					rlutil::setColor(rlutil::WHITE);
					rlutil::setBackgroundColor(rlutil::BLUE);
				} else if (sideEsementsInfo.sidesList[sideIndex].type == Result_RoomElementsPopulator::SideElementType::Wall) {
					rlutil::setColor(rlutil::WHITE);
					rlutil::setBackgroundColor(rlutil::DARKGREY);
				} else {
					rlutil::setColor(rlutil::WHITE);
					rlutil::setBackgroundColor(rlutil::BLACK);
				}
				std::wcout << sideEsementsInfo.sidesList[sideIndex].tilesLen << " ";
			}

			std::wcout << std::endl;
		}
	}

	rlutil::resetColor();
#if defined(_MSC_VER)
	_setmode(_fileno(stdout), prevMode);
#endif
}

REP_Base::~REP_Base() {
	//void
}
