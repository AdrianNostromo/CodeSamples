#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/TouchStage2D.h>
#include <graphics/visual/drawable/IStage.h>
#include <base/MM.h>

class Stage2D : public TouchStage2D, virtual public IStage {priv typedef TouchStage2D super;pub dCtor(Stage2D);
	// This is used for the transform preupdate logic.
	// This is incremented in some specific instances (if a child or sub...child with the same globalTransformPreupdateIndex has a transform_invalidation occur).
	priv int globalTransformPreupdateIndexCounter = 0;

	priv ListDL<IRenderOrderDrawableNode2D*> renderOrderDrawablesList{LOC};

    pub explicit Stage2D(graphics::IEnvironment2D* environment, IInputHandler* inputHandler);
	prot void createMain() override;

	pub int getGlobalTransformPreupdateIndexCounter() final;
	pub void incrementGlobalTransformPreupdateIndexCounter() final;

	pub void insertRenderOrderDrawable(IRenderOrderDrawableNode2D* drawable) final;
	pub void removeRenderOrderDrawable(IRenderOrderDrawableNode2D* drawable) final;
	priv static void UpdateNextGroupRenderOrderDrawablesWithRenderable(
		ListDL<IRenderOrderDrawableNode2D*>::Entry* startEntry,
		IRenderOrderDrawableNode2D* old_drawableWithRenderable, IRenderOrderDrawableNode2D* new_drawableWithRenderable);
	
	pub int syncStageDrawSpecs(
		const int parentInvalidationBitMap,
		graphics::IRenderer& renderer) final;

	pub ~Stage2D() override;
};
