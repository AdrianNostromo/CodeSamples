#include <worldGame3D/gw/entity/module/touchable3D/Touchable3DModule.h>
#include "GPMTouchable3DEntities.h"
#include <base/math/collision/Intersector.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace base;

GPMTouchable3DEntities::GPMTouchable3DEntities(base::IGameWorld* gw)
    : super(gw)
{
    gw->addEventListener(
        IGamePartMain::GWEvent_onEntityAdded->type, IGamePartMain::GWEvent_onEntityRemoved_Pre->type,
        std::bind(&GPMTouchable3DEntities::onEvent, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );
}

void GPMTouchable3DEntities::onEvent(IEventListener& eventListener, GWEvent& event) {
    if(event.type == IGamePartMain::GWEvent_onEntityAdded->type) {
        onEntityAdded(event.getEntityData());
    }else if(event.type == IGamePartMain::GWEvent_onEntityRemoved_Pre->type) {
        onEntityRemoved_Pre(event.getEntityData());
    }else {
        throw LogicException(LOC);
    }
}

void GPMTouchable3DEntities::createBMain() {
    super::createBMain();

    //void
}

void GPMTouchable3DEntities::onEntityAdded(IWorldEntity* entity) {
    ITouchable3DModule* touchable = entity->getComponentAs<ITouchable3DModule*>(false/*mustExist*/);
    if(touchable != nullptr) {
        touchableEntities.appendReference(entity);
    }
}

void GPMTouchable3DEntities::onEntityRemoved_Pre(IWorldEntity* entity) {
    ITouchable3DModule* touchable = entity->getComponentAs<ITouchable3DModule*>(false/*mustExist*/);
    if(touchable != nullptr) {
        if(!touchableEntities.remove(entity)) {
            throw LogicException(LOC);
        }
    }
}

//asdA0;// Optimisation. All touchable entities are organised in a world grid according to their AABB position (one AABB can be in multiple cells, test the readius from AABB center to compute grid cells to occupy). The ray will be tested against this grid to get a list of cells and enumerate each cell to test it's entities. Need a flag on the entities to not test the same one multiple times (a int testCounter or something).
std::shared_ptr<ArrayList<IGamePartMain::DT_Entity_Distance>> GPMTouchable3DEntities::getEntitiesUnderTouch(Ray& ray, IEntitiesFilter* entityFilter) {
    std::shared_ptr<ArrayList<DT_Entity_Distance>> intersectedEntitiesList = std::make_shared<ArrayList<DT_Entity_Distance>>();

    for(int i=0;i<touchableEntities.size();i++) {
        IWorldEntity* entity = *touchableEntities.getPointer(i);
        if(entity != nullptr && (entityFilter == nullptr || entityFilter->test(entity))) {
            ITouchable3DModule* touchable = entity->getComponentAs<ITouchable3DModule*>(true/*mustExist*/);
            BoundingBox* touchBounds = touchable->getTouchBoundsAABB();
            Vector3 rayIntersectionPoint{};
            if (touchBounds && Intersector::intersectRayBoundsFast(ray, *touchBounds, &rayIntersectionPoint)) {
                float dist = Math::dist3D(
                    rayIntersectionPoint.x, rayIntersectionPoint.y, rayIntersectionPoint.z,
					ray.origin.x, ray.origin.y, ray.origin.z
                );

                if(intersectedEntitiesList->count() == 0) {
                    intersectedEntitiesList->append_emplace(entity, dist);
                }else {
                    int insertIndex = FetchInsertIndexClosestFirst(intersectedEntitiesList, dist);
                    intersectedEntitiesList->insert_emplace(insertIndex, entity, dist);
                }
            }
        }
    }

    return intersectedEntitiesList;
}

int GPMTouchable3DEntities::FetchInsertIndexClosestFirst(std::shared_ptr<ArrayList<DT_Entity_Distance>>& list, float targetDist) {
    for(int i=0;i<list->count();i++) {
        DT_Entity_Distance* entry = list->getPointer(i);

        if(targetDist < entry->dist) {
            return i;
        }
    }

    return list->count();
}

std::shared_ptr<ArrayList<IGamePartMain::DT_Entity_Distance>> GPMTouchable3DEntities::getEntitiesUnderTouch(Ray& ray) {
    return getEntitiesUnderTouch(ray, nullptr);
}

GPMTouchable3DEntities::~GPMTouchable3DEntities() {
    //void
}
