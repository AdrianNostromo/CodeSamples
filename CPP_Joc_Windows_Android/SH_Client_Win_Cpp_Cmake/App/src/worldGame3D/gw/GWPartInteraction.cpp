#include "GWPartInteraction.h"

using namespace base;

GWPartInteraction::GWPartInteraction(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
    : super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

IGPInteraction* GWPartInteraction::getGPInteraction() {
    return gpInteraction;
}

IGPInteraction* GWPartInteraction::getInteraction() {
    return gpInteraction;
}

void GWPartInteraction::createGParts() {
    super::createGParts();

    addGamePartUninitialised(gpInteraction = newGPInteraction());
}

GPInteraction* GWPartInteraction::newGPInteraction() {
    GPInteraction* ret = new GPInteraction(this);

    return ret;
}

void GWPartInteraction::disposeMain() {
    if (gpInteraction != nullptr) {
        gpInteraction = nullptr;
    }

    super::disposeMain();
}

GWPartInteraction::~GWPartInteraction() {
    //void
}
