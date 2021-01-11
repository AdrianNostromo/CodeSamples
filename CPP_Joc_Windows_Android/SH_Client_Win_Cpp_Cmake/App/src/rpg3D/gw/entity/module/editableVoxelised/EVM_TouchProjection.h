#pragma once

#include <base/gh.h>
#include "EVM_EditStructure.h"
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <base/list/Array3D.h>
#include <graphics/visual3d/drawable/VoxlesGrid3DGeometry.h>

namespace rpg3D {
class EVM_TouchProjection : public EVM_EditStructure {priv typedef EVM_EditStructure super;pub dCtor(EVM_TouchProjection);
	priv class VoxelTrianglesCache {
		pub Vector3Int gPos;

		pub std::shared_ptr<Array1D<float>> triangles;

	    pub dCtor(VoxelTrianglesCache);
		pub explicit VoxelTrianglesCache(Vector3Int& gPos, std::shared_ptr<Array1D<float>> triangles)
			: gPos(gPos), triangles(triangles)
		{
			//void
		}
	};
	
	// This can be used by other tools logic (eg: remover, placer, ...);
	priv ArrayList<VoxelTrianglesCache> voxelTrianglesCache{};
	// This will cause a cache array recreation.
	// Set this flag on mat changed to/from a matIndex of 0.
	priv bool isInvalid_trianglesCache = true;
	
	// This will update the data of the cache only.
	// Set this flag on a model rotation or pos change. Include the vHolder in the check.
	priv bool cacheExists_editorStructure_rotator = false;
	priv Vector3 cachePos_editorStructure_rotator{};
	priv Quaternion cacheRot_editorStructure_rotator{};
	
	priv bool cacheExists_editingVoxelsVisual = false;
	priv Vector3 cachePos_editingVoxelsVisual{};
	priv Quaternion cacheRot_editingVoxelsVisual{};
	
    pub explicit EVM_TouchProjection(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);

	pub std::shared_ptr<TouchVoxelTarget> getClosestVoxelForRay(Ray& ray, bool computeTouchSide) final;

	prot void onVoxelMaterialIndexChanged(
		Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin,
		int oldEditorMatIndex, int originalRegisteredVoxelMatIndex,
		int newEditorMatIndex, int newRegisteredVertexColorIndex,
		bool updatesHistory) override;

	prot ArrayList<VoxelTrianglesCache>* peekVoxelTrianglesCache();

	priv static void rebuildVoxelTrianglesCache(ArrayList<VoxelTrianglesCache>& trianglesCache, Array3D<VoxlesGrid3DGeometry::Voxel>* voxelGrid);
	priv static int peekVoxelMatIdFromGridSafe(int x, int y, int z, Array3D<VoxlesGrid3DGeometry::Voxel>* voxelGrid);
	priv static bool isVoxelCacheable(int x, int y, int z, Array3D<VoxlesGrid3DGeometry::Voxel>* voxelGrid);

	priv static void updateVoxelTrianglesCache(Matrix4& parentTransform, Vector3& voxelGridOffset, float voxelSize, ArrayList<VoxelTrianglesCache>& trianglesCache);

	pub void onIsEditingEnabledChanged() override;

    pub ~EVM_TouchProjection() override;
};
};
