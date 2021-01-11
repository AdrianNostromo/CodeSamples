#include <worldGame3D/gw/manager/GPManager.h>
#include "GWPartManager.h"

using namespace base;

GWPartManager::GWPartManager(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
    : super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

IGPManager* GWPartManager::getManager() {
    return manager;
}

void GWPartManager::createGParts() {
    super::createGParts();

    addGamePartUninitialised(manager = newGPManager());
}

GPManager* GWPartManager::newGPManager() {
    GPManager* ret = new GPManager(this);

    return ret;
}

void GWPartManager::disposeMain() {
    if(manager != nullptr) {
        manager = nullptr;
    }

    super::disposeMain();
}

GWPartManager::~GWPartManager() {
    //void
}
