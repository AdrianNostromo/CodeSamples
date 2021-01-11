#include "R_RenderPhases.h"
#include "renderPhase/IRenderPhase.h"
#include <base/exceptions/LogicException.h>
#include <graphics/renderer/renderablesManager/IRenderablesManager.h>
#include <graphics/model/instance/ModelInstance.h>
#include <base/opengl/util/GLUtil.h>

using namespace graphics;

R_RenderPhases::R_RenderPhases()
	: super()
{
	//void
}

void R_RenderPhases::addRenderPhase(IRenderPhase* renderPhase) {
	if (!renderPhase->getIsCreated()) {
		// The render phase was not initialised. Call reservedCreate on it before.
		throw LogicException(LOC);
	}
	if (renderPhasesMap.containsKeyB(renderPhase->getType())) {
		// Already exists.
		throw LogicException(LOC);
	}
	if (renderPhase->getIsEnabled()) {
		// Renderer change cannont occur while the rPhase is enabled.
		throw LogicException(LOC);
	}
	if (getRenderPhase(renderPhase->getType(), false/*mustExist*/) != nullptr) {
		// RPhase already exists.
		throw LogicException(LOC);
	}

	// Insert using a rendering order priority.
	/// Note. Error if the same rendering order priority is encountered in the list more than once.
	int renderOrder = renderPhase->getRenderOrder();

	bool isInserted = false;
	for (ListDL<IRenderPhase*>::Entry* entry = renderPhasesList.getFirst(); entry != nullptr; entry = entry->next) {
		if (renderOrder < entry->data->getRenderOrder()) {
			renderPhase->getInHandlerListEntryRef() = renderPhasesList.insertBeforeDirect(entry, renderPhase);

			isInserted = true;
		}
	}
	if (!isInserted) {
		renderPhase->getInHandlerListEntryRef() = renderPhasesList.appendDirect(renderPhase);
	}

	renderPhase->setRenderer(this);

	renderPhasesMap.putDirectB(renderPhase->getType(), renderPhase);

	onRenderPhaseAdded(renderPhase);
}

void R_RenderPhases::removeRenderPhaseMustExist(int type) {
	IRenderPhase* renderPhase = getRenderPhase(type, true/*mustExist*/);
	if (renderPhase == nullptr || renderPhase->getInHandlerListEntryRef() == nullptr) {
		throw LogicException(LOC);
	}
	if (renderPhase->getIsEnabled()) {
		// Renderer change cannont occur while the rPhase is enabled.
		throw LogicException(LOC);
	}

	removeRenderPhaseMustExist(renderPhase);
}

void R_RenderPhases::removeRenderPhaseMustExist(IRenderPhase* renderPhase) {
	if (renderPhase == nullptr || renderPhase->getInHandlerListEntryRef() == nullptr) {
		throw LogicException(LOC);
	}
	if (renderPhase->getIsEnabled()) {
		// Renderer change cannont occur while the rPhase is enabled.
		throw LogicException(LOC);
	}

	onRenderPhaseRemovePre(renderPhase);

	renderPhase->setRenderer(nullptr);

	renderPhase->getInHandlerListEntryRef()->remove();
	renderPhase->getInHandlerListEntryRef() = nullptr;

	renderPhasesMap.removeMustExistB(renderPhase->getType());
}

IRenderPhase* R_RenderPhases::getRenderPhase(int rPhaseType, bool mustExist) {
	IRenderPhase* renderPhase = renderPhasesMap.getDirect(rPhaseType, nullptr);
	if (renderPhase == nullptr && mustExist) {
		throw LogicException(LOC);
	}

	return renderPhase;
}

void R_RenderPhases::onRenderPhaseAdded(IRenderPhase* renderPhase) {
	super::onRenderPhaseAdded(renderPhase);

	//void
}

void R_RenderPhases::onRenderPhaseRemovePre(IRenderPhase* renderPhase) {
	super::onRenderPhaseRemovePre(renderPhase);

	//void
}

void R_RenderPhases::prepareForRendering(graphics::IEnvironment* environment) {
	super::prepareForRendering(environment);

	unsigned int errorCode = GLUtil::gl->getError();
	if (errorCode != IGL::NO_ERROR) {
		throw LogicException(LOC);
	}

	for (ListDL<IRenderPhase*>::Entry* entry = renderPhasesList.getFirst(); entry != nullptr; entry = entry->next) {
		IRenderPhase* renderPhase = entry->data;
		if (renderPhase->getIsEnabled()) {
		}
	}
}

void R_RenderPhases::render(graphics::IEnvironment* environment) {
	super::render(environment);

	unsigned int errorCode = GLUtil::gl->getError();
	if (errorCode != IGL::NO_ERROR) {
		throw LogicException(LOC);
	}

	for (ListDL<IRenderPhase*>::Entry* entry = renderPhasesList.getFirst(); entry != nullptr; entry = entry->next) {
		IRenderPhase* renderPhase = entry->data;
		if (renderPhase->getIsEnabled()) {
			renderPhase->render(environment);
		}
	}
}

void R_RenderPhases::dispose() {
	while(renderPhasesList.count() > 0) {
		ListDL<IRenderPhase*>::Entry* entry = renderPhasesList.getFirst();
		IRenderPhase* rPhase = entry->data;
		// The removeRenderPhaseMustExist call will remove it from this list.
		/// entry->remove();

		rPhase->setIsEnabled(false);

		removeRenderPhaseMustExist(rPhase);

		rPhase->reservedDispose();
		delete rPhase;

	}

	super::dispose();
}

R_RenderPhases::~R_RenderPhases() {
	//void
}
