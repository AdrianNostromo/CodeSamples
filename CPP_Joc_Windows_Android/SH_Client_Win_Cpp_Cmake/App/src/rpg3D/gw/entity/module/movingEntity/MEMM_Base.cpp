#include "MEMM_Base.h"

using namespace rpg3D;

int MEMM_Base::moveRequestTypeCounter = 0;

MEMM_Base::MEMM_Base(
    IWorldEntity* entity,
    rpg3D::MovingEntityTemplate* t)
    : super(entity, t)
{
    //void
}

int MEMM_Base::newMoveRequestType() {
    return ++moveRequestTypeCounter;
}

MEMM_Base::~MEMM_Base() {
    //void
}
