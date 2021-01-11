#include "GPG2DC_GCellReservations.h"
#include <rpg3D/gw/entity/module/grid2DContent/IGrid2DContentModule.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;

GPG2DC_GCellReservations::GPG2DC_GCellReservations(
    base::IGameWorld* gw, 
    Vector2& gCellSize, bool allowMultipleEntitiesPerGCell, bool allowEntitiesOutsideOfTheGridArea)
	: super(gw, gCellSize, allowMultipleEntitiesPerGCell, allowEntitiesOutsideOfTheGridArea)
{
	//void
}

GPG2DC_GCellReservations::~GPG2DC_GCellReservations() {
	//void
}
