#include "EVM_TouchProjection.h"
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <base/math/collision/Intersector.h>
#include "util/VoxelSideTouchDetectionHelper.h"
#include "util/VoxelTrianglesUtil.h"

using namespace rpg3D;

EVM_TouchProjection::EVM_TouchProjection(
	IWorldEntity* entity,
	EditableVoxelisedTemplate* t)
	: super(
		entity,
		t)
{
	//void
}

std::shared_ptr<EVM_TouchProjection::TouchVoxelTarget> EVM_TouchProjection::getClosestVoxelForRay(Ray& ray, bool computeTouchSide) {
	bool isCacheUpdateRequired = false;

	if (isInvalid_trianglesCache) {
		isInvalid_trianglesCache = false;

		isCacheUpdateRequired = true;

		// Generate a new cache structure.
		rebuildVoxelTrianglesCache(voxelTrianglesCache, editingVoxelsVisual->peekVoxelsGrid()->peekGrid());
	}

	if (!isCacheUpdateRequired) {
		if (cacheExists_editorStructure_rotator != (getEditorStructure_rotator() != nullptr)
			|| (cacheExists_editorStructure_rotator && (!cachePos_editorStructure_rotator.equals(getEditorStructure_rotator()->getPos()) || !cacheRot_editorStructure_rotator.equals(getEditorStructure_rotator()->getRotation()))))
		{
			isCacheUpdateRequired = true;
		} else if (cacheExists_editingVoxelsVisual != (editingVoxelsVisual != nullptr)
			|| (cacheExists_editingVoxelsVisual && (!cachePos_editingVoxelsVisual.equals(editingVoxelsVisual->getPos()) || !cacheRot_editingVoxelsVisual.equals(editingVoxelsVisual->getRotation()))))
		{
			isCacheUpdateRequired = true;
		}
	}

	if (isCacheUpdateRequired) {
		// Update the value values.
		cacheExists_editorStructure_rotator = getEditorStructure_rotator() != nullptr;
		if (cacheExists_editorStructure_rotator) {
			cachePos_editorStructure_rotator.set(getEditorStructure_rotator()->getPos());
			cacheRot_editorStructure_rotator.set(getEditorStructure_rotator()->getRotation());
		}

		cacheExists_editingVoxelsVisual = editingVoxelsVisual != nullptr;
		if (cacheExists_editingVoxelsVisual) {
			cachePos_editingVoxelsVisual.set(editingVoxelsVisual->getPos());
			cacheRot_editingVoxelsVisual.set(editingVoxelsVisual->getRotation());
		}

		float voxelSize = editingVoxelsVisual->peekVoxelsGrid()->getVoxelSize();
		Vector3 voxelGridOffset{ editingVoxelsVisual->peekVoxelsGrid()->getVoxelGridOffset() };
		Matrix4 parentTransform{ editingVoxelsVisual->getGlobalTransform(false) };

		// Fill the existing cache with data.
		updateVoxelTrianglesCache(parentTransform, voxelGridOffset, voxelSize, voxelTrianglesCache);
	}
	
	std::shared_ptr<Vector3Int> closestVoxelGridPos = nullptr;
	float closestVoxelDist = 0.0f;

	// Enumerate each voxel from the group and check if it is on the collision ray.
	Vector3 intersectionPoint{};
	for (int i = 0; i < voxelTrianglesCache.size(); i++) {
		VoxelTrianglesCache* entry = voxelTrianglesCache.getPointer(i);

		if (Intersector::intersectRayTriangles(ray, *entry->triangles, &intersectionPoint)) {
			float dist = Math::dist3D(
				ray.origin.x, ray.origin.y, ray.origin.z,
				intersectionPoint.x, intersectionPoint.y, intersectionPoint.z
			);

			if (closestVoxelGridPos == nullptr || dist < closestVoxelDist) {
				if (closestVoxelGridPos == nullptr) {
					closestVoxelGridPos = std::make_shared<Vector3Int>(entry->gPos.x, entry->gPos.y, entry->gPos.z);
				} else {
					closestVoxelGridPos->set(entry->gPos.x, entry->gPos.y, entry->gPos.z);
				}
				closestVoxelDist = dist;
			}
		}
	}

	int touchVoxelSide = -1;
	if (computeTouchSide && closestVoxelGridPos != nullptr) {
		// Check on which side the touch occurred.
		// Get the triangles for each side and check them separately again.

		float voxelSize = editingVoxelsVisual->peekVoxelsGrid()->getVoxelSize();
		Vector3 voxelGridOffset{ editingVoxelsVisual->peekVoxelsGrid()->getVoxelGridOffset() };
		Matrix4 parentTransform{ editingVoxelsVisual->getGlobalTransform(false) };

		touchVoxelSide = VoxelSideTouchDetectionHelper::computeTouchSide(
			voxelSize, voxelGridOffset, parentTransform, ray,
			*closestVoxelGridPos
		);
	}

	if (closestVoxelGridPos != nullptr) {
		return std::make_shared<TouchVoxelTarget>(*closestVoxelGridPos, touchVoxelSide);
	}

	return nullptr;
}

void EVM_TouchProjection::onVoxelMaterialIndexChanged(
	Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin,
	int oldEditorMatIndex, int originalRegisteredVoxelMatIndex,
	int newEditorMatIndex, int newRegisteredVertexColorIndex,
	bool updatesHistory)
{
	super::onVoxelMaterialIndexChanged(
		gPos, postChangeGridBoundsMin,
		oldEditorMatIndex, originalRegisteredVoxelMatIndex,
		newEditorMatIndex, newRegisteredVertexColorIndex,
		updatesHistory
	);

	if (originalRegisteredVoxelMatIndex != newRegisteredVertexColorIndex && ((originalRegisteredVoxelMatIndex < 0 && newRegisteredVertexColorIndex >= 0) || (originalRegisteredVoxelMatIndex >= 0 && newRegisteredVertexColorIndex < 0))) {
		isInvalid_trianglesCache = true;
	}
}

ArrayList<EVM_TouchProjection::VoxelTrianglesCache>* EVM_TouchProjection::peekVoxelTrianglesCache() {
	if (isInvalid_trianglesCache) {
		throw LogicException(LOC);
	}
	return &voxelTrianglesCache;
}

void EVM_TouchProjection::rebuildVoxelTrianglesCache(ArrayList<VoxelTrianglesCache>& trianglesCache, Array3D<VoxlesGrid3DGeometry::Voxel>* voxelGrid) {
	int ct = 0;
	for (int x = 0; x < voxelGrid->getCountA(); x++) {
		for (int y = 0; y < voxelGrid->getCountB(); y++) {
			for (int z = 0; z < voxelGrid->getCountC(); z++) {
				VoxlesGrid3DGeometry::Voxel* voxel = voxelGrid->getPointer(x, y, z);

				if (voxel->matIndex >= 0 && isVoxelCacheable(x, y, z, voxelGrid)) {
					if (ct >= trianglesCache.size()) {
						// Add cache entries only if they don't already exist.
						Vector3Int gPos(x, y, z);
						trianglesCache.append_emplace(
							gPos,
							std::make_shared<Array1D<float>>(VoxelTrianglesUtil::voxelTrianglesBase->getCount())
						);
					} else {
						VoxelTrianglesCache* entry = trianglesCache.getPointer(ct);
						entry->gPos.set(x, y, z);
					}
					ct++;
				}
			}
		}
	}

	// Remove extra cache entries.
	while (trianglesCache.size() > ct) {
		trianglesCache.remove(trianglesCache.size() - 1);
	}

	//LogHandler.devLogHandler.postDebug("Entity editor touch voxels cache size: " + ct);
}

bool EVM_TouchProjection::isVoxelCacheable(int x, int y, int z, Array3D<VoxlesGrid3DGeometry::Voxel>* voxelGrid) {
	if (peekVoxelMatIdFromGridSafe(x, y, z, voxelGrid) >= 0 && (
		peekVoxelMatIdFromGridSafe(x - 1, y, z, voxelGrid) < 0 || peekVoxelMatIdFromGridSafe(x + 1, y, z, voxelGrid) < 0 ||
		peekVoxelMatIdFromGridSafe(x, y - 1, z, voxelGrid) < 0 || peekVoxelMatIdFromGridSafe(x, y + 1, z, voxelGrid) < 0 ||
		peekVoxelMatIdFromGridSafe(x, y, z - 1, voxelGrid) < 0 || peekVoxelMatIdFromGridSafe(x, y, z + 1, voxelGrid) < 0))
	{
		return true;
	}

	return false;
}

// Returns 0 if out of bounds.
int EVM_TouchProjection::peekVoxelMatIdFromGridSafe(int x, int y, int z, Array3D<VoxlesGrid3DGeometry::Voxel>* voxelGrid) {
	if (x < 0 || y < 0 || z < 0) {
		return -1;
	}

	if (x >= voxelGrid->getCountA() ||
		y >= voxelGrid->getCountB() ||
		z >= voxelGrid->getCountC())
	{
		return -1;
	}

	VoxlesGrid3DGeometry::Voxel* voxel = voxelGrid->getPointer(x, y, z);

	return voxel->matIndex;
}

void EVM_TouchProjection::updateVoxelTrianglesCache(Matrix4& parentTransform, Vector3& voxelGridOffset, float voxelSize, ArrayList<VoxelTrianglesCache>& trianglesCache) {
	for (int i = 0; i < trianglesCache.size(); i++) {
		VoxelTrianglesCache* entry = trianglesCache.getPointer(i);

		std::shared_ptr<Array1D<float>> t1 = VoxelTrianglesUtil::updateTransformedTriangles(parentTransform, voxelGridOffset, voxelSize, entry->gPos.x, entry->gPos.y, entry->gPos.z, entry->triangles, VoxelTrianglesUtil::voxelTrianglesBase);
		(void)t1;
	}
}

void EVM_TouchProjection::onIsEditingEnabledChanged() {
	super::onIsEditingEnabledChanged();

	if (!isEditingEnabled) {
		//void
	} else {
		isInvalid_trianglesCache = true;
	}
}

EVM_TouchProjection::~EVM_TouchProjection() {
	//void
}
