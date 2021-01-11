#include "GPG2DC_Base.h"
#include <base/math/Math.h>
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

GPG2DC_Base::GPG2DC_Base(
	base::IGameWorld* gw,
	Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea)
	: super(gw),
	gCellSize(gCellSize), allowMultipleEntitiesPerGCell(allowMultipleEntitiesPerGCell), allowEntitiesOutsideOfTheGridArea(allowEntitiesOutsideOfTheGridArea)
{
	//void
}

void GPG2DC_Base::initTrackingRegion_gridBounds(Bounds2DInt& trackingRegion_gridBounds) {
	if (trackingRegion_type != None || trackingRegion_gridBounds.getLenX() <= 0 || trackingRegion_gridBounds.getLenY() <= 0) {
		throw LogicException(LOC);
	}

	this->trackingRegion_gridBounds.set(trackingRegion_gridBounds);
	trackingRegion_type = GridBounds;
}

void GPG2DC_Base::initTrackingRegion_cellsGrid(sp<Array2D<unsigned char>> trackingRegion_cellsGrid) {
	if (trackingRegion_type != None || trackingRegion_cellsGrid == nullptr) {
		throw LogicException(LOC);
	}
	
	this->trackingRegion_cellsGrid = trackingRegion_cellsGrid;
	this->trackingRegion_gridBounds.min.set(0, 0);
	this->trackingRegion_gridBounds.max.set(this->trackingRegion_cellsGrid->getLengthA(), this->trackingRegion_cellsGrid->getLengthB());
	trackingRegion_type = CellsGrid;
}

const Vector2& GPG2DC_Base::getGridOriginOffset() {
	return gridOriginOffset;
}

void GPG2DC_Base::setGridOriginOffset(Vector2& gridOriginOffset) {
	this->gridOriginOffset.set(gridOriginOffset);
}

void GPG2DC_Base::gPosToWorldPos(
	Vector2Int& gPos, float anchorX, float anchorY,
	Vector2& oWorldPos)
{
	oWorldPos.set(
		(gPos.x + anchorX) * gCellSize.x + gridOriginOffset.x,
		(gPos.y + anchorY) * gCellSize.y + gridOriginOffset.y
	);
}

void GPG2DC_Base::gPosToWorldPos(
	Vector2Int& gPos, float anchorX, float anchorY,
	Vector3& oWorldPos)
{
	oWorldPos.set(
		(gPos.x + anchorX) * gCellSize.x + gridOriginOffset.x,
		(gPos.y + anchorY) * gCellSize.y + gridOriginOffset.y,
		oWorldPos.z
	);
}

void GPG2DC_Base::gPosCenterToWorldPos(Vector2Int& gPos, Vector2& oWorldPos) {
	oWorldPos.set(
		(gPos.x + 0.5f) * gCellSize.x + gridOriginOffset.x,
		(gPos.y + 0.5f) * gCellSize.y + gridOriginOffset.y
	);
}

void GPG2DC_Base::worldPosToGPos(Vector2& worldPos, Vector2Int& oGPos) {
	oGPos.set(
		(int)Math::floor((worldPos.x - gridOriginOffset.x) / gCellSize.x),
		(int)Math::floor((worldPos.y - gridOriginOffset.y) / gCellSize.y)
	);
}

void GPG2DC_Base::worldPosToGPos(Vector3& worldPos, Vector2Int& oGPos) {
	oGPos.set(
		(int)Math::floor((worldPos.x - gridOriginOffset.x) / gCellSize.x),
		(int)Math::floor((worldPos.y - gridOriginOffset.y) / gCellSize.y)
	);
}

void GPG2DC_Base::worldPosToGCellCenterWorldPos(Vector2& worldPos) {
	Vector2Int gPos{
		(int)Math::floor((worldPos.x - gridOriginOffset.x) / gCellSize.x),
		(int)Math::floor((worldPos.y - gridOriginOffset.y) / gCellSize.y)
	};

	gPosCenterToWorldPos(gPos, worldPos);
}

bool GPG2DC_Base::isGPosInTrackingRegion(Vector2Int& gPos) {
	return isGPosInTrackingRegion(gPos.x, gPos.y);
}

bool GPG2DC_Base::isGPosInTrackingRegion(int gX, int gY) {
	bool isInTrackingRegion = false;
	if (trackingRegion_type == GridBounds) {
		isInTrackingRegion = trackingRegion_gridBounds.contains(gX, gY);
	}else if (trackingRegion_type == CellsGrid) {
		// Only empty if 0;
		isInTrackingRegion = trackingRegion_cellsGrid->getDirect(gX, gY, 0) != 0 ? true : false;
	}else {
		throw LogicException(LOC);
	}

	return isInTrackingRegion;
}

Bounds2DInt& GPG2DC_Base::getGridTrackingRegionBounds() {
	return trackingRegion_gridBounds;
}

GPG2DC_Base::~GPG2DC_Base() {
	//void
}
