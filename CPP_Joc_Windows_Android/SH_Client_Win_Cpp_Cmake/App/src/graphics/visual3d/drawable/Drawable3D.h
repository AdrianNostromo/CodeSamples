#pragma once

#include <base/gh.h>
#include <graphics/visual3d/drawable/Container3D.h>
#include <graphics/visual3d/drawable/IDrawable3D.h>
#include <base/math/Affine2.h>
#include <base/math/Matrix4.h>
#include <base/geometry/Rectangle.h>
#include <graphics/renderer/IRenderer.h>

namespace graphics {
	class RenderableLiteInstanceVIMN;
	class MADiffuseTexturesArray;
};

class Drawable3D : public Container3D, virtual public IDrawable3D {priv typedef Container3D super;pub dCtor(Drawable3D);
	pub class RenderingPosRoundingType {
		pub static const unsigned int None = 1;
		pub static const unsigned int Round = 2;
		pub static const unsigned int Floor = 3;
		pub static const unsigned int Ceil = 4;
	};

	priv Matrix4 renderingTransform{};
	pub Matrix4 tmpMat4{};

	priv bool isNodePartInRenderer = false;
	prot graphics::RenderableLiteInstanceVIMN* nodePart;
	prot graphics::MADiffuseTexturesArray* maDiffuseTexturesArray = nullptr;

	pub explicit Drawable3D();

	prot Matrix4* getRenderingTransformP() { return &renderingTransform; }

	prot int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer) override;

	priv int syncRenderingTransform();

	pub void onGlobal_isVisibleChanged() override;
	prot void onRootChanged() override;

	priv void releaseRenderData();

	prot void disposeMain() override;
	pub ~Drawable3D() override;
};
