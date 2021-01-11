#include "GWPartMain.h"

using namespace base;

GWPartMain::GWPartMain(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
    : super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

IGamePartMain* GWPartMain::getGPMain() {
    return gpMain;
}

IGamePartMain* GWPartMain::getMain() {
    return gpMain;
}

void GWPartMain::createGParts() {
    super::createGParts();

    addGamePartUninitialised(gpMain = newGPMain());
}

int GWPartMain::createComponents() {
    super::createComponents();

    gpMain->createComponents();

    return 0;
}

int GWPartMain::createLayers() {
    super::createLayers();

    gpMain->createLayers();

    return 0;
}

int GWPartMain::createContent() {
    super::createContent();

    gpMain->createContent();

    return 0;
}

GamePartMain* GWPartMain::newGPMain() {
    GamePartMain* ret = new GamePartMain(this);

    return ret;
}

void GWPartMain::isPlayEnabled_changedEvent() {
    super::isPlayEnabled_changedEvent();

    gpMain->isPlayEnabled_changedEvent();
}

void GWPartMain::disposeMain() {
    if (gpMain != nullptr) {
        gpMain = nullptr;
    }

    super::disposeMain();
}

GWPartMain::~GWPartMain() {
    //void
}
