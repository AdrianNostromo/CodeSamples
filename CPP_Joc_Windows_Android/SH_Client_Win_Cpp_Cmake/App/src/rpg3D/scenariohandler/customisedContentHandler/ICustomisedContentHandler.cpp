#include "ICustomisedContentHandler.h"
#include <worldGame3D/gw/event/GWEvent.h>

using namespace rpg3D;

base::GWEvent* ICustomisedContentHandler::GWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded = new base::GWEvent("GWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded");
base::GWEvent* ICustomisedContentHandler::GWEvent_PlayerUnitMaterialsAndVoxelsGrid_edited = new base::GWEvent("GWEvent_PlayerUnitMaterialsAndVoxelsGrid_edited");
