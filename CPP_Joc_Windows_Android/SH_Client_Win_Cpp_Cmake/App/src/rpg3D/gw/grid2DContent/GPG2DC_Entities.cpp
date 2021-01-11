#include "GPG2DC_Entities.h"
#include <rpg3D/gw/entity/module/grid2DContent/IGrid2DContentModule.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;

GPG2DC_Entities::GPG2DC_Entities(
    base::IGameWorld* gw, 
    Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea)
	: super(gw, gCellSize, allowMultipleEntitiesPerGCell, allowEntitiesOutsideOfTheGridArea)
{
	//void
}

void GPG2DC_Entities::hookGridEntity(IGrid2DContentModule* grid2DContent, Vector3& pos) {
	if(grid2DContent->getGridListEntryReference() != nullptr) {
		throw LogicException(LOC);
	}
	grid2DContent->getGridListEntryReference() = gridEntitiesList.appendDirect(grid2DContent);

	onGridEntityHooked(grid2DContent, pos);
}

void GPG2DC_Entities::onGridEntityHooked(IGrid2DContentModule* grid2DContent, Vector3& pos) {
	//void
}

void GPG2DC_Entities::releaseGridEntity(IGrid2DContentModule* grid2DContent) {
	if (grid2DContent->getGridListEntryReference() == nullptr) {
		throw LogicException(LOC);
	}

	onGridEntityReleasePre(grid2DContent);

	grid2DContent->getGridListEntryReference()->remove();
	grid2DContent->getGridListEntryReference() = nullptr;
}

void GPG2DC_Entities::onGridEntityReleasePre(IGrid2DContentModule* grid2DContent) {
	//void
}

GPG2DC_Entities::~GPG2DC_Entities() {
	//void
}
