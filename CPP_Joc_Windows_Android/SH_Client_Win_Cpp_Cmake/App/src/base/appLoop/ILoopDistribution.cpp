#include "ILoopDistribution.h"
#include <base/appLoop/event/LoopEvent.h>
#include <base/MM.h>

using namespace base;

base::LoopEvent* ILoopDistribution::AppLoopEvent_input = new base::LoopEvent("AppLoopEvent_input");
base::LoopEvent* ILoopDistribution::AppLoopEvent_inputDispatch = new base::LoopEvent("AppLoopEvent_inputDispatch");
base::LoopEvent* ILoopDistribution::AppLoopEvent_general = new base::LoopEvent("AppLoopEvent_general");
base::LoopEvent* ILoopDistribution::AppLoopEvent_eco = new base::LoopEvent("AppLoopEvent_eco");
base::LoopEvent* ILoopDistribution::AppLoopEvent_gameWorld_pre = new base::LoopEvent("AppLoopEvent_gameWorld_pre");
base::LoopEvent* ILoopDistribution::AppLoopEvent_gameWorld = new base::LoopEvent("AppLoopEvent_gameWorld");
base::LoopEvent* ILoopDistribution::AppLoopEvent_ui = new base::LoopEvent("AppLoopEvent_ui");
base::LoopEvent* ILoopDistribution::AppLoopEvent_delayedComponentsDispose = new base::LoopEvent("AppLoopEvent_delayedComponentsDispose");
