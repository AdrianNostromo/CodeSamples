#include "R_GeometryManager.h"
#include <graphics/renderer/geometryManager/GeometryManager.h>
#include <base/exceptions/LogicException.h>
#include <graphics/renderer/geometryManager/GeometryManager.h>

using namespace graphics;

R_GeometryManager::R_GeometryManager()
	: super()
{
	//void
}

void R_GeometryManager::createMain() {
	super::createMain();

	geometryManager = new GeometryManager();
	geometryManager->reservedCreate();
}

IGeometryManager* R_GeometryManager::getGeometryManager() {
	return geometryManager;
}

void R_GeometryManager::onRenderPhaseAdded(IRenderPhase* renderPhase) {
	super::onRenderPhaseAdded(renderPhase);

	geometryManager->onRenderPhaseAdded(renderPhase);
}

void R_GeometryManager::onRenderPhaseRemovePre(IRenderPhase* renderPhase) {
	super::onRenderPhaseRemovePre(renderPhase);

	geometryManager->onRenderPhaseRemovePre(renderPhase);
}

void R_GeometryManager::render(graphics::IEnvironment* environment) {
	super::render(environment);

	geometryManager->tickLogic();
}

void R_GeometryManager::dispose() {
	if (geometryManager != nullptr) {
		geometryManager->reservedDispose();
		delete geometryManager;
		geometryManager = nullptr;
	}

	super::dispose();
}

R_GeometryManager::~R_GeometryManager() {
	//void
}
