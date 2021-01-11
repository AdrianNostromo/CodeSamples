#include "IGPTurnBasedPlay.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/event/GWEvent.h>

using namespace rpg3D;

int IGPTurnBasedPlay::TYPE = GetNew_TYPE();

DEF_componentBasics(IGPTurnBasedPlay);

base::GWEvent* IGPTurnBasedPlay::GWEvent_turnChanged = new base::GWEvent("GWEvent_turnChanged");
