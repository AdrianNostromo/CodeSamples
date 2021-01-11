#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <base/math/PosRotSize3D.h>
#include <memory>
#include <base/list/Array1D.h>

namespace rpg3D {
class VoxelTrianglesUtil {
	priv static Vector3 DLF;
	priv static Vector3 DLB;
	priv static Vector3 DRF;
	priv static Vector3 DRB;
	priv static Vector3 ULF;
	priv static Vector3 ULB;
	priv static Vector3 URF;
	priv static Vector3 URB;

	pub static std::shared_ptr<Array1D<float>> voxelTrianglesSize_Down;
	pub static std::shared_ptr<Array1D<float>> voxelTrianglesSize_Up;
	pub static std::shared_ptr<Array1D<float>> voxelTrianglesSize_Front;
	pub static std::shared_ptr<Array1D<float>> voxelTrianglesSize_Back;
	pub static std::shared_ptr<Array1D<float>> voxelTrianglesSize_Left;
	pub static std::shared_ptr<Array1D<float>> voxelTrianglesSize_Right;

	pub static std::shared_ptr<Array1D<float>> voxelTrianglesBase;

   pub dCtor(VoxelTrianglesUtil);
    pub explicit VoxelTrianglesUtil();

	pub static std::shared_ptr<Array1D<float>> getTransformedTriangles(Matrix4& parentTransform, Vector3& voxelGridOffset, float voxelSize, int gridX, int gridY, int gridZ);
	pub static std::shared_ptr<Array1D<float>> updateTransformedTriangles(
		Matrix4& parentTransform, Vector3& voxelGridOffset, float voxelSize,
		int gridX, int gridY, int gridZ,
		std::shared_ptr<Array1D<float>> editTriangles, std::shared_ptr<Array1D<float>> templateTriangles);

    pub virtual ~VoxelTrianglesUtil();
};
};
