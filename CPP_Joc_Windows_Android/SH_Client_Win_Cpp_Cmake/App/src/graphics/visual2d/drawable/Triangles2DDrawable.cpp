#include "Triangles2DDrawable.h"
#include <base/math/util/BoolUtil.h>
#include <graphics/visual/TextureArray.h>
#include <base/exceptions/LogicException.h>
#include <graphics/model/RenderableLiteInstanceVI.h>
#include <graphics/material/attributeTypes/MADiffuseTexturesArray.h>
#include <graphics/util/GraphicsUtil.h>
#include <graphics/visual/Texture.h>

Triangles2DDrawable::Triangles2DDrawable(Texture* texture)
    : super(),
	texture(texture)
{
    //void
}

Triangles2DDrawable::Triangles2DDrawable()
	:Triangles2DDrawable(nullptr)
{
	//void
}

void Triangles2DDrawable::createMain() {
    super::createMain();

	nodePart->verticesUnmanaged = nullptr;
	nodePart->vertices_count = 0;

	nodePart->indicesUnmanaged = nullptr;
	nodePart->indices_count = 0;
}

const Texture* Triangles2DDrawable::getTexture() {
	return texture;
};

void Triangles2DDrawable::setTexture(Texture* texture) {
	if (this->texture == texture) {
		return;
	}

	this->texture = texture;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::triangles_texture));
}

sp<Array1D<ITriangles2DDrawable::LocalVertex>> Triangles2DDrawable::getLocalVerticesManaged() {
	return localVerticesManaged;
}

ITriangles2DDrawable::LocalVertex& Triangles2DDrawable::getLocalVertex(int vertexIndex) {
	if (localVerticesManaged == nullptr || localVerticesManaged->count() <= vertexIndex) {
		throw LogicException(LOC);
	}

	return localVerticesManaged->getReference(vertexIndex);
}

sp<Array1D<unsigned short>> Triangles2DDrawable::getLocalIndicesManaged() {
	return localIndicesManaged;
}

void Triangles2DDrawable::setLocalGeometry(sp<Array1D<LocalVertex>> localVerticesManaged, sp<Array1D<unsigned short>> localIndicesManaged) {
	this->localVerticesManaged = localVerticesManaged;
	this->localIndicesManaged = localIndicesManaged;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, LocalInvalidationFlags::localTrianglesContainer));
}

void Triangles2DDrawable::localGeometryDataChanged() {
	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0, 
		LocalInvalidationFlags::triangles_tintColor, 
		LocalInvalidationFlags::triangles_transform, 
		LocalInvalidationFlags::triangles_uv,
		LocalInvalidationFlags::triangles_indices)
	);
}

int Triangles2DDrawable::syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	int r = super::syncDrawSpecsPre(combinedInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
		LocalInvalidationFlags::localTrianglesContainer))
	{
        int renderData_invalidationBitMap_Vertices = 0;
        int renderData_invalidationBitMap_Indices = 0;
        if (syncVertices_localTrianglesContainer(renderData_invalidationBitMap_Vertices, renderData_invalidationBitMap_Indices) != 0) {
			return -2;
		}

        if (renderData_invalidationBitMap_Vertices != 0 || renderData_invalidationBitMap_Indices != 0) {
            nodePart->invalidate(renderData_invalidationBitMap_Vertices, renderData_invalidationBitMap_Indices, 0, 0);
        }
	}

	return 0;
}

int Triangles2DDrawable::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	int r = super::syncDrawSpecs(combinedInvalidationBitMap, renderer, preRenderData);
	if (r != 0) {
		return r;
	}
	
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::transform, LocalInvalidationFlags::triangles_transform)) {
		if (syncVertices_transform() != 0) {
			return -1;
		}
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, LocalInvalidationFlags::triangles_texture)) {
		if (syncVertices_texture() != 0) {
			return -2;
		}
	}
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, LocalInvalidationFlags::triangles_uv, LocalInvalidationFlags::triangles_texture)) {
		// Note. Also update on texture change because uv are not updated if there is no texture.
		if (syncVertices_uv() != 0) {
			return -2;
		}
	}
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap,
		InvalidationFlags::alpha,
		InvalidationFlags::tintColor,
        LocalInvalidationFlags::triangles_tintColor))
	{
		if (syncVertices_color() != 0) {
			return -3;
		}
	}
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, LocalInvalidationFlags::triangles_indices)) {
		if (sync_indices() != 0) {
			return -2;
		}
	}

	return 0;
}

int Triangles2DDrawable::syncVertices_localTrianglesContainer(int& renderData_invalidationBitMap_Vertices, int& renderData_invalidationBitMap_Indices) {
    int localVerticesCount = localVerticesManaged != nullptr ? localVerticesManaged->count() : 0;
    int localIndicesCount = localIndicesManaged != nullptr ? localIndicesManaged->count() : 0;

    if(nodePart->vertices_count != localVerticesCount || nodePart->indices_count != localIndicesCount) {
		if (nodePart->vertices_count != localVerticesCount) {
			if (nodePart->verticesUnmanaged != nullptr) {
				delete[] static_cast<graphics::GLTypes::Vertex_2DGenericTexture*>(nodePart->verticesUnmanaged);

				nodePart->verticesUnmanaged = nullptr;
				nodePart->vertices_count = 0;
			}
			if (localVerticesCount > 0) {
				nodePart->verticesUnmanaged = new graphics::GLTypes::Vertex_2DGenericTexture[localVerticesCount];
				nodePart->vertices_count = localVerticesCount;
			}

			renderData_invalidationBitMap_Vertices |= graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup;
		}
		
		if (nodePart->indices_count != localIndicesCount) {
			if (nodePart->indicesUnmanaged != nullptr) {
				delete[] nodePart->indicesUnmanaged;

				nodePart->indicesUnmanaged = nullptr;
				nodePart->indices_count = 0;
			}
			if (localIndicesCount > 0) {
				nodePart->indicesUnmanaged = new unsigned short[localIndicesCount];
				nodePart->indices_count = localIndicesCount;
			}

			renderData_invalidationBitMap_Indices |= graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup;
		}
    } else {
		renderData_invalidationBitMap_Vertices |= graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup;
    }

	if (localVerticesCount > 0 && localIndicesCount > 0) {
		invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
			LocalInvalidationFlags::triangles_tintColor,
			LocalInvalidationFlags::triangles_transform,
			LocalInvalidationFlags::triangles_uv,
			LocalInvalidationFlags::triangles_indices)
		);
	}

    return 0;
}

int Triangles2DDrawable::syncVertices_transform() {
	if (nodePart->vertices_count <= 0) {
		return 0;
	}
	if (localVerticesManaged == nullptr) {
		throw LogicException(LOC);
	}

	Affine2& transform = getRenderingTransformAffine2();

	int count = localVerticesManaged->count();
	LocalVertex* localVertices = localVerticesManaged->data;
	graphics::GLTypes::Vertex_2DGenericTexture* glVertices = static_cast<graphics::GLTypes::Vertex_2DGenericTexture*>(nodePart->verticesUnmanaged);
	for (int i = 0; i < count; i++) {
		LocalVertex& localVertex = localVertices[i];

		Vector2 p{ localVertex.pos.x, localVertex.pos.y };

		transform.applyTo(p);
		glVertices[i].pos.x = p.x;
		glVertices[i].pos.y = p.y;
	}

	nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

int Triangles2DDrawable::syncVertices_texture() {
	if (nodePart->vertices_count <= 0) {
		return 0;
	}
	if (localVerticesManaged == nullptr) {
		throw LogicException(LOC);
	}

	// Set texture index to -1 if unused.
	int textureIndex = texture != nullptr ? texture->packedTextureIndex : -1;

	int count = localVerticesManaged->count();
	LocalVertex* localVertices = localVerticesManaged->data;
	graphics::GLTypes::Vertex_2DGenericTexture* glVertices = static_cast<graphics::GLTypes::Vertex_2DGenericTexture*>(nodePart->verticesUnmanaged);
	for (int i = 0; i < count; i++) {
		LocalVertex& localVertex = localVertices[i];

		// This is checked in the shader in a efficient way with the new logic, no need to check here.
		//if (localVertex.uv.x >=0 && localVertex.uv.y >= 0 && textureIndex < 0) {
		//	// Using uv without a texture will cause a opengl error.
		//	throw LogicException(LOC);
		//}

		glVertices[i].a_diffuseTextureIndex = textureIndex;
	}

	nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

int Triangles2DDrawable::syncVertices_uv() {
	if (nodePart->vertices_count <= 0) {
		return 0;
	}
	if (localVerticesManaged == nullptr) {
		throw LogicException(LOC);
	}

	int count = localVerticesManaged->count();
	LocalVertex* localVertices = localVerticesManaged->data;
	graphics::GLTypes::Vertex_2DGenericTexture* glVertices = static_cast<graphics::GLTypes::Vertex_2DGenericTexture*>(nodePart->verticesUnmanaged);
	for (int i = 0; i < count; i++) {
		LocalVertex& localVertex = localVertices[i];

		// This is checked in the shader in a efficient way with the new logic, no need to check here.
		//if (localVertex.uv.x >=0 && localVertex.uv.y >= 0 && texture == nullptr) {
		//	// Using uv without a texture will cause a opengl error.
		//	throw LogicException(LOC);
		//}

		glVertices[i].uv.x = localVertex.uv.x;
		glVertices[i].uv.y = localVertex.uv.y;
	}

	nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

int Triangles2DDrawable::syncVertices_color() {
	if (nodePart->vertices_count <= 0) {
		return 0;
	}
	if (localVerticesManaged == nullptr) {
		throw LogicException(LOC);
	}

	float globalAlpha = getGlobalAlpha();

	int count = localVerticesManaged->count();
	LocalVertex* localVertices = localVerticesManaged->data;
	graphics::GLTypes::Vertex_2DGenericTexture* glVertices = static_cast<graphics::GLTypes::Vertex_2DGenericTexture*>(nodePart->verticesUnmanaged);
	for (int i = 0; i < count; i++) {
		LocalVertex& localVertex = localVertices[i];

		glVertices[i].diffuseColor.r = static_cast<unsigned char>(localVertex.tintColor.r * 255.0f);
		glVertices[i].diffuseColor.g = static_cast<unsigned char>(localVertex.tintColor.g * 255.0f);
		glVertices[i].diffuseColor.b = static_cast<unsigned char>(localVertex.tintColor.b * 255.0f);
		glVertices[i].diffuseColor.a = static_cast<unsigned char>((localVertex.tintColor.a * globalAlpha) * 255.0f);
	}

	nodePart->invalidate(graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0, 0);

	return 0;
}

int Triangles2DDrawable::sync_indices() {
	if (nodePart->indices_count <= 0) {
		return 0;
	}
	if (localIndicesManaged == nullptr) {
		throw LogicException(LOC);
	}

	memcpy(nodePart->indicesUnmanaged, localIndicesManaged->data, nodePart->indices_count * sizeof(unsigned short));

	nodePart->invalidate(0, graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, 0, 0);

	return 0;
}

void Triangles2DDrawable::onRenderDataHooked() {
	super::onRenderDataHooked();

	nodePart->invalidate(
		graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup,
		graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup,
		0,
		0
	);
}

void Triangles2DDrawable::disposeMain() {
	if (nodePart->verticesUnmanaged != nullptr) {
		delete[] static_cast<graphics::GLTypes::Vertex_2DGenericTexture*>(nodePart->verticesUnmanaged);

		nodePart->verticesUnmanaged = nullptr;
		nodePart->vertices_count = 0;
	}
	if (nodePart->indicesUnmanaged != nullptr) {
		delete[] nodePart->indicesUnmanaged;

		nodePart->indicesUnmanaged = nullptr;
		nodePart->indices_count = 0;
	}

    super::disposeMain();
}

Triangles2DDrawable::~Triangles2DDrawable() {
    //void
}
