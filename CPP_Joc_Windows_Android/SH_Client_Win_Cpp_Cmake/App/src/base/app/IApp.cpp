#include "IApp.h"
#include <base/app/event/AppEvent.h>
#include <base/MM.h>

base::AppEvent* IApp::Event_isAppActiveChangePre = new base::AppEvent("Event_isAppActiveChangePre");
base::AppEvent* IApp::Event_isAppActiveChanged = new base::AppEvent("Event_isAppActiveChanged");
