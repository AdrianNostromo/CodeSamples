#include "IContentZone.h"
#include <worldGame3D/gw/event/GWEvent.h>
#include <base/MM.h>

using namespace worldGame3D;

base::GWEvent* IContentZone::GWEvent_onZoneActivated = newt base::GWEvent("GWEvent_onZoneActivated");
base::GWEvent* IContentZone::GWEvent_onZoneDeactivatePre = newt base::GWEvent("GWEvent_onZoneDeactivatePre");
