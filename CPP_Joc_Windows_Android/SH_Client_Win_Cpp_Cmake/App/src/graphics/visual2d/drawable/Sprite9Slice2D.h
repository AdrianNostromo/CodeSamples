#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/InteractiveDrawable2D.h>
#include <graphics/visual/TextureRegion.h>
#include <graphics/visual2d/drawable/util/AlignTypeH.h>
#include <graphics/visual2d/drawable/util/AlignTypeV.h>
#include "ISprite9Slice2D.h"
#include <graphics/visual/TextureArray.h>
#include <graphics/util/GLTypes.h>
#include <base/list/ListDL.h>
#include <base/MM.h>
#include <base/math/Bounds2D.h>
#include <base/math/Slice9SizePercents.h>

namespace base {
class Sprite9Slice2D : public InteractiveDrawable2D, virtual public ISprite9Slice2D {priv typedef InteractiveDrawable2D super;pub dCtor(Sprite9Slice2D);
    pub class LocalInvalidationFlags : public InvalidationFlags {
        pub static const unsigned int texture = InvalidationFlags::LAST_USED_BIT_INDEX + 1;
        pub static const unsigned int vertices_meshStructure = InvalidationFlags::LAST_USED_BIT_INDEX + 2;

        // These flags are used as a optimisation to avoid the overhead that is from InvalidationFlags::transform,color,alpha.
        pub static const unsigned int vertices_pos = InvalidationFlags::LAST_USED_BIT_INDEX + 3;
        pub static const unsigned int vertices_color = InvalidationFlags::LAST_USED_BIT_INDEX + 4;
        pub static const unsigned int vertices_uv = super::InvalidationFlags::LAST_USED_BIT_INDEX + 2;

        pub static const unsigned int LAST_USED_BIT_INDEX = vertices_uv;
	};
    priv enum RegionBitIndex {
        x0y0=0, x1y0=1, x2y0=2,
        x0y1=3, x1y1=4, x2y1=5,
        x0y2=6, x1y2=7, x2y2=8
    };

    priv bool drawCenterRegion;
    priv TextureRegion* textureRegion = nullptr;

    // NOTE. The total size is computed, no need to use exact 100%.
    priv Slice9SizePercents sliceSizePercents{
        40.0f/*x0*/, 20.0f/*x1*/, 40.0f/*x2*/,
        40.0f/*y0*/, 20.0f/*y1*/, 40.0f/*y2*/ };

    priv Bounds2D innerBounds{ 0.0f/*xMin*/, 0.0f/*yMin*/, 0.0f/*xMax*/, 0.0f/*yMax*/ };

    // This is always updated for transform and color (even if no texture exists).
    priv int quadsCount = 0;
    priv graphics::GLTypes::Quad_2DGenericTexture* quadsArray = nullptr;

    priv int usedRegionsBitMap = 0;
    priv graphics::GLTypes::Quad_2DGenericTexture* regionQuadPointers[9]{nullptr};

    pub explicit Sprite9Slice2D(
        bool drawCenterRegion, TextureRegion* textureRegion=nullptr, Slice9SizePercents* sliceSizePercents=nullptr);
    prot void createMain() override;

    pub const TextureRegion* getTextureRegion() final;
    pub void setTextureRegion(TextureRegion* textureRegion, Slice9SizePercents const* sliceSizePercents=nullptr) final;

    pub Slice9SizePercents const& getSliceSizePercents() final;
    pub void setSliceSizePercents(Slice9SizePercents const& sliceSizePercents) final;
    
    pub bool getDrawCenterRegion() final;
    pub void setDrawCenterRegion(bool drawCenterRegion) final;

    pub Bounds2D const& getInnerBounds() final;
    pub void setInnerBounds(Bounds2D& innerBounds) final;

    pub float getInnerBoundsMinX() final;
    pub void setInnerBoundsMinX(float innerBoundsMinX) final;

    pub float getInnerBoundsMinY() final;
    pub void setInnerBoundsMinY(float innerBoundsMinY) final;

    pub float getInnerBoundsMaxX() final;
    pub void setInnerBoundsMaxX(float innerBoundsMaxX) final;

    pub float getInnerBoundsMaxY() final;
    pub void setInnerBoundsMaxY(float innerBoundsMaxY) final;

    pub int syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;
    pub int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;
    
    pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
    pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

    prot void onRenderDataHooked() override;

    priv int sync_vertices_meshStructure(int& renderData_invalidationBitMap_Vertices, int& renderData_invalidationBitMap_Indices);
    priv int sync_vertices_pos();
    priv static inline void WriteQuadPos(
        graphics::GLTypes::Quad_2DGenericTexture& quad, Affine2& transform,
        float xMin, float yMin, float xMax, float yMax);
    priv int sync_vertices_texture();
    priv int sync_vertices_uv();
    priv static inline void WriteQuadUV(
        graphics::GLTypes::Quad_2DGenericTexture& quad,
        float u, float v, float u2, float v2);
    priv int sync_vertices_color();

    prot void disposeMain() override;
    pub ~Sprite9Slice2D() override;
};
};
