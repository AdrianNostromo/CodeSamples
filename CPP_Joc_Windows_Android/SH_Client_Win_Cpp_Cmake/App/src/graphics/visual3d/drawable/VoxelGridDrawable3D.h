#pragma once

#include <graphics/visual3d/drawable/Drawable3D.h>
#include <graphics/visual/TextureRegion.h>
#include <graphics/renderer/IRenderer.h>
#include "VoxlesGrid3DGeometry.h"
#include <base/visual3d/IManagedDynamicVisual.h>

class VoxelGridDrawable3D : public Drawable3D, virtual public VoxlesGrid3DGeometry::IHandler, virtual public IManagedDynamicVisual {priv typedef Drawable3D super;pub dCtor(VoxelGridDrawable3D);
	pub class VoxelGridDrawable3DInvalidationFlags : public InvalidationFlags {
		pub static const unsigned int mesh = InvalidationFlags::LAST_USED_BIT_INDEX + 1;

		pub static const unsigned int LAST_USED_BIT_INDEX = mesh;
	};

	priv VoxlesGrid3DGeometry voxelsGrid;

	pub explicit VoxelGridDrawable3D();
	pub void reservedCreate() final;

	pub int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer) override;

	pub void onGeometryInvalidation() final;

	pub void updateVisual() override;

	pub VoxlesGrid3DGeometry* peekVoxelsGrid() { return &voxelsGrid; }
	pub VoxlesGrid3DGeometry* getVoxelsGrid() { return &voxelsGrid; }

	prot virtual void createMain();

	prot void onRenderDataHooked() override;

	prot void disposeMain() override ;
	
	priv int syncModelWorldTransform();
	priv int syncVertices_mesh();

	pub ~VoxelGridDrawable3D() override;
};
