#include "RG_Base.h"
#include <base/exceptions/LogicException.h>
#include <stdio.h>
#include <iostream>
#include <vendor/rlutil/rlutil.h>
#include <iostream>
#include <fcntl.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

RG_Base::RG_Base() {
	//void
}

void RG_Base::clear() {
	roomIndex = -1;
	startZonePos.idt();
	zoneGrid = nullptr;
}

void RG_Base::autoGrowRoom(
	int roomIndex, Vector2Int& startZonePos, int preferredRoomSize_smallAxis, int preferredRoomSize_bigAxis,
	Array2D<ZoneGridCell>* zoneGrid, RoomGrowType roomGrowType, Bounds2DInt& oFinalRoomBounds, Random* rng)
{
	if (zoneGrid == nullptr) {
		throw LogicException(LOC);
	}

	clear();

	// Save params.
	this->roomIndex = roomIndex;
	this->startZonePos.set(startZonePos);
	this->preferredRoomSize_smallAxis = preferredRoomSize_smallAxis;
	this->preferredRoomSize_bigAxis = preferredRoomSize_bigAxis;
	this->zoneGrid = zoneGrid;
	this->rng = rng;
}

RG_Base::~RG_Base() {
	//void
}
