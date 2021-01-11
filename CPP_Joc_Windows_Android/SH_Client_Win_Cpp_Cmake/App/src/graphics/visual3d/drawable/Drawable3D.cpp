#include "Drawable3D.h"
#include <base/math/util/BoolUtil.h>
#include <assert.h>
#include <base/geometry/Circle.h>
#include <base/math/Math.h>
#include <base/exceptions/LogicException.h>
#include <graphics/model/RenderableLiteInstanceVIMN.h>
#include <graphics/model/RenderableLiteInstanceVIMN.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/renderer/IRenderer.h>
#include <graphics/renderer/renderablesManager/IRenderablesManager.h>

Drawable3D::Drawable3D()
	:super()
{
	nodePart = new graphics::RenderableLiteInstanceVIMN();
	nodePart->initVertexAttributesList(graphics::GraphicsUtil::VertexAttributesList_3DGenericTexture);
	nodePart->setMaterial(graphics::GraphicsUtil::material_3DGenericTexture_solid);
	maDiffuseTexturesArray = nodePart->getMaterial()->getMustExistAs<graphics::MADiffuseTexturesArray*>(graphics::MADiffuseTexturesArray::TypeBitGroup);
}

int Drawable3D::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer) {
	int r = super::syncDrawSpecs(combinedInvalidationBitMap, renderer);
	if (r != 0) {
		return r;
	}

	if (!isNodePartInRenderer) {
		renderer.getRenderablesManager()->add(nodePart);
		isNodePartInRenderer = true;

		nodePart->invalidate(
			graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup, graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup,
			graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup, graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup
		);

		onRenderDataHooked();
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::transform)) {
		if (syncRenderingTransform() != 0) {
			return -1;
		}
	}

	return 0;
}

int Drawable3D::syncRenderingTransform() {
	renderingTransform.set(getGlobalTransform(false));

	nodePart->getWorldTransformMustExist()->set(renderingTransform);

	nodePart->getNormalWorldTransformMustExist()->set(*tmpMat4.set(renderingTransform)->invUnchecked()->transpose());

	nodePart->invalidate(
		0, 0,
		graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup
	);

	return 0;
}

void Drawable3D::onGlobal_isVisibleChanged() {
	super::onGlobal_isVisibleChanged();

	if (!getGlobal_isVisible() && isNodePartInRenderer) {
		releaseRenderData();
	}
}

void Drawable3D::onRootChanged() {
	super::onRootChanged();

	if (this->getRoot() && getIsVisible()) {
		//void
	} else {
		if (isNodePartInRenderer) {
			releaseRenderData();
		}
	}
}

void Drawable3D::releaseRenderData() {
	if (isNodePartInRenderer) {
		isNodePartInRenderer = false;

		nodePart->removeFromRenderer();
	}
}

void Drawable3D::disposeMain() {
	releaseRenderData();

	super::disposeMain();
}

Drawable3D::~Drawable3D() {
	//void
}
