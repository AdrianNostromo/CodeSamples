#include "Entry_RenderablesManager.h"
#include <base/exceptions/LogicException.h>
#include <graphics/renderer/renderablesManager/IRenderablesManager.h>

using namespace graphics;

Entry_RenderablesManager::Entry_RenderablesManager(IRenderable* renderable, IRenderablesManager* renderablesManager)
	: renderable(renderable), 
	renderablesManager(renderablesManager)
{
	//void
}

void Entry_RenderablesManager::remove() {
	if (renderablesManager == nullptr) {
		throw LogicException(LOC);
	}

	renderablesManager->remove(this);
}

Entry_RenderablesManager::~Entry_RenderablesManager() {
	//void
}
