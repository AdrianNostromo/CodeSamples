#include "GWContent.h"

using namespace base;

GWPartContent::GWPartContent(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
    : super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

base::IGPContent* GWPartContent::getContent() {
    return gpContent;
}

base::IGPContent* GWPartContent::getGPContent() {
    return gpContent;
}

void GWPartContent::createGParts() {
    super::createGParts();

    addGamePartUninitialised(content = gpContent = newGPContent());
}

base::GamePartContent* GWPartContent::newGPContent() {
    base::GamePartContent* ret = new base::GamePartContent(this);

    return ret;
}

void GWPartContent::disposeMain() {
    if (gpContent != nullptr) {
        gpContent = nullptr;
        content = nullptr;
    }

    super::disposeMain();
}

GWPartContent::~GWPartContent() {
    //void
}
