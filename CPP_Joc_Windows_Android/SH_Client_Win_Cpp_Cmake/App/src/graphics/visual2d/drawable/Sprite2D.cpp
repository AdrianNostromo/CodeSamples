#include "Sprite2D.h"
#include <base/math/util/BoolUtil.h>
#include <graphics/visual/TextureArray.h>
#include <base/exceptions/LogicException.h>
#include <graphics/model/RenderableLiteInstanceVI.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/util/GraphicsUtil.h>

Sprite2D::Sprite2D(TextureRegion* textureRegion)
    : super()
{
    this->textureRegion = textureRegion;
}

Sprite2D::Sprite2D()
	:Sprite2D(nullptr)
{
	//void
}

void Sprite2D::createMain() {
    super::createMain();

	quad = new graphics::GLTypes::Quad_2DGenericTexture();

	nodePart->vertices_count = 1 * 4;
	nodePart->verticesUnmanaged = quad;

	nodePart->indicesUnmanaged = graphics::GraphicsUtil::genericIndices;
	nodePart->indices_count = 6;
}

const TextureRegion* Sprite2D::getTextureRegion() {
	return textureRegion;
};

void Sprite2D::setTextureRegion(TextureRegion* textureRegion) {
	if (this->textureRegion == textureRegion) {
		return;
	}

	this->textureRegion = textureRegion;

	if (this->textureRegion != nullptr) {
		invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::texture, LocalInvalidationFlags::vertices_uv));
	} else {
		invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::texture));
	}
}

int Sprite2D::syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	int r = super::syncDrawSpecsPre(combinedInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
		LocalInvalidationFlags::texture))
	{
		// Check if texture nullptr state changed.
		if (textureRegion != nullptr) {
			// Texture set to non-nullptr.
			if (nodePart->indices_count == 0) {
				nodePart->indices_count = 6;

				nodePart->invalidate(0, graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup, 0, 0);
			}
		} else if(textureRegion == nullptr) {
			// Texture set to nullptr.
			if (nodePart->indices_count == 6) {
				// Don't also clear the vertices because no-texture is a rare occurence and there is no need currently to optimise the vertices away.
				nodePart->indices_count = 0;

				nodePart->invalidate(0, graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup, 0, 0);
			}
		}
	}

	return 0;
}

int Sprite2D::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	int r = super::syncDrawSpecs(combinedInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::transform)) {
		if (sync_vertices_pos() != 0) {
			return -1;
		}
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, LocalInvalidationFlags::texture)) {
		if (sync_vertices_texture() != 0) {
			return -2;
		}
	}
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, LocalInvalidationFlags::vertices_uv, LocalInvalidationFlags::texture) && textureRegion != nullptr) {
		// Note. Also update on texture change because uv are not updated if there is no texture.
		if (sync_vertices_uv() != 0) {
			return -2;
		}
	}
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
		InvalidationFlags::alpha,
		InvalidationFlags::tintColor))
	{
		if (sync_vertices_color() != 0) {
			return -3;
		}
	}

	return 0;
}

int Sprite2D::sync_vertices_pos() {
	Affine2 transform = getRenderingTransformAffine2();

    float xMin = 0.0f;
    float yMin = 0.0f;

    float xMax = xMin + getWidth();
    float yMax = yMin + getHeight();

    Vector2 p{};

    // Construct quads.
    // (0.0f, 0.0f)
    p.set(xMin, yMin);
    transform.applyTo(p);
    quad->bl.pos.x = p.x;
    quad->bl.pos.y = p.y;

	// (1.0f, 0.0f)
	p.set(xMax, yMin);
	transform.applyTo(p);
	quad->br.pos.x = p.x;
	quad->br.pos.y = p.y;

    // (1.0f, 1.0f)
    p.set(xMax, yMax);
    transform.applyTo(p);
    quad->tr.pos.x = p.x;
    quad->tr.pos.y = p.y;

	// (0.0f, 1.0f)
	p.set(xMin, yMax);
	transform.applyTo(p);
	quad->tl.pos.x = p.x;
	quad->tl.pos.y = p.y;

	nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

int Sprite2D::sync_vertices_texture() {
	if (textureRegion != nullptr) {
		quad->bl.a_diffuseTextureIndex = textureRegion->getTexture()->packedTextureIndex;
		quad->br.a_diffuseTextureIndex = textureRegion->getTexture()->packedTextureIndex;
		quad->tr.a_diffuseTextureIndex = textureRegion->getTexture()->packedTextureIndex;
		quad->tl.a_diffuseTextureIndex = textureRegion->getTexture()->packedTextureIndex;

		nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);
	}

	return 0;
}

int Sprite2D::sync_vertices_uv() {
	if (textureRegion != nullptr) {
		const TextureRegion* region = textureRegion;
		
		float u = (region != nullptr) ? region->getU() : -1;
		float v = (region != nullptr) ? region->getV() : -1;
		float u2 = (region != nullptr) ? region->getU2() : -1;
		float v2 = (region != nullptr) ? region->getV2() : -1;

		quad->bl.uv.x = u;
		quad->bl.uv.y = v;

		quad->br.uv.x = u2;
		quad->br.uv.y = v;

		quad->tr.uv.x = u2;
		quad->tr.uv.y = v2;

		quad->tl.uv.x = u;
		quad->tl.uv.y = v2;

		nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);
	}

	return 0;
}

int Sprite2D::sync_vertices_color() {
	Color tCol{};

	tCol.set(getGlobalTintColor());
	tCol.a *= getGlobalAlpha();

	quad->bl.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
	quad->bl.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
	quad->bl.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
	quad->bl.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

	quad->br.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
	quad->br.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
	quad->br.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
	quad->br.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

	quad->tr.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
	quad->tr.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
	quad->tr.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
	quad->tr.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

	quad->tl.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
	quad->tl.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
	quad->tl.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
	quad->tl.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

	nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

void Sprite2D::onRenderDataHooked() {
	super::onRenderDataHooked();


	nodePart->invalidate(
		graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup,
		graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup,
		0,
		0
	);
}

void Sprite2D::disposeMain() {
	if (quad != nullptr) {
		delete quad;
		quad = nullptr;
	}

    super::disposeMain();
}

Sprite2D::~Sprite2D() {
    //void
}
