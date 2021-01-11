#include "SH_CustomContent.h"
#include "customisedContentHandler/CustomisedContentHandler.h"

using namespace rpg3D;

SH_CustomContent::SH_CustomContent(
	IRenderingHandler* renderingHandler,
	IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
	IContainer2D* layer2DGameWorld,
	IApp* app)
	: super(renderingHandler, appAssets, appMetrics, appUtils, layer2DGameWorld, app)
{
	//void
}

void SH_CustomContent::create() {
	super::create();
	
	addComponent(customisedContentHandler = new rpg3D::CustomisedContentHandler(app), true/*doCreate*/);
}

rpg3D::ICustomisedContentHandler* SH_CustomContent::getCustomisedContentHandler() {
	return customisedContentHandler;
}

void SH_CustomContent::onActiveScenarioDeactivatePre(base::IGameWorld* gameWorld) {
	super::onActiveScenarioDeactivatePre(gameWorld);

	getCustomisedContentHandler()->setActiveGameWorld(nullptr);
}

void SH_CustomContent::onActiveScenarioActivated(base::IGameWorld* gameWorld) {
	super::onActiveScenarioActivated(gameWorld);

	getCustomisedContentHandler()->setActiveGameWorld(gameWorld);
}

SH_CustomContent::~SH_CustomContent() {
	//void
}
