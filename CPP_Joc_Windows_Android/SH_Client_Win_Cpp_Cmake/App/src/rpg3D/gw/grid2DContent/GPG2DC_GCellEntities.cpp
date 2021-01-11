#include "GPG2DC_GCellEntities.h"
#include <rpg3D/gw/entity/module/grid2DContent/IGrid2DContentModule.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;

GPG2DC_GCellEntities::GPG2DC_GCellEntities(
    base::IGameWorld* gw, 
    Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea)
	: super(gw, gCellSize, allowMultipleEntitiesPerGCell, allowEntitiesOutsideOfTheGridArea)
{
	//void
}

void GPG2DC_GCellEntities::onGridEntityHooked(IGrid2DContentModule* grid2DContent, Vector3& pos) {
	super::onGridEntityHooked(grid2DContent, pos);

	gridEntityPosChanged(grid2DContent, pos);
}

void GPG2DC_GCellEntities::onGridEntityReleasePre(IGrid2DContentModule* grid2DContent) {
	if (grid2DContent->getInGCell_listEntryRef() != nullptr) {
		removeEntityFromGCell(grid2DContent);
	}

	super::onGridEntityReleasePre(grid2DContent);
}

void GPG2DC_GCellEntities::gridEntityPosChanged(IGrid2DContentModule* grid2DContent, Vector3& pos) {
	Vector2Int& gPosRef = grid2DContent->getGPosRef(false/*mustBeInAGCell*/);
	IListEntry*& inGCell_listEntry = grid2DContent->getInGCell_listEntryRef();

	if (inGCell_listEntry != nullptr) {
		Vector2Int newGPos{};
		worldPosToGPos(pos, newGPos);
		if (gPosRef.equals(newGPos)) {
			// Entity already in this gCell, this should never occur.
			throw LogicException(LOC);
		}

		removeEntityFromGCell(grid2DContent);
	}

	addEntityToGCell(grid2DContent, pos);
}

sp<ListDL<IGrid2DContentModule*>> GPG2DC_GCellEntities::getAllEntitiesAtGPos(int gPosX, int gPosY, bool errorOnOutOfBounds) {
	if (!isGPosInTrackingRegion(gPosX, gPosY)) {
		// Outside of grid bounds.
		if (errorOnOutOfBounds) {
			throw LogicException(LOC);
		}

		return nullptr;
	}
	
	sp<ListDL<IGrid2DContentModule*>> gCellEntriesList = gridEntitiesMap.getDirect(gPosX, gPosY, nullptr);

	return gCellEntriesList;
}

bool GPG2DC_GCellEntities::getIsGCellOccupied(int gPosX, int gPosY) {
	sp<ListDL<IGrid2DContentModule*>> gCellEntriesList = gridEntitiesMap.getDirect(gPosX, gPosY, nullptr);

	return gCellEntriesList != nullptr && gCellEntriesList->count() > 0 ? true : false;
}

void GPG2DC_GCellEntities::addEntityToGCell(IGrid2DContentModule* grid2DContent, Vector3& pos) {
	Vector2Int& gPosRef = grid2DContent->getGPosRef(false/*mustBeInAGCell*/);
	IListEntry*& inGCell_listEntryRef = grid2DContent->getInGCell_listEntryRef();
	if (inGCell_listEntryRef != nullptr) {
		// Already in a gCell, remove it first.
		throw LogicException(LOC);
	}

	Vector2Int gPos{};
	worldPosToGPos(pos, gPos);

	if (!isGPosInTrackingRegion(gPos)) {
		if (!allowEntitiesOutsideOfTheGridArea) {
			// Outside of grid area is not allowed.
			throw LogicException(LOC);
		} else {
			// Not in the grid area.
			return;
		}
	}

	gPosRef.set(gPos);

	sp<ListDL<IGrid2DContentModule*>> gCellEntriesList = gridEntitiesMap.getDirect(gPos.x, gPos.y, nullptr);
	if (gCellEntriesList == nullptr) {
		gridEntitiesMap.putDirect(gPos.x, gPos.y, gCellEntriesList = new ListDL<IGrid2DContentModule*>());
	}
	if (!allowMultipleEntitiesPerGCell && gCellEntriesList->count() > 0) {
		// Only 1 entity is currently allowed per gCell.
		throw LogicException(LOC);
	}

	inGCell_listEntryRef = gCellEntriesList->appendDirect(grid2DContent);
	grid2DContent->getGCellBoundsRef(false/*mustBeInAGCell*/).set(
		gPosRef.x * gCellSize.x + gridOriginOffset.x/*xMin*/, gPosRef.y * gCellSize.y + gridOriginOffset.y/*yMin*/,
		(gPosRef.x + 1) * gCellSize.x + gridOriginOffset.x/*xMax*/, (gPosRef.y + 1) * gCellSize.y + gridOriginOffset.y/*yMax*/
	);

	//GlobalAccessLogHandler::devLogHandler->post_debug("gPos %i %i", gPosRef.x, gPosRef.y);
}

void GPG2DC_GCellEntities::removeEntityFromGCell(IGrid2DContentModule* grid2DContent) {
	Vector2Int& gPos = grid2DContent->getGPosRef();
	IListEntry*& inGCell_listEntryRef = grid2DContent->getInGCell_listEntryRef();
	if (inGCell_listEntryRef == nullptr) {
		throw LogicException(LOC);
	}

	sp<ListDL<IGrid2DContentModule*>> gCellEntriesList = gridEntitiesMap.getDirect(gPos.x, gPos.y, nullptr);
	if (gCellEntriesList == nullptr) {
		throw LogicException(LOC);
	}

	inGCell_listEntryRef->remove();
	inGCell_listEntryRef = nullptr;

	if (gCellEntriesList->count() == 0) {
		gridEntitiesMap.removeMustExist(gPos.x, gPos.y);
	}
}

GPG2DC_GCellEntities::~GPG2DC_GCellEntities() {
	//void
}
