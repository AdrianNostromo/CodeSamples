#include "IGamePartMain.h"

using namespace base;

GWEvent* IGamePartMain::GWEvent_onEntityAdded = new GWEvent("GWEvent_onEntityAdded");
GWEvent* IGamePartMain::GWEvent_onEntityRemoved_Pre = new GWEvent("GWEvent_onEntityRemoved_Pre");
