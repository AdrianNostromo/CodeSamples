#pragma once

#include "IStage3D.h"
#include <graphics/visual3d/drawable/Container3D.h>
#include <graphics/visual3d/drawable/IRoot3D.h>

class Stage3D : public Container3D, virtual public IStage3D, virtual public IRoot3D {
private: typedef Container3D super;
private:
	// This is used for the transform preupdate logic.
	// This is incremented in some specific instances (if a child or sub...child with the same globalTransformPreupdateIndex has a transform_invalidation occur).
	int globalTransformPreupdateIndexCounter = 0;

public:
	Stage3D(Stage3D const&) = delete;
	Stage3D(Stage3D &&) = default;
	Stage3D& operator=(Stage3D const&) = delete;
	Stage3D& operator=(Stage3D &&) = default;

	explicit Stage3D();

	int getGlobalTransformPreupdateIndexCounter() override;
	void incrementGlobalTransformPreupdateIndexCounter() override;

	pub int syncStageDrawSpecs(
		const int parentInvalidationBitMap,
		graphics::IRenderer& renderer) final;

	~Stage3D() override;
};
