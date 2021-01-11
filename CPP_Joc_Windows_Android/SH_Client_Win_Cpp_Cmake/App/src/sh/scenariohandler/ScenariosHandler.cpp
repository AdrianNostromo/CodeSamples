#include "ScenariosHandler.h"
#include <base/exceptions/UnhandledCaseException.h>
#include <base/exceptions/LogicException.h>

using namespace sh;

ScenariosHandler::ScenariosHandler(
    IRenderingHandler* renderingHandler,
    IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
    IContainer2D* layer2DGameWorld,
    IAppSquareHeads* appLocal)
    : super(renderingHandler, appAssets, appMetrics, appUtils, layer2DGameWorld, appLocal)
{
    //void
}

void ScenariosHandler::onGWEvent(std::string id, std::shared_ptr<base::GWEventDataBase> data) {
    bool isProcessed = true;
    try {
        super::onGWEvent(id, data);
    }catch(UnhandledCaseException& x) {
        isProcessed = false;
    }
    if(!isProcessed) {
        throw LogicException(LOC);
    }
}

ScenariosHandler::~ScenariosHandler() {
    //void
}
