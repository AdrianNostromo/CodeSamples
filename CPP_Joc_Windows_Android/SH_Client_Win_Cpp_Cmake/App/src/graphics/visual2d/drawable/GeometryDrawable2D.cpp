#include "GeometryDrawable2D.h"
#include <graphics/model/RenderableLiteInstanceVI.h>
#include <graphics/renderer/renderablesManager/IRenderablesManager.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/renderer/IRenderer.h>
#include <base/math/util/BoolUtil.h>

GeometryDrawable2D::GeometryDrawable2D()
	: super()
{
	nodePart = new graphics::RenderableLiteInstanceVI();
	nodePart->initVertexAttributesList(graphics::GraphicsUtil::VertexAttributesList_2DGenericTexture);
	nodePart->setMaterial(graphics::GraphicsUtil::material_2DGenericTexture);
	maDiffuseTexturesArray = nodePart->getMaterial()->getMustExistAs<graphics::MADiffuseTexturesArray*>(graphics::MADiffuseTexturesArray::TypeBitGroup);
}

IDrawableNode2D* GeometryDrawable2D::getDisplayTreeMinLeafDrawableWithRenderData() {
    if (getInParent_drawablesWithRenderDataListEntryRef() == nullptr) {
        // This function is never called unless the drawable is in the list_with_render_data.
        throw LogicException(LOC);
    }

    return this;
}

IDrawableNode2D* GeometryDrawable2D::getDisplayTreeMaxLeafDrawableWithRenderData() {
    if (getInParent_drawablesWithRenderDataListEntryRef() == nullptr) {
        // This function is never called unless the drawable is in the list_with_render_data.
        throw LogicException(LOC);
    }

    return this;
}

int GeometryDrawable2D::syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
    int r = super::syncDrawSpecsPre(combinedInvalidationBitMap, renderer, preRenderData);
    if (r != 0) {
        return r;
    }

    // Don't check if vertices or indices exist because the drawable should be in the renderer if has a root and is globally_visible.
    if (!isNodePartInRenderer && getRoot() != nullptr && getGlobal_isVisible()) {
        if (getParent() == nullptr || getRoot() == nullptr) {
            // This function must always be called when a parent and root are available.
            throw LogicException(LOC);
        }
        IDrawableNode2D* insertAfterDrawable = getParent()->onChildAddingToRendererAndGetPrevDrawableWithRenderData(this);
        graphics::IRenderable* insertAfterTarget = nullptr;
        if (insertAfterDrawable != nullptr) {
            insertAfterTarget = insertAfterDrawable->getRenderData();
            if (insertAfterTarget == nullptr) {
                throw LogicException(LOC);
            }
        }
        
        renderer.getRenderablesManager()->insertAfter(nodePart/*nodePart*/, insertAfterTarget/*insertAfterTarget*/);
        isNodePartInRenderer = true;

        onRenderDataHooked();
    }

    return 0;
}

int GeometryDrawable2D::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
    int r = super::syncDrawSpecs(combinedInvalidationBitMap, renderer, preRenderData);
    if (r != 0) {
        return r;
    }

    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::transform)) {
        if (syncRenderingTransform() != 0) {
            return -1;
        }
    }

    return 0;
}

int GeometryDrawable2D::syncRenderingTransform() {
    renderingTransformAffine2.set(getGlobalTransform(false));
    syncRenderingTransformSizing(renderingTransformAffine2);
    
    if (renderingPosRoundingType != RenderingPosRoundingType::None) {
        Vector2 translation;
        renderingTransformAffine2.getTranslation(translation);

        if (renderingPosRoundingType == RenderingPosRoundingType::Round) {
            translation.x = Math::round(translation.x);
            translation.y = Math::round(translation.y);
        } else if (renderingPosRoundingType == RenderingPosRoundingType::Floor) {
            translation.x = Math::floor(translation.x);
            translation.y = Math::floor(translation.y);
        } else if (renderingPosRoundingType == RenderingPosRoundingType::Ceil) {
            translation.x = Math::ceil(translation.x);
            translation.y = Math::ceil(translation.y);
        } else {
            assert(false);
        }

        renderingTransformAffine2.m02 = translation.x;
        renderingTransformAffine2.m12 = translation.y;
    }

    renderingTransformMatrix4.set(renderingTransformAffine2);

    return 0;
}

void GeometryDrawable2D::syncRenderingTransformSizing(Affine2& renderingTransformAffine2) {
    //void
}


void GeometryDrawable2D::onRootChanged() {
    super::onRootChanged();

    if (this->getRoot() && getIsVisible()) {
        //void
    } else {
        if (isNodePartInRenderer) {
            // Need to do this release in root removed becase the root change indicates removal from the display tree
            /// and parent removal doesn't occur on cases when the parent of the parent is removed from it's parent.
            releaseRenderData();
        }
    }
}

void GeometryDrawable2D::onGlobal_isVisibleChanged() {
    super::onGlobal_isVisibleChanged();

    if (!getGlobal_isVisible() && isNodePartInRenderer) {
        releaseRenderData();
    }
}

void GeometryDrawable2D::onRenderDataHooked() {
    //void
}

void GeometryDrawable2D::releaseRenderData() {
    if (isNodePartInRenderer) {
        isNodePartInRenderer = false;

        nodePart->removeFromRenderer();

        if (getParent() == nullptr) {
            // This function must always be called when a parent is available.
            /// Note. The root is probably already removed.
            throw LogicException(LOC);
        }
        getParent()->onChildRemovedFromRenderer(this);
    }
}

graphics::RenderableLiteInstanceVI* GeometryDrawable2D::getNodePart() {
    return nodePart;
}

graphics::RenderableLiteInstanceVI* GeometryDrawable2D::getRenderData() {
    return nodePart;
}

bool GeometryDrawable2D::getHasRenderable() {
    return true;
}

void GeometryDrawable2D::disposeMain() {
    releaseRenderData();

    if (nodePart != nullptr) {
        nodePart->reservedDisposeMain();
        delete nodePart;
        nodePart = nullptr;
    }

    super::disposeMain();
}

GeometryDrawable2D::~GeometryDrawable2D() {
	//void
}
