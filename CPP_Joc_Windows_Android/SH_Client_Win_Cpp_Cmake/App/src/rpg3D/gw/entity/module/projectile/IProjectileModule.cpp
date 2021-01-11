#include "IProjectileModule.h"
#include <worldGame3D/gw/entity/event/EntityEvent.h>

using namespace rpg3D;

int IProjectileModule::TYPE = GetNew_TYPE();
DEF_componentBasics(IProjectileModule);

base::EntityEvent* IProjectileModule::EVENT_projectile_purposeFinished = new base::EntityEvent(false, "EVENT_projectile_purposeFinished");
