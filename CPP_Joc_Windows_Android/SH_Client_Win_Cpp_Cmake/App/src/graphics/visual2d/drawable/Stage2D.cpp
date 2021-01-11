/*
* To change this license header, choose License Headers in Project Properties.
* To change this template file, choose Tools | Templates
* and open the template in the editor.
*/

/*
* File:   Stage2D.cpp
* Author: Adrin
*
* Created on June 1, 2018, 2:48 PM
*/

#include "Stage2D.h"
#include "IRenderOrderDrawableNode2D.h"
#include <base/exceptions/LogicException.h>
#include <graphics/model/RenderableLiteInstanceVI.h>
#include <base/list/ListDL.h>

Stage2D::Stage2D(graphics::IEnvironment2D* environment, IInputHandler* inputHandler)
	:super(environment, inputHandler)
{
	//void
}

void Stage2D::createMain() {
	setRoot(this);
}

int Stage2D::getGlobalTransformPreupdateIndexCounter() {
	return globalTransformPreupdateIndexCounter;
}

void Stage2D::incrementGlobalTransformPreupdateIndexCounter() {
	this->globalTransformPreupdateIndexCounter++;
}

void Stage2D::insertRenderOrderDrawable(IRenderOrderDrawableNode2D* drawable) {
	if (drawable->getInStage_renderOrderDrawablesListEntryRef() != nullptr) {
		throw LogicException(LOC);
	}

	// Don't check if the renderData is in the renderer because this precedes that.
	if (drawable->getHasRenderable() && drawable->getRenderData() == nullptr) {
		// Render data required but not found. This should not occur.
		throw LogicException(LOC);
	}

	IRenderOrderDrawableNode2D* previousOrParentDrawable = drawable->computePreviousOrParentDrawableInStageRenderOrder();
	if (previousOrParentDrawable != nullptr) {
		if (previousOrParentDrawable->getInStage_renderOrderDrawablesListEntryRef() == nullptr) {
			// The previous or parent must be in the list already.
			throw LogicException(LOC);
		}

		drawable->getInStage_renderOrderDrawablesListEntryRef() = renderOrderDrawablesList.insertAfterDirect(
			previousOrParentDrawable->getInStage_renderOrderDrawablesListEntryRef()/*existingEntry*/, drawable/*data*/
		);
	}else {
		// No previous entry. This should occur only for stage.
		if (drawable != this) {
			throw LogicException(LOC);
		}

		drawable->getInStage_renderOrderDrawablesListEntryRef() = renderOrderDrawablesList.prependDirect(
			drawable/*data*/
		);
	}

	// Handler previous_drawable_with_renderable update.
	IRenderOrderDrawableNode2D* previousDrawableWithRenderable = nullptr;
	if (drawable->getInStage_renderOrderDrawablesListEntryRef()->prev != nullptr) {
		if (drawable->getInStage_renderOrderDrawablesListEntryRef()->prev->data->getHasRenderable()) {
			previousDrawableWithRenderable = drawable->getInStage_renderOrderDrawablesListEntryRef()->prev->data;
		} else {
			previousDrawableWithRenderable = drawable->getInStage_renderOrderDrawablesListEntryRef()->prev->data->getPreviousDrawableWithRenderableRef();
		}
	}

	// Manualy set the self drawable.
	drawable->getPreviousDrawableWithRenderableRef() = previousDrawableWithRenderable;

	if (drawable->getHasRenderable()) {
		// For the next entries until a renderable_user occurs, set the new drawable.
		UpdateNextGroupRenderOrderDrawablesWithRenderable(
			drawable->getInStage_renderOrderDrawablesListEntryRef()->next/*startEntry*/,
			previousDrawableWithRenderable/*old_drawableWithRenderable*/, drawable/*new_drawableWithRenderable*/
		);
	}
}

void Stage2D::removeRenderOrderDrawable(IRenderOrderDrawableNode2D* drawable) {
	if (drawable->getInStage_renderOrderDrawablesListEntryRef() == nullptr) {
		throw LogicException(LOC);
	}

	// Handler previous_drawable_with_renderable update.
	if (drawable->getHasRenderable()) {
		IRenderOrderDrawableNode2D* previousDrawableWithRenderable = nullptr;
		if (drawable->getInStage_renderOrderDrawablesListEntryRef()->prev != nullptr) {
			previousDrawableWithRenderable = drawable->getPreviousDrawableWithRenderableRef();
		}

		// Get the previous entry value if any and set that to the next ones until a renderable_user is detected.
		UpdateNextGroupRenderOrderDrawablesWithRenderable(
			drawable->getInStage_renderOrderDrawablesListEntryRef()->next/*startEntry*/,
			drawable/*old_drawableWithRenderable*/, previousDrawableWithRenderable/*new_drawableWithRenderable*/
		);
	}

	drawable->getInStage_renderOrderDrawablesListEntryRef()->remove();
	drawable->getInStage_renderOrderDrawablesListEntryRef() = nullptr;
}

void Stage2D::UpdateNextGroupRenderOrderDrawablesWithRenderable(
	ListDL<IRenderOrderDrawableNode2D*>::Entry* startEntry,
	IRenderOrderDrawableNode2D* old_drawableWithRenderable, IRenderOrderDrawableNode2D* new_drawableWithRenderable)
{
	if (startEntry == nullptr) {
		return;
	}

	for (ListDL<IRenderOrderDrawableNode2D*>::Entry* entry = startEntry; entry != nullptr; entry = entry->next) {
		if (entry->data->getPreviousDrawableWithRenderableRef() != old_drawableWithRenderable) {
			// The previous value should have been old_drawableWithRenderable.
			throw LogicException(LOC);
		}

		entry->data->getPreviousDrawableWithRenderableRef() = new_drawableWithRenderable;

		if (entry->data->getHasRenderable()) {
			// The entry drawable uses it's own renderable.
			break;
		}
	}
}

int Stage2D::syncStageDrawSpecs(
	const int parentInvalidationBitMap,
	graphics::IRenderer& renderer)
{
	RenderData2D* preRenderData = nullptr;

	return syncDrawSpecsWhereNeeded(
		parentInvalidationBitMap,
		renderer,
		&preRenderData
	);
}

Stage2D::~Stage2D() {
    //void
}
