#include "VoxelSideTouchDetectionHelper.h"
#include "VoxelTrianglesUtil.h"
#include <base/math/Math.h>
#include <base/math/collision/Intersector.h>
#include <graphics/visual3d/util/VoxelSides.h>
#include <base/statics/StaticsInit.h>

using namespace rpg3D;

Array1D<VoxelSideTouchDetectionHelper::SideConfig>* VoxelSideTouchDetectionHelper::sideConfigsList = base::StaticsInit::AddCbGeneral<Array1D<VoxelSideTouchDetectionHelper::SideConfig>*>(1, []() {
	sideConfigsList = new Array1D<VoxelSideTouchDetectionHelper::SideConfig>( 6, new SideConfig[6] {
		SideConfig(VoxelSides::Down, VoxelTrianglesUtil::voxelTrianglesSize_Down),
		SideConfig(VoxelSides::Up, VoxelTrianglesUtil::voxelTrianglesSize_Up),
		SideConfig(VoxelSides::Front, VoxelTrianglesUtil::voxelTrianglesSize_Front),
		SideConfig(VoxelSides::Back, VoxelTrianglesUtil::voxelTrianglesSize_Back),
		SideConfig(VoxelSides::Left, VoxelTrianglesUtil::voxelTrianglesSize_Left),
		SideConfig(VoxelSides::Right, VoxelTrianglesUtil::voxelTrianglesSize_Right)
	});
});

VoxelSideTouchDetectionHelper::VoxelSideTouchDetectionHelper() {
	//void
}

int VoxelSideTouchDetectionHelper::computeTouchSide(
	float voxelSize, Vector3& voxelGridOffset, Matrix4& parentTransform, Ray& ray,
	Vector3Int& testVoxelGPos)
{
	int closestFaceId = -1;
	float closestFaceDist = 0.0f;
	
	Vector3 intersectionPoint{};
	std::shared_ptr< Array1D<float>> sideTransformedTriangles = nullptr;
	for (int i = 0; i < sideConfigsList->getCount(); i++) {
		SideConfig* sideConfig = sideConfigsList->getPointer(i);

		if (sideTransformedTriangles == nullptr) {
			sideTransformedTriangles = std::make_shared< Array1D<float>>(sideConfig->sideTemplateVoxels->getCount());
		}

		VoxelTrianglesUtil::updateTransformedTriangles(
			parentTransform, voxelGridOffset, voxelSize,
			testVoxelGPos.x, testVoxelGPos.y, testVoxelGPos.z,
			sideTransformedTriangles, sideConfig->sideTemplateVoxels
		);
		bool isIntersection = Intersector::intersectRayTriangles(ray, *sideTransformedTriangles, &intersectionPoint);

		if (isIntersection) {
			float dist = Math::dist3D(
				ray.origin.x, ray.origin.y, ray.origin.z,
				intersectionPoint.x, intersectionPoint.y, intersectionPoint.z
			);

			if (closestFaceId < 0 || dist < closestFaceDist) {
				closestFaceId = sideConfig->sideId;
				closestFaceDist = dist;
			}
		}
	}

	if (closestFaceId < 0) {
		throw LogicException(LOC);
	}

	return closestFaceId;
}

VoxelSideTouchDetectionHelper::~VoxelSideTouchDetectionHelper() {
	//void
}
