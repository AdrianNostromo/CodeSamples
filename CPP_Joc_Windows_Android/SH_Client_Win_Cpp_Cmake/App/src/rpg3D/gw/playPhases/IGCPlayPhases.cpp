#include "IGCPlayPhases.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/event/GWEvent.h>

using namespace rpg3D;
using namespace rpg3D::playPhases;

int IGCPlayPhases::TYPE = GetNew_TYPE();

DEF_componentBasics(IGCPlayPhases);

base::GWEvent* IGCPlayPhases::GWEvent_turnChanged = new base::GWEvent("GWEvent_turnChanged");
