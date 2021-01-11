#include "IGPManager.h"
#include <worldGame3D/gw/event/GWEvent.h>

using namespace rpg3D;

// This event is ignorres the contacts on the first frame. This is used to avoid teleporters activation if the player is spawned on a teleporter.
base::GWEvent* IGPManager::GWEvent_onPlayerActivatedPlatformNonSpawn = new base::GWEvent("GWEvent_onPlayerActivatedPlatformNonSpawn");
