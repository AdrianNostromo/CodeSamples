#include "SH_Base.h"

using namespace rpg3D;

SH_Base::SH_Base(
	IRenderingHandler* renderingHandler,
	IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
	IContainer2D* layer2DGameWorld,
	IApp* app)
	: super(renderingHandler, appAssets, appMetrics, appUtils, layer2DGameWorld, app)
{
	//void
}

SH_Base::~SH_Base() {
	//void
}
