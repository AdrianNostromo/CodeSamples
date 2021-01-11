#include "Sprite9Slice2D.h"
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>
#include <base/math/Math.h>
#include <graphics/model/RenderableLiteInstanceVI.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace base;

Sprite9Slice2D::Sprite9Slice2D(
    bool drawCenterRegion,
    TextureRegion* textureRegion, Slice9SizePercents* sliceSizePercents)
    : super(),
    drawCenterRegion(drawCenterRegion),
    textureRegion(textureRegion)
{
    if (sliceSizePercents != nullptr) {
        this->sliceSizePercents.set(*sliceSizePercents);
    }
}

void Sprite9Slice2D::createMain() {
    super::createMain();

    nodePart->verticesUnmanaged = nullptr;
    nodePart->vertices_count = 0;

    nodePart->indicesUnmanaged = graphics::GraphicsUtil::genericIndices;
    nodePart->indices_count = 0;
}

const TextureRegion* Sprite9Slice2D::getTextureRegion() {
    return textureRegion;
};

void Sprite9Slice2D::setTextureRegion(TextureRegion* textureRegion, Slice9SizePercents const* sliceSizePercents) {
    if (this->textureRegion == textureRegion) {
        return;
    }

    this->textureRegion = textureRegion;
    if (sliceSizePercents != nullptr) {
        this->sliceSizePercents.set(*sliceSizePercents);
    }

    if (this->textureRegion != nullptr) {
        invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::texture, LocalInvalidationFlags::vertices_uv));
    } else {
        invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::texture));
    }
}

Slice9SizePercents const& Sprite9Slice2D::getSliceSizePercents() {
    return sliceSizePercents;
}

void Sprite9Slice2D::setSliceSizePercents(Slice9SizePercents const& sliceSizePercents) {
    if (this->sliceSizePercents.equals(sliceSizePercents)) {
        return;
    }

    this->sliceSizePercents.set(sliceSizePercents);

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::vertices_uv));
}

bool Sprite9Slice2D::getDrawCenterRegion() {
    return drawCenterRegion;
}

void Sprite9Slice2D::setDrawCenterRegion(bool drawCenterRegion) {
    if (this->drawCenterRegion == drawCenterRegion) {
        return;
    }
    
    this->drawCenterRegion = drawCenterRegion;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(
        0,
        LocalInvalidationFlags::vertices_meshStructure, LocalInvalidationFlags::vertices_pos
    ));
}

Bounds2D const& Sprite9Slice2D::getInnerBounds() {
    return innerBounds;
}

void Sprite9Slice2D::setInnerBounds(Bounds2D& innerBounds) {
    if (this->innerBounds.equals(innerBounds)) {
        return;
    }

    this->innerBounds.set(innerBounds);

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(
        0, 
        LocalInvalidationFlags::vertices_meshStructure, LocalInvalidationFlags::vertices_pos
    ));
}

float Sprite9Slice2D::getInnerBoundsMinX() {
    return innerBounds.min.x;
}

void Sprite9Slice2D::setInnerBoundsMinX(float innerBoundsMinX) {
    if (this->innerBounds.min.x == innerBoundsMinX) {
        return;
    }

    this->innerBounds.min.x = innerBoundsMinX;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(
        0,
        LocalInvalidationFlags::vertices_meshStructure, LocalInvalidationFlags::vertices_pos
    ));
}

float Sprite9Slice2D::getInnerBoundsMinY() {
    return innerBounds.min.y;
}

void Sprite9Slice2D::setInnerBoundsMinY(float innerBoundsMinY) {
    if (this->innerBounds.min.y == innerBoundsMinY) {
        return;
    }

    this->innerBounds.min.y = innerBoundsMinY;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(
        0,
        LocalInvalidationFlags::vertices_meshStructure, LocalInvalidationFlags::vertices_pos
    ));
}

float Sprite9Slice2D::getInnerBoundsMaxX() {
    return innerBounds.max.x;
}

void Sprite9Slice2D::setInnerBoundsMaxX(float innerBoundsMaxX) {
    if (this->innerBounds.max.x == innerBoundsMaxX) {
        return;
    }

    this->innerBounds.max.x = innerBoundsMaxX;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(
        0,
        LocalInvalidationFlags::vertices_meshStructure, LocalInvalidationFlags::vertices_pos
    ));
}

float Sprite9Slice2D::getInnerBoundsMaxY() {
    return innerBounds.max.y;
}

void Sprite9Slice2D::setInnerBoundsMaxY(float innerBoundsMaxY) {
    if (this->innerBounds.max.y == innerBoundsMaxY) {
        return;
    }

    this->innerBounds.max.y = innerBoundsMaxY;

    invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(
        0,
        LocalInvalidationFlags::vertices_meshStructure, LocalInvalidationFlags::vertices_pos
    ));
}

int Sprite9Slice2D::syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	int r = super::syncDrawSpecsPre(combinedInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}

    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
        LocalInvalidationFlags::vertices_meshStructure, LocalInvalidationFlags::transform))
    {// Also sync this on transform change because some regions may change because the inner bounds is used on a new outter_size.
        int renderData_invalidationBitMap_Vertices = 0;
        int renderData_invalidationBitMap_Indices = 0;
        if (sync_vertices_meshStructure(renderData_invalidationBitMap_Vertices, renderData_invalidationBitMap_Indices) != 0) {
            return -2;
        }

        if (renderData_invalidationBitMap_Vertices != 0 || renderData_invalidationBitMap_Indices != 0) {
            nodePart->invalidate(
                renderData_invalidationBitMap_Vertices, renderData_invalidationBitMap_Indices,
                0/*invalidationBitMask_ModelTransforms*/, 0/*invalidationBitMask_ModelNormalTransforms*/
            );
        }
    }

	return 0;
}

int Sprite9Slice2D::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	int r = super::syncDrawSpecs(combinedInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}

    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
        InvalidationFlags::transform,
        LocalInvalidationFlags::vertices_pos))
    {
		if (sync_vertices_pos() != 0) {
			return -1;
		}
	}
    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, LocalInvalidationFlags::texture)) {
        if (sync_vertices_texture() != 0) {
            return -2;
        }
    }
    if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, LocalInvalidationFlags::vertices_uv)) {
        // Note. Also update on texture change because uv are not updated if there is no texture.
        if (sync_vertices_uv() != 0) {
            return -2;
        }
    }
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
		InvalidationFlags::alpha,
		InvalidationFlags::tintColor,
		LocalInvalidationFlags::vertices_color))
	{
		if (sync_vertices_color() != 0) {
			return -3;
		}
	}

	return 0;
}

int Sprite9Slice2D::sync_vertices_meshStructure(int& renderData_invalidationBitMap_Vertices, int& renderData_invalidationBitMap_Indices) {
    int new_quadsCount = 0;
    int new_usedRegionsBitMap = 0;
    // This is change to regions count or used regions swap.
    bool isMeshStructureChanged = false;

    // Manually check each region for changes.
    if (sliceSizePercents.x[0] > 0.0f && sliceSizePercents.y[0] >= 0.0f
        && innerBounds.min.x > 0.0f && innerBounds.min.y > 0.0f)
    {// x0y0 region. bottom left
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x0y0);
    }
    if (sliceSizePercents.x[1] > 0.0f && sliceSizePercents.y[0] >= 0.0f
        && innerBounds.getWidth() > 0.0f && innerBounds.min.y > 0.0f)
    {// x1y0 region. center left
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x1y0);
    }
    if (sliceSizePercents.x[2] > 0.0f && sliceSizePercents.y[0] >= 0.0f
        && innerBounds.max.x < getWidth() && innerBounds.min.y > 0.0f)
    {// x2y0 region. top left
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x2y0);
    }

    if (sliceSizePercents.x[0] > 0.0f && sliceSizePercents.y[1] >= 0.0f
        && innerBounds.min.x > 0.0f && innerBounds.getHeight() > 0.0f)
    {// x0y1 region. bottom center
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x0y1);
    }
    if (drawCenterRegion && sliceSizePercents.x[1] > 0.0f && sliceSizePercents.y[1] >= 0.0f
        && innerBounds.getWidth() > 0.0f && innerBounds.getHeight() > 0.0f)
    {// x1y1 region.center center
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x1y1);
    }
    if (sliceSizePercents.x[2] > 0.0f && sliceSizePercents.y[1] >= 0.0f
        && innerBounds.max.x < getWidth() && innerBounds.getHeight() > 0.0f)
    {// x2y1 region. top center
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x2y1);
    }

    if (sliceSizePercents.x[0] > 0.0f && sliceSizePercents.y[2] >= 0.0f
        && innerBounds.min.x > 0.0f && innerBounds.max.y < getHeight())
    {// x0y2 region. bottom right
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x0y2);
    }
    if (sliceSizePercents.x[1] > 0.0f && sliceSizePercents.y[2] >= 0.0f
        && innerBounds.getWidth() > 0.0f && innerBounds.max.y < getHeight())
    {// x1y2 region. center right
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x1y2);
    }
    if (sliceSizePercents.x[2] > 0.0f && sliceSizePercents.y[2] >= 0.0f
        && innerBounds.max.x < getWidth() && innerBounds.max.y < getHeight())
    {// x2y2 region. top right
        new_quadsCount++;
        BoolUtil::setBitAtIndexReference(new_usedRegionsBitMap, RegionBitIndex::x2y2);
    }

    int drawSpecs_invalidationBitMap = 0;

    if (new_quadsCount != quadsCount) {
        // Vertices need full update. Update Sprite9Slice2D speciffic vertex data.
        drawSpecs_invalidationBitMap = BoolUtil::setBitAtIndexDirect(drawSpecs_invalidationBitMap,
            LocalInvalidationFlags::texture,
            LocalInvalidationFlags::vertices_pos,
            LocalInvalidationFlags::vertices_color,
            LocalInvalidationFlags::vertices_uv
        );
    } else if(usedRegionsBitMap != new_usedRegionsBitMap) {
        // Some active,innactive regions swapped place. Update the region speciffic vertex data.
        drawSpecs_invalidationBitMap = BoolUtil::setBitAtIndexDirect(drawSpecs_invalidationBitMap,
            LocalInvalidationFlags::vertices_pos,
            LocalInvalidationFlags::vertices_uv
        );
    }

    if (new_quadsCount != quadsCount) {
        if (quadsArray != nullptr) {
            delete[] quadsArray;

            quadsArray = nullptr;
        }

        quadsCount = new_quadsCount;
        if (quadsCount > 0) {
            quadsArray = new graphics::GLTypes::Quad_2DGenericTexture[quadsCount];
        }
        nodePart->verticesUnmanaged = quadsArray;
        nodePart->vertices_count = quadsCount * 4;
        if (quadsCount > graphics::GraphicsUtil::genericIndices_quadsCount) {
            // The pre-cached indices data from GraphicsUtil has a quads limit that got reached by the text. Increase that list if needed.
            // This should not occur under normal app ussage.
            // Clip the text indices and dispatch a warning.
            nodePart->indices_count = graphics::GraphicsUtil::genericIndices_quadsCount * 6;
            GlobalAccessLogHandler::devLogHandler->post_debug("BitmapLabel tried to use more cached_indices that available, clipping indices to the max amount.");
        } else {
            nodePart->indices_count = quadsCount * 6;
        }

        renderData_invalidationBitMap_Vertices |= graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup;
        renderData_invalidationBitMap_Indices |= graphics::IGeometryProvider_Indices::InvalidationBitGroups::countAndData_bitGroup;
    } else {
        renderData_invalidationBitMap_Vertices |= graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup;
    }

    if (usedRegionsBitMap != new_usedRegionsBitMap) {
        usedRegionsBitMap = new_usedRegionsBitMap;

        int quadsIndex = 0;
        for (int i = 0; i < 9; i++) {
            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, i)) {
                regionQuadPointers[i] = &quadsArray[quadsIndex++];
            } else {
                regionQuadPointers[i] = nullptr;
            }
        }
    }

    if(drawSpecs_invalidationBitMap != 0) {
        invalidateDrawSpecs(drawSpecs_invalidationBitMap);
    }

	return 0;
}

int Sprite9Slice2D::sync_vertices_pos() {
    Affine2 transform = getRenderingTransformAffine2();

    float xMin = 0.0f;
    float yMin = 0.0f;

    float xMax = xMin + getWidth();
    float yMax = yMin + getHeight();

    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x0y0)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x0y0], transform,
            xMin, yMin, innerBounds.min.x, innerBounds.min.y
        );
    }
    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x1y0)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x1y0], transform,
            innerBounds.min.x, yMin, innerBounds.max.x, innerBounds.min.y
        );
    }
    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x2y0)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x2y0], transform,
            innerBounds.max.x, yMin, xMax, innerBounds.min.y
        );
    }
    
    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x0y1)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x0y1], transform,
            xMin, innerBounds.min.y, innerBounds.min.x, innerBounds.max.y
        );
    }
    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x1y1)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x1y1], transform,
            innerBounds.min.x, innerBounds.min.y, innerBounds.max.x, innerBounds.max.y
        );
    }
    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x2y1)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x2y1], transform,
            innerBounds.max.x, innerBounds.min.y, xMax, innerBounds.max.y
        );
    }
    
    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x0y2)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x0y2], transform,
            xMin, innerBounds.max.y, innerBounds.min.x, yMax
        );
    }
    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x1y2)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x1y2], transform,
            innerBounds.min.x, innerBounds.max.y, innerBounds.max.x, yMax
        );
    }
    if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x2y2)) {
        WriteQuadPos(
            *regionQuadPointers[RegionBitIndex::x2y2], transform,
            innerBounds.max.x, innerBounds.max.y, xMax, yMax
        );
    }
    
    nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

    return 0;
}

inline void Sprite9Slice2D::WriteQuadPos(
    graphics::GLTypes::Quad_2DGenericTexture& quad, Affine2& transform,
    float xMin, float yMin, float xMax, float yMax)
{
    Vector2 p{};

    // Construct quads.
    // (0.0f, 0.0f)
    p.set(xMin, yMin);
    transform.applyTo(p);
    quad.bl.pos.x = p.x;
    quad.bl.pos.y = p.y;

    // (1.0f, 0.0f)
    p.set(xMax, yMin);
    transform.applyTo(p);
    quad.br.pos.x = p.x;
    quad.br.pos.y = p.y;

    // (1.0f, 1.0f)
    p.set(xMax, yMax);
    transform.applyTo(p);
    quad.tr.pos.x = p.x;
    quad.tr.pos.y = p.y;

    // (0.0f, 1.0f)
    p.set(xMin, yMax);
    transform.applyTo(p);
    quad.tl.pos.x = p.x;
    quad.tl.pos.y = p.y;
}

int Sprite9Slice2D::sync_vertices_texture() {
    if (textureRegion != nullptr) {
        for (int i = 0; i < quadsCount; i++) {
            graphics::GLTypes::Quad_2DGenericTexture& quad = quadsArray[i];

            quad.bl.a_diffuseTextureIndex = textureRegion->getTexture()->packedTextureIndex;
            quad.br.a_diffuseTextureIndex = textureRegion->getTexture()->packedTextureIndex;
            quad.tr.a_diffuseTextureIndex = textureRegion->getTexture()->packedTextureIndex;
            quad.tl.a_diffuseTextureIndex = textureRegion->getTexture()->packedTextureIndex;
        }

        nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);
    }

    return 0;
}

int Sprite9Slice2D::sync_vertices_uv() {
    if (textureRegion != nullptr) {
        const TextureRegion* region = textureRegion;

        if (region != nullptr) {
            float u = region->getU();
            float v = region->getV();
            float u2 = region->getU2();
            float v2 = region->getV2();

            float u_size = u2 - u;
            float v_size = v2 - v;

            float sliceTotalSizeX = sliceSizePercents.x[0] + sliceSizePercents.x[1] + sliceSizePercents.x[2];
            float sliceTotalSizeY = sliceSizePercents.y[0] + sliceSizePercents.y[1] + sliceSizePercents.y[2];

            Bounds2D innerRegionUV{};
            innerRegionUV.min.set(
                u + u_size * sliceSizePercents.x[0] / sliceTotalSizeX,
                v + v_size * sliceSizePercents.y[0] / sliceTotalSizeY
            );
            innerRegionUV.max.set(
                u + u_size * (sliceSizePercents.x[0] + sliceSizePercents.x[1]) / sliceTotalSizeX,
                v + v_size * (sliceSizePercents.y[0] + sliceSizePercents.y[1]) / sliceTotalSizeY
            );

            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x0y0)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x0y0],
                    u, v, innerRegionUV.min.x, innerRegionUV.min.y
                );
            }
            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x1y0)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x1y0],
                    innerRegionUV.min.x, v, innerRegionUV.max.x, innerRegionUV.min.y
                );
            }
            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x2y0)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x2y0],
                    innerRegionUV.max.x, v, u2, innerRegionUV.min.y
                );
            }

            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x0y1)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x0y1],
                    u, innerRegionUV.min.y, innerRegionUV.min.x, innerRegionUV.max.y
                );
            }
            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x1y1)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x1y1],
                    innerRegionUV.min.x, innerRegionUV.min.y, innerRegionUV.max.x, innerRegionUV.max.y
                );
            }
            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x2y1)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x2y1],
                    innerRegionUV.max.x, innerRegionUV.min.y, u2, innerRegionUV.max.y
                );
            }

            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x0y2)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x0y2],
                    u, innerRegionUV.max.y, innerRegionUV.min.x, v2
                );
            }
            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x1y2)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x1y2],
                    innerRegionUV.min.x, innerRegionUV.max.y, innerRegionUV.max.x, v2
                );
            }
            if (BoolUtil::isBitAtIndex(usedRegionsBitMap, RegionBitIndex::x2y2)) {
                WriteQuadUV(
                    *regionQuadPointers[RegionBitIndex::x2y2],
                    innerRegionUV.max.x, innerRegionUV.max.y, u2, v2
                );
            }
        } else {
            for (int i = 0; i < quadsCount; i++) {
                graphics::GLTypes::Quad_2DGenericTexture& quad = quadsArray[i];

                quadsArray[0].bl.uv.x = -1;
                quadsArray[0].bl.uv.y = -1;

                quadsArray[0].br.uv.x = -1;
                quadsArray[0].br.uv.y = -1;

                quadsArray[0].tr.uv.x = -1;
                quadsArray[0].tr.uv.y = -1;

                quadsArray[0].tl.uv.x = -1;
                quadsArray[0].tl.uv.y = -1;
            }
        }
        nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);
    }

    return 0;
}

inline void Sprite9Slice2D::WriteQuadUV(
    graphics::GLTypes::Quad_2DGenericTexture& quad,
    float u, float v, float u2, float v2)
{
    quad.bl.uv.x = u;
    quad.bl.uv.y = v;

    quad.br.uv.x = u2;
    quad.br.uv.y = v;

    quad.tr.uv.x = u2;
    quad.tr.uv.y = v2;

    quad.tl.uv.x = u;
    quad.tl.uv.y = v2;
}

int Sprite9Slice2D::sync_vertices_color() {
    Color tCol{};

	tCol.set(getGlobalTintColor());
	tCol.a *= getGlobalAlpha();

	for(int i=0;i<quadsCount;i++) {
        graphics::GLTypes::Quad_2DGenericTexture& quad = quadsArray[i];

        quad.bl.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
        quad.bl.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
        quad.bl.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
        quad.bl.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

        quad.br.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
        quad.br.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
        quad.br.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
        quad.br.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

        quad.tr.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
        quad.tr.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
        quad.tr.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
        quad.tr.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);

        quad.tl.diffuseColor.r = static_cast<unsigned char>(tCol.r * 255.0f);
        quad.tl.diffuseColor.g = static_cast<unsigned char>(tCol.g * 255.0f);
        quad.tl.diffuseColor.b = static_cast<unsigned char>(tCol.b * 255.0f);
        quad.tl.diffuseColor.a = static_cast<unsigned char>(tCol.a * 255.0f);
    }

    nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

void Sprite9Slice2D::onRenderDataHooked() {
	super::onRenderDataHooked();

    nodePart->invalidate(
        graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup, 
        graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup,
        0,
        0
    );
}

std::shared_ptr<base::IWrappedValue> Sprite9Slice2D::getPropertyValue(std::string property) {
    if(property == "drawCenterRegion") {
        return base::IWrappedValue::new_bool(drawCenterRegion);
    }
    
    else if(property == "innerBoundsMinX") {
        return base::IWrappedValue::new_float(innerBounds.min.x);
    }else if(property == "innerBoundsMinY") {
        return base::IWrappedValue::new_float(innerBounds.min.y);
    }else if(property == "innerBoundsMaxX") {
        return base::IWrappedValue::new_float(innerBounds.max.x);
    }else if(property == "innerBoundsMaxY") {
        return base::IWrappedValue::new_float(innerBounds.max.y);
    }

    return super::getPropertyValue(property);
}

void Sprite9Slice2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
    if(property == "drawCenterRegion") {
        setDrawCenterRegion(value->getDirectAs_bool());
    }
    
    else if(property == "innerBoundsMinX") {
        setInnerBoundsMinX(value->getDirectAs_float());
    }else if(property == "innerBoundsMinY") {
        setInnerBoundsMinY(value->getDirectAs_float());
    }else if(property == "innerBoundsMaxX") {
        setInnerBoundsMaxX(value->getDirectAs_float());
    }else if(property == "innerBoundsMaxY") {
        setInnerBoundsMaxY(value->getDirectAs_float());
    }

    else {
        super::setPropertyValue(property, value);
    }
}

void Sprite9Slice2D::disposeMain() {
	//void

    super::disposeMain();
}

Sprite9Slice2D::~Sprite9Slice2D() {
    if(quadsArray != nullptr) {
        delete[] quadsArray;

        quadsArray = nullptr;
    }
}
