#include "SH_Base.h"
#include <base/component/IComponent_ScenariosHandlerComponent.h>

using namespace worldGame3D;

SH_Base::SH_Base(
	IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils, IContainer2D* layer2DGameWorld,
	IApp* app)
	: super(IComponent_ScenariosHandlerComponent::COMPONENT_CATEGORY),
	appAssets(appAssets), appMetrics(appMetrics), appUtils(appUtils),
	app(app),
	layer2DGameWorld(layer2DGameWorld)
{
	//void
}

IContainer2D* SH_Base::getLayer2DGameWorld() {
    return layer2DGameWorld;
}

SH_Base::~SH_Base() {
	//void
}
