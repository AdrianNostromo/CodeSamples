#include "IStatusEffectsModule.h"
#include <worldGame3D/gw/entity/event/EntityEvent.h>

using namespace rpg3D;

int IStatusEffectsModule::TYPE = GetNew_TYPE();
DEF_componentBasics(IStatusEffectsModule);

base::EntityEvent* IStatusEffectsModule::EVENT_statusEffects_changed = new base::EntityEvent(false, "EVENT_statusEffects_changed");
