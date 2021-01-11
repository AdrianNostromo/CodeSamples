#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/GeometryDrawable2D.h>
#include "ITriangles2DDrawable.h"
#include <graphics/util/GLTypes.h>
#include <base/memory/SharedPointer.h>

class Triangles2DDrawable : public GeometryDrawable2D, virtual public ITriangles2DDrawable {priv typedef GeometryDrawable2D super;pub dCtor(Triangles2DDrawable);
	pub class LocalInvalidationFlags : public InvalidationFlags {
		pub static const unsigned int localTrianglesContainer = InvalidationFlags::LAST_USED_BIT_INDEX + 1;

		pub static const unsigned int triangles_transform = InvalidationFlags::LAST_USED_BIT_INDEX + 2;
		pub static const unsigned int triangles_tintColor = InvalidationFlags::LAST_USED_BIT_INDEX + 3;
		pub static const unsigned int triangles_texture = InvalidationFlags::LAST_USED_BIT_INDEX + 4;
		pub static const unsigned int triangles_uv = super::InvalidationFlags::LAST_USED_BIT_INDEX + 5;
		pub static const unsigned int triangles_indices = InvalidationFlags::LAST_USED_BIT_INDEX + 6;

		pub static const unsigned int LAST_USED_BIT_INDEX = triangles_indices;
	};

	priv Texture* texture = nullptr;

	// Vertices are counter clockwise.
	priv sp<Array1D<LocalVertex>> localVerticesManaged = nullptr;
	// Indices are counter clockwise.
	priv sp<Array1D<unsigned short>> localIndicesManaged = nullptr;

    pub explicit Triangles2DDrawable(Texture* texture);
	pub explicit Triangles2DDrawable();
	prot void createMain() override;

	pub const Texture* getTexture() override;
	pub void setTexture(Texture* texture) override;

	pub sp<Array1D<LocalVertex>> getLocalVerticesManaged();
	pub LocalVertex& getLocalVertex(int vertexIndex);
	pub sp<Array1D<unsigned short>> getLocalIndicesManaged();
	pub void setLocalGeometry(sp<Array1D<LocalVertex>> localVerticesManaged, sp<Array1D<unsigned short>> localIndicesManaged);
	pub void localGeometryDataChanged();

	pub int syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;
	pub int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) override;

	prot void onRenderDataHooked() override;

	priv int syncVertices_localTrianglesContainer(int& renderData_invalidationBitMap_Vertices, int& renderData_invalidationBitMap_Indices);
	priv int syncVertices_transform();
	priv int syncVertices_texture();
	priv int syncVertices_uv();
	priv int syncVertices_color();
	priv int sync_indices();

	prot void disposeMain() override;
	pub ~Triangles2DDrawable() override;
    
};
