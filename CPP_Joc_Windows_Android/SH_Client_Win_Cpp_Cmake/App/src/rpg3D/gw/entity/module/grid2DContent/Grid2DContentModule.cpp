#include "Grid2DContentModule.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/IGameWorldGameParts.h>
#include <rpg3D/gw/entity/template/grid2DContent/Grid2DContentTemplate.h>
#include <rpg3D/gw/grid2DContent/IGPGrid2DContent.h>
#include <base/list/IListEntry.h>

using namespace rpg3D;

Grid2DContentModule::Grid2DContentModule(
    IWorldEntity* entity,
    Grid2DContentTemplate* t)
    : super(entity, t)
{
    //void
}

void Grid2DContentModule::createBMain() {
    super::createBMain();
    
    //void
}

void Grid2DContentModule::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
    super::connectToGameParts(gwGameParts);

    gpGrid2DContent = gwGameParts->getComponentAs<rpg3D::IGPGrid2DContent*>(false/*mustExist*/);

    if(gpGrid2DContent != nullptr) {
        gpGrid2DContent->hookGridEntity(this, *e->getPos());
        
        e->addEventListener(
            IWorldEntity::EVENT_syncPos->type,
            std::bind(&Grid2DContentModule::onEvent, this, std::placeholders::_1, std::placeholders::_2),
            localAutoListenersList
        );
    }
}

IListEntry*& Grid2DContentModule::getGridListEntryReference() {
    return gridListEntry;
}

Vector2Int& Grid2DContentModule::getGPosRef(bool mustBeInAGCell) {
    if (mustBeInAGCell && inGCell_listEntry == nullptr) {
        // Entity is not in a gCell.
        throw LogicException(LOC);
    }

    return gPos;
}

Bounds2D& Grid2DContentModule::getGCellBoundsRef(bool mustBeInAGCell) {
    if (mustBeInAGCell && inGCell_listEntry == nullptr) {
        // Entity is not in a gCell.
        throw LogicException(LOC);
    }

    return gCellBounds;
}

IListEntry*& Grid2DContentModule::getInGCell_listEntryRef() {
    return inGCell_listEntry;
}

void Grid2DContentModule::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IWorldEntity::EVENT_syncPos->type) {
        Vector3* pos = e->getPos();
        // Make the inGCell_listEntry optional to handle entities that are outside of the grid area (may be usefull in some cases to have a smaller tracking grid).
        if (inGCell_listEntry == nullptr || !gCellBounds.contains(pos->x, pos->y)) {
            gpGrid2DContent->gridEntityPosChanged(this, *pos);
        }
	}

	else {
		throw LogicException(LOC);
	}
}

void Grid2DContentModule::disconnectFromGamePartsPre() {
    localAutoListenersList.clear();

    if (gpGrid2DContent != nullptr) {
        gpGrid2DContent->releaseGridEntity(this);

        gpGrid2DContent = nullptr;
    }

    super::disconnectFromGamePartsPre();
}

void Grid2DContentModule::disposePre() {
    //void

    super::disposePre();
}

Grid2DContentModule::~Grid2DContentModule() {
	//void
}

