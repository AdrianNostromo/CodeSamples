#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/DrawableNode2D.h>
#include "IGeometryDrawable2D.h"
#include <base/math/Affine2.h>
#include <base/math/Matrix4.h>

namespace graphics {
    class RenderableLiteInstanceVI;
    class MADiffuseTexturesArray;
};

class GeometryDrawable2D : public DrawableNode2D, public virtual IGeometryDrawable2D {priv typedef DrawableNode2D super;pub dCtor(GeometryDrawable2D);
    pub class RenderingPosRoundingType {
        pub static const unsigned int None = 1;
        pub static const unsigned int Round = 2;
        pub static const unsigned int Floor = 3;
        pub static const unsigned int Ceil = 4;
    };

    priv Affine2 renderingTransformAffine2{};
    priv Matrix4 renderingTransformMatrix4{};

    //asdC;// maybe also implement this for size;
    prot unsigned int renderingPosRoundingType = RenderingPosRoundingType::None;

    priv bool isNodePartInRenderer = false;
    prot graphics::RenderableLiteInstanceVI* nodePart;
    prot graphics::MADiffuseTexturesArray* maDiffuseTexturesArray = nullptr;

    pub explicit GeometryDrawable2D();

    prot Affine2& getRenderingTransformAffine2() { return renderingTransformAffine2; }
    prot Matrix4& getRenderingTransformMatrix4() { return renderingTransformMatrix4; }
    
    pub IDrawableNode2D* getDisplayTreeMinLeafDrawableWithRenderData() final;
    pub IDrawableNode2D* getDisplayTreeMaxLeafDrawableWithRenderData() final;

    prot int syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;
    prot virtual int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;

    priv int syncRenderingTransform();
    // This is used to apply anchor logic for sized drawables.
    prot virtual void syncRenderingTransformSizing(Affine2& renderingTransformAffine2);
    prot void onRootChanged() override;

    pub void onGlobal_isVisibleChanged() override;

    prot virtual void onRenderDataHooked();
    // Keep this private if possible. The reason is the mechanism used to control render order probably uses this if it didn't change.
    priv void releaseRenderData();

    pub graphics::RenderableLiteInstanceVI* getNodePart();

    pub graphics::RenderableLiteInstanceVI* getRenderData() final;

    pub bool getHasRenderable() override;

    prot void disposeMain() override;
    pub ~GeometryDrawable2D() override;
};
