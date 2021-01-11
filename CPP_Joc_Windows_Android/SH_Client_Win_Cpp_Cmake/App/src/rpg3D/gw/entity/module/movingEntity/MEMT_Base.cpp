#include "MEMT_Base.h"

using namespace rpg3D;

int MEMT_Base::turnRequestTypeCounter = 0;

MEMT_Base::MEMT_Base(
    IWorldEntity* entity,
    rpg3D::MovingEntityTemplate* t)
    : super(entity, t)
{
	//void
}

int MEMT_Base::newTurnRequestType() {
    return ++turnRequestTypeCounter;
}

MEMT_Base::~MEMT_Base() {
	//void
}
