#include "IGPContent.h"

using namespace base;

GWEvent* IGPContent::GWEvent_onPlayerEntitySelected = new GWEvent("GWEvent_onPlayerEntitySelected");
GWEvent* IGPContent::GWEvent_onPlayerEntityDeselectedPre = new GWEvent("GWEvent_onPlayerEntityDeselectedPre");
GWEvent* IGPContent::GWEvent_onSelectedPlayerEntityChanged = new GWEvent("GWEvent_onSelectedPlayerEntityChanged");
