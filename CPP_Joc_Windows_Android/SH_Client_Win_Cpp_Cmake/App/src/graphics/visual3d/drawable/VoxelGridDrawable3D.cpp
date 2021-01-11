#include "VoxelGridDrawable3D.h"
#include <base/math/util/BoolUtil.h>
#include <graphics/model/RenderableLiteInstanceVIMN.h>

VoxelGridDrawable3D::VoxelGridDrawable3D()
	:Drawable3D(), voxelsGrid(this)
{
	//void
}

int VoxelGridDrawable3D::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer) {
	int r = Drawable3D::syncDrawSpecs(combinedInvalidationBitMap, renderer);
	if (r != 0) {
		return r;
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::transform)) {
		if (syncModelWorldTransform() != 0) {
			return -1;
		}
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, VoxelGridDrawable3DInvalidationFlags::mesh)) {
		if (syncVertices_mesh() != 0) {
			return -1;
		}
	}

	return 0;
}

int VoxelGridDrawable3D::syncModelWorldTransform() {
	// Update already occured and the rendering data has a pointer to the transform. Just invalidate.

	nodePart->invalidate(
		0, 0,
		graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup, graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup
	);
	
	return 0;
}

int VoxelGridDrawable3D::syncVertices_mesh() {
	int oldVerticesCount = voxelsGrid.getVerticesCount();
	int oldIndicesCount = voxelsGrid.getIndicesCount();
	
	voxelsGrid.updateGeometry();

	int invalidationBitMask_Vertices = 0;
	if (voxelsGrid.getVerticesCount() != oldVerticesCount) {
		nodePart->verticesUnmanaged = voxelsGrid.getVerticesArray();
		nodePart->vertices_count = voxelsGrid.getVerticesCount();
		
		invalidationBitMask_Vertices = graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup;
	} else {
		invalidationBitMask_Vertices = graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup;
	}
	
	int invalidationBitMask_Indices = 0;
	if (voxelsGrid.getIndicesCount() != oldIndicesCount) {
		nodePart->indicesUnmanaged = voxelsGrid.getIndicesArray();
		nodePart->indices_count = voxelsGrid.getIndicesCount();
		
		invalidationBitMask_Indices = graphics::IGeometryProvider::InvalidationBitGroups::countAndData_bitGroup;
	} else {
		invalidationBitMask_Indices = graphics::IGeometryProvider::InvalidationBitGroups::dataSameCount_bitGroup;
	}

	if (invalidationBitMask_Vertices != 0 || invalidationBitMask_Indices != 0) {
		nodePart->invalidate(
			invalidationBitMask_Vertices, invalidationBitMask_Indices,
			0, 0
		);
	}

	return 0;
}

void VoxelGridDrawable3D::onRenderDataHooked() {
	super::onRenderDataHooked();

	//void
}

void VoxelGridDrawable3D::onGeometryInvalidation() {
	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		VoxelGridDrawable3DInvalidationFlags::mesh
	));
}

void VoxelGridDrawable3D::reservedCreate() {
    createMain();
}

void VoxelGridDrawable3D::createMain() {
    //void
}

void VoxelGridDrawable3D::updateVisual() {
    //void
}


void VoxelGridDrawable3D::disposeMain() {
	//void

	super::disposeMain();
}

VoxelGridDrawable3D::~VoxelGridDrawable3D() {
	//void
}
