#include "GWPartSim.h"

GWPartSim::GWPartSim(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
    : super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

base::IGPSim* GWPartSim::getSim() {
    return gpSim;
}

base::IGPSim* GWPartSim::getGPSim() {
    return gpSim;
}

int GWPartSim::createComponents() {
    super::createComponents();

    gpSim->createComponents();

    return 0;
}

void GWPartSim::createGParts() {
    super::createGParts();

    addGamePartUninitialised(gpSim = newGPSim());
}

base::GamePartSimExtendable* GWPartSim::newGPSim() {
    // Must be fully extended.

    throw LogicException(LOC);
}

void GWPartSim::isPlayEnabled_changedEvent() {
    super::isPlayEnabled_changedEvent();

    gpSim->isPlayEnabled_changedEvent();
}

void GWPartSim::disposeMain() {
    if(gpSim != nullptr) {
        gpSim = nullptr;
    }

    super::disposeMain();
}

GWPartSim::~GWPartSim() {
    //void
}
