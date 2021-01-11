#include "ScenariosHandler.h"

using namespace worldGame3D;

ScenariosHandler::ScenariosHandler(
    IRenderingHandler* renderingHandler,
    IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
    IContainer2D* layer2DGameWorld,
	IApp* app)
	: super(renderingHandler, appAssets, appMetrics, appUtils, layer2DGameWorld, app)
{
	//void
}

ScenariosHandler::~ScenariosHandler() {
	//void
}
