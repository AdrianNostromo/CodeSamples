#include "R_RenderablesManager.h"
#include <graphics/renderer/renderablesManager/RenderablesManager.h>

using namespace graphics;

R_RenderablesManager::R_RenderablesManager()
	: super()
{
	//void
}

void R_RenderablesManager::createMain() {
	super::createMain();

	renderablesManager = new RenderablesManager();
	renderablesManager->handler = this;
	renderablesManager->reservedCreate();
}

void R_RenderablesManager::prepareForRendering(graphics::IEnvironment* environment) {
	super::prepareForRendering(environment);

	renderablesManager->syncModels();
}

IRenderablesManager* R_RenderablesManager::getRenderablesManager() {
	return renderablesManager;
}

void R_RenderablesManager::disposeMain() {
	if (renderablesManager != nullptr) {
		renderablesManager->reservedDisposeMain();
		delete renderablesManager;
		renderablesManager = nullptr;
	}

	super::disposeMain();
}

R_RenderablesManager::~R_RenderablesManager() {
	//void
}
