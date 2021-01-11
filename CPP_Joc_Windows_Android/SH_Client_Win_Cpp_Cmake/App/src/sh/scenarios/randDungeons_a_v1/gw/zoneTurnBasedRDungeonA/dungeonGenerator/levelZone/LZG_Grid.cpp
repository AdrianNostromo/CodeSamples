#include "LZG_Grid.h"
#include <iostream>
#include <vendor/rlutil/rlutil.h>
#if defined(_MSC_VER)
#include <io.h>
#include <fcntl.h>
#endif
#include <base/math/util/BoolUtil.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

const int LZG_Grid::ZONE_GRID_PADDING = 8;

LZG_Grid::LZG_Grid(
	std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
	Random* rng)
	: super(
		config, zoneIndex,
		rng)
{
	//void
}

void LZG_Grid::buildSpecialLocationsListAndZoneGrid() {
	int zoneGridPadding = ZONE_GRID_PADDING;

	specialLocationsList = new Array1D<SpecialLocation>{ config->specialRoomsCount };

	int gridCellsCount = config->zoneFloorGridSize_width * config->zoneFloorGridSize_height;
	float gridAspectRatio = (float)config->zoneFloorGridSize_width / (float)config->zoneFloorGridSize_height;

	int fragmentSpaceGridSizeY = Math::ceil(Math::sqrt((float)config->specialRoomsCount / gridAspectRatio));
	int fragmentSpaceGridSizeX = Math::ceil((float)config->specialRoomsCount / (float)fragmentSpaceGridSizeY);

	int fragmentSpaceGridCellSizeX = Math::floor(config->zoneFloorGridSize_width / (float)fragmentSpaceGridSizeX);
	int fragmentSpaceGridCellSizeY = Math::floor(config->zoneFloorGridSize_height / (float)fragmentSpaceGridSizeY);

	int zoneGridSizeX = zoneGridPadding + fragmentSpaceGridSizeX * fragmentSpaceGridCellSizeX + zoneGridPadding;
	int zoneGridSizeY = zoneGridPadding + fragmentSpaceGridSizeY * fragmentSpaceGridCellSizeY + zoneGridPadding;

	zoneGrid = new Array2D<ZoneGridCell>{
		zoneGridSizeX,
		zoneGridSizeY
	};

	// Prepare all cell location in an array.
	Array1D<Vector2Int> fragmentSpaceSpecialLocationsList{ fragmentSpaceGridSizeX * fragmentSpaceGridSizeY };
	PrepareFragmentSpaceSpecialLocationsList(
		fragmentSpaceSpecialLocationsList, fragmentSpaceGridSizeX, fragmentSpaceGridSizeY,
		rng
	);

	// Create a special zone_grid entry randomly inside each regions_grid_slot.
	populateSpecialLocationsList(
		fragmentSpaceGridCellSizeX, fragmentSpaceGridCellSizeY, fragmentSpaceSpecialLocationsList,
		zoneGridPadding
	);
}

void LZG_Grid::PrepareFragmentSpaceSpecialLocationsList(
	Array1D<Vector2Int>& fragmentSpaceSpecialLocationsList, int fragmentSpaceGridSizeX, int fragmentSpaceGridSizeY,
	Random* rng)
{
	// Fill the list with the grid slots in order.
	for (int yy = 0; yy < fragmentSpaceGridSizeY; yy++) {
		for (int xx = 0; xx < fragmentSpaceGridSizeX; xx++) {
			int index = yy * fragmentSpaceGridSizeX + xx;

			fragmentSpaceSpecialLocationsList.getReference(index).set(xx, yy);
		}
	}

	// Shuffle the list.
	int lastEntryIndex = fragmentSpaceSpecialLocationsList.getCount() - 1;
	// Don't process the entry at index 0 because it will just replace it's self.
	while (lastEntryIndex > 0) {
		int randIndex = rng->nextInt(lastEntryIndex);

		fragmentSpaceSpecialLocationsList.getReference(randIndex).replaceEachOther(fragmentSpaceSpecialLocationsList.getReference(lastEntryIndex));

		lastEntryIndex--;
	}
}

void LZG_Grid::populateSpecialLocationsList(
	int fragmentSpaceGridCellSizeX, int fragmentSpaceGridCellSizeY, Array1D<Vector2Int>& fragmentSpaceSpecialLocationsList,
	int zoneGridPadding)
{
	for (int roomIndex = 0; roomIndex < specialLocationsList->getCount(); roomIndex++) {
		Vector2Int& p = fragmentSpaceSpecialLocationsList.getReference(roomIndex);

		int positionVarianceX = Math::floor((fragmentSpaceGridCellSizeX - 1) * config->specialRoomsSubGridPositionRandomisingStrength);
		int positionVarianceY = Math::floor((fragmentSpaceGridCellSizeY - 1) * config->specialRoomsSubGridPositionRandomisingStrength);

		int retriesCount = 0;
		while (true) {
			if (retriesCount > 50) {
				throw LogicException(LOC);
			}

			int gPosX = zoneGridPadding + p.x * fragmentSpaceGridCellSizeX + rng->nextInt(0, positionVarianceX);
			int gPosY = zoneGridPadding + p.y * fragmentSpaceGridCellSizeY + rng->nextInt(0, positionVarianceY);

			if (zoneGrid->getReference(gPosX, gPosY).roomIndex != -1) {
				// Grid slot occupied, try again.
				retriesCount++;

				continue;
			}

			specialLocationsList->getReference(roomIndex).pos.set(
				gPosX,
				gPosY
			);

			zoneGrid->getReference(gPosX, gPosY).roomIndex = roomIndex;

			break;
		}
	}
}

void LZG_Grid::logIntermediaryPathingGrid() {
	std::string s = "logIntermediaryPathingGrid\n";
	for (int y = zoneGrid->getLengthB() - 1; y >= 0; y--) {
		for (int x = 0; x < zoneGrid->getLengthB(); x++) {
			ZoneGridCell& val = zoneGrid->getReference(x, y);
			if (val.pathingDists[0] >= 0) {
				s.push_back(Math::encodeNumberToSingleChar(val.pathingDists[0]));
				s += "."; 
			} else if (val.roomIndex >= 0) {
				s += "X.";
			} else {
				s += " .";
			}
		}

		s += "\n";
	}
	printf("%s", s.c_str());
}

void LZG_Grid::logPathingRoadsGrid() {
	std::string s = "logPathingRoadsGrid\n";
	for (int y = zoneGrid->getLengthB() - 1; y >= 0; y--) {
		for (int x = 0; x < zoneGrid->getLengthB(); x++) {
			ZoneGridCell& val = zoneGrid->getReference(x, y);
			if (val.roadDirBitMap != 0) {
				s += "#.";
			} else {
				s += " .";
			}
		}

		s += "\n";
	}
	printf("%s", s.c_str());
}

void LZG_Grid::logPathingRoadsAndRoomsGrid() {
	std::string s = "logPathingRoadsAndRoomsGrid\n";
	for (int y = zoneGrid->getLengthB() - 1; y >= 0; y--) {
		for (int x = 0; x < zoneGrid->getLengthB(); x++) {
			ZoneGridCell& val = zoneGrid->getReference(x, y);
			if (val.roomIndex >= 0) {
				s += "X.";
			} else if (val.roadDirBitMap != 0) {
				s += "#.";
			} else {
				s += " .";
			}
		}

		s += "\n";
	}
	printf("%s", s.c_str());
}

void LZG_Grid::logSpecialLocationsAndRoadsGrid() {
	std::string s = "logSpecialLocationsAndRoadsGrid\n";
	for (int y = zoneGrid->getLengthB() - 1; y >= 0; y--) {
		for (int x = 0; x < zoneGrid->getLengthB(); x++) {
			ZoneGridCell& val = zoneGrid->getReference(x, y);
			if (val.roomIndex >= 0) {
				s.push_back(Math::encodeNumberToSingleChar(val.roomIndex));
				s += ".";
			} else if (val.roadDirBitMap != 0) {
				s += "#.";
			} else {
				s += " .";
			}
		}

		s += "\n";
	}
	printf("%s", s.c_str());
}

void LZG_Grid::logRooms() {
	std::string s = "logRooms\n";
	for (int y = zoneGrid->getLengthB() - 1; y >= 0; y--) {
		for (int x = 0; x < zoneGrid->getLengthB(); x++) {
			ZoneGridCell& val = zoneGrid->getReference(x, y);
			if (val.roomIndex >= 0) {
				s.push_back(Math::encodeNumberToSingleChar(val.roomIndex));
				s += ".";
			} else if (val.roadDirBitMap != 0) {
				// There must be no un-roomified roads.
				throw LogicException(LOC);
			} else {
				s += " .";
			}
		}

		s += "\n";
	}
	printf("%s", s.c_str());
}

void LZG_Grid::logZoneGrid(bool drawPerZoneBgFill, bool drawRoadsWithPriority) {
#if defined(_MSC_VER)
	int prevMode = _setmode(_fileno(stdout), _O_U8TEXT);
#endif
	rlutil::saveDefaultColor();

	std::wcout << L"Log zoneGrid: ";
	if (zoneGrid != nullptr) {
		std::wcout << zoneGrid->getLengthA() << L", " << zoneGrid->getLengthB() << std::endl;
	} else {
		std::wcout << L"nullptr" << std::endl;

		return;
	}

	int l = zoneGrid->getLengthA() * 2 - 1 + 2;
	for (int gX = 0; gX < l; gX++) {
		std::wcout << L"\u2591";
	}
	std::wcout << std::endl;

	for (int gY = zoneGrid->getLengthB() - 1; gY >= 0; gY--) {
		for (int gX = 0; gX < zoneGrid->getLengthA(); gX++) {
			ZoneGridCell& gCell = zoneGrid->getReference(gX, gY);

			if (gX == 0) {
				rlutil::setColor(rlutil::WHITE);
				rlutil::setBackgroundColor(rlutil::BLACK);

				std::wcout << L"\u2591";
			}

			if (drawPerZoneBgFill && gCell.roomIndex >= 0) {
				const int colsCount = 13;
				static int colsList[colsCount]{
					rlutil::BLUE,
					rlutil::GREEN,
					rlutil::CYAN,
					rlutil::RED,
					rlutil::MAGENTA,
					rlutil::BROWN,
					rlutil::GREY,
					rlutil::LIGHTBLUE,
					rlutil::LIGHTGREEN,
					rlutil::LIGHTCYAN,
					rlutil::LIGHTRED,
					rlutil::LIGHTMAGENTA,
					rlutil::YELLOW
				};
				int zoneCol = colsList[gCell.roomIndex % colsCount];
				rlutil::setBackgroundColor(zoneCol);
			} else {
				bool isGreyBg = false;
				if ((gX % 2) != 0) {
					isGreyBg = true;
				}
				if ((gY % 2) != 0) {
					isGreyBg = !isGreyBg;
				}
				if (!isGreyBg) {
					rlutil::setBackgroundColor(rlutil::BLACK);
				} else {
					rlutil::setBackgroundColor(rlutil::DARKGREY);
				}
			}

			if (gCell.roomIndex >= 0 && (!drawRoadsWithPriority || gCell.roadDirBitMap == 0)) {
				std::wcout << Math::encodeNumberToSingleChar(gCell.roomIndex);
				if (gX < zoneGrid->getLengthA() - 1) {
					std::wcout << L" ";
				}
			} else if (gCell.roadDirBitMap != 0) {
				// Corners LB LT RT RB \u2517 \u250F \u2513 \u251B
				// Edge ends xPositive xNegative yPositive yNegative \u257A \u2578 \u2579 \u257B
				// T intersections xPositive_yBoth xNegative_yBoth xBoth_yPositive xBoth_yNegative \u2523 \u252B \u253B \u2533
				// Unconnected room \u25A1
				if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xPositive | ZoneGridCell::RoadDir::xNegative | ZoneGridCell::RoadDir::yPositive | ZoneGridCell::RoadDir::yNegative)) {
					// Cross \u254B
					std::wcout << L"\u254B";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xPositive | ZoneGridCell::RoadDir::yNegative | ZoneGridCell::RoadDir::yPositive)) {
					std::wcout << L"\u2523";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xNegative | ZoneGridCell::RoadDir::yNegative | ZoneGridCell::RoadDir::yPositive)) {
					std::wcout << L"\u252B";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xNegative | ZoneGridCell::RoadDir::xPositive | ZoneGridCell::RoadDir::yPositive)) {
					std::wcout << L"\u253B";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xNegative | ZoneGridCell::RoadDir::xPositive | ZoneGridCell::RoadDir::yNegative)) {
					std::wcout << L"\u2533";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xPositive | ZoneGridCell::RoadDir::xNegative)) {
					// Horizontal \u2501
					std::wcout << L"\u2501";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::yPositive | ZoneGridCell::RoadDir::yNegative)) {
					// Vertical \u2503
					std::wcout << L"\u2503";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xPositive | ZoneGridCell::RoadDir::yPositive)) {
					std::wcout << L"\u2517";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xPositive | ZoneGridCell::RoadDir::yNegative)) {
					std::wcout << L"\u250F";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xNegative | ZoneGridCell::RoadDir::yNegative)) {
					std::wcout << L"\u2513";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xNegative | ZoneGridCell::RoadDir::yPositive)) {
					std::wcout << L"\u251B";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xPositive)) {
					std::wcout << L"\u257A";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::xNegative)) {
					std::wcout << L"\u2578";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::yPositive)) {
					std::wcout << L"\u2579";
				} else if (BoolUtil::isBitsAll(gCell.roadDirBitMap, ZoneGridCell::RoadDir::yNegative)) {
					std::wcout << L"\u257B";
				} else {
					std::wcout << L"\u25A1";
				}

				if (gX < zoneGrid->getLengthA() - 1) {
					if (gCell.roadDirBitMap != 0 && (ZoneGridCell::RoadDir::xPositive & gCell.roadDirBitMap) != 0) {
						// Right direction road exists.
						std::wcout << L"\u2501";
					} else {
						std::wcout << L" ";
					}
				}
			} else {
				std::wcout << L" ";
				if (gX < zoneGrid->getLengthA() - 1) {
					std::wcout << L" ";
				}
			}

			if (gX == zoneGrid->getLengthA() - 1) {
				rlutil::setColor(rlutil::WHITE);
				rlutil::setBackgroundColor(rlutil::BLACK);

				std::wcout << L"\u2591";
			}
		}

		std::wcout << std::endl;
	}

	l = zoneGrid->getLengthA() * 2 - 1 + 2;
	for (int gX = 0; gX < l; gX++) {
		std::wcout << L"\u2591";
	}
	std::wcout << std::endl;

	rlutil::resetColor();
#if defined(_MSC_VER)
	_setmode(_fileno(stdout), prevMode);
#endif
}

LZG_Grid::~LZG_Grid() {
	if (zoneGrid != nullptr) {
		delete zoneGrid;
		zoneGrid = nullptr;
	}
	if (specialLocationsList != nullptr) {
		delete specialLocationsList;
		specialLocationsList = nullptr;
	}
}
