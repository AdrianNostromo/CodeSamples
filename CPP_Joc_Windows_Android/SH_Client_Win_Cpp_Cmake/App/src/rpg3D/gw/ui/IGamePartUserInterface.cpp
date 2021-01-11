#include "IGamePartUserInterface.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/event/GWEvent.h>

using namespace rpg3D;

int IGamePartUserInterface::TYPE = GetNew_TYPE();

DEF_componentBasics(IGamePartUserInterface);

base::GWEvent* IGamePartUserInterface::GWEvent_onEntityEditor_activated = new base::GWEvent("GWEvent_onEntityEditor_activated");
base::GWEvent* IGamePartUserInterface::GWEvent_onEntityEditor_deactivated = new base::GWEvent("GWEvent_onEntityEditor_deactivated");

base::GWEvent* IGamePartUserInterface::GWEvent_onZoneEditor_activated = new base::GWEvent("GWEvent_onZoneEditor_activated");
base::GWEvent* IGamePartUserInterface::GWEvent_onZoneEditor_deactivated = new base::GWEvent("GWEvent_onZoneEditor_deactivated");
