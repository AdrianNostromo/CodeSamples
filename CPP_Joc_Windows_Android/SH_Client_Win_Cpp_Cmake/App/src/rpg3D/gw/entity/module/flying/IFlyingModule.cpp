#include "IFlyingModule.h"
#include <worldGame3D/gw/entity/event/EntityEvent.h>

using namespace rpg3D;

int IFlyingModule::TYPE = GetNew_TYPE();
DEF_componentBasics(IFlyingModule);

base::EntityEvent* IFlyingModule::EVENT_FlyingFinished_TargetLost = new base::EntityEvent(false, "EVENT_FlyingFinished_TargetLost");
base::EntityEvent* IFlyingModule::EVENT_FlyingFinished_DestinationReached = new base::EntityEvent(false, "EVENT_FlyingFinished_DestinationReached");
