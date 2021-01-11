#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/InteractiveDrawable2D.h>
#include <graphics/visual/TextureRegion.h>
#include "ISprite2D.h"
#include <graphics/util/GLTypes.h>

class Sprite2D : public InteractiveDrawable2D, virtual public ISprite2D {priv typedef InteractiveDrawable2D super;pub dCtor(Sprite2D);
	pub class LocalInvalidationFlags : public super::InvalidationFlags {
		pub static const unsigned int texture = super::InvalidationFlags::LAST_USED_BIT_INDEX + 1;
		pub static const unsigned int vertices_uv = super::InvalidationFlags::LAST_USED_BIT_INDEX + 2;

		// Sprite2D doesn't need custom optimisation pos,color flags because the vertices are created once and never change count.

		pub static const unsigned int LAST_USED_BIT_INDEX = vertices_uv;
	};

	priv TextureRegion* textureRegion = nullptr;

	// This is always updated for transform and color (even if no texture exists).
	priv graphics::GLTypes::Quad_2DGenericTexture* quad = nullptr;

    pub explicit Sprite2D(TextureRegion* textureRegion);
	pub explicit Sprite2D();
	prot void createMain() override;

	pub const TextureRegion* getTextureRegion() override;
	pub void setTextureRegion(TextureRegion* textureRegion) override;

	pub int syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;
	pub int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;

	prot void onRenderDataHooked() override;

	prot void disposeMain() override;

	priv int sync_vertices_pos();
	priv int sync_vertices_texture();
	priv int sync_vertices_uv();
	priv int sync_vertices_color();

	pub ~Sprite2D() override;
    
};
