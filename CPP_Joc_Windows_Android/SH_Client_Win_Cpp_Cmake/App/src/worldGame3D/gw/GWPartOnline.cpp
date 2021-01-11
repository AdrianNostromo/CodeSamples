#include "GWPartOnline.h"

using namespace base;

GWPartOnline::GWPartOnline(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
    : super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

IGPOnline* GWPartOnline::getOnline() {
    return gpOnline;
}

IGPOnline* GWPartOnline::getGPOnline() {
    return gpOnline;
}

void GWPartOnline::createGParts() {
    super::createGParts();

    addGamePartUninitialised(gpOnline = newGPOnline());
}

GPOnline* GWPartOnline::newGPOnline() {
    GPOnline* ret = new GPOnline(this);

    return ret;
}

void GWPartOnline::disposeMain() {
    if(gpOnline != nullptr) {
        gpOnline = nullptr;
    }

    super::disposeMain();
}

GWPartOnline::~GWPartOnline() {
    //void
}
