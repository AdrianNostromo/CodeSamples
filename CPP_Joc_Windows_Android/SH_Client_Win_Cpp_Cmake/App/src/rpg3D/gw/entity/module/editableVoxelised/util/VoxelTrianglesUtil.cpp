#include "VoxelTrianglesUtil.h"
#include <base/list/ArrayUtil.h>
#include <base/visual3d/Visual3DVoxelUtil.h>
#include <base/math/PosRotSize3D.h>
#include <base/statics/StaticsInit.h>

using namespace rpg3D;

Vector3 VoxelTrianglesUtil::DLF{ -0.5f, -0.5f, -0.5f };
Vector3 VoxelTrianglesUtil::DLB{ -0.5f, 0.5f, -0.5f };
Vector3 VoxelTrianglesUtil::DRF{ 0.5f, -0.5f, -0.5f };
Vector3 VoxelTrianglesUtil::DRB{ 0.5f, 0.5f, -0.5f };
Vector3 VoxelTrianglesUtil::ULF{ -0.5f, -0.5f, 0.5f };
Vector3 VoxelTrianglesUtil::ULB{ -0.5f, 0.5f, 0.5f };
Vector3 VoxelTrianglesUtil::URF{ 0.5f, -0.5f, 0.5f };
Vector3 VoxelTrianglesUtil::URB{ 0.5f, 0.5f, 0.5f };

std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::voxelTrianglesSize_Down = nullptr;
std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::voxelTrianglesSize_Up = nullptr;
std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::voxelTrianglesSize_Front = nullptr;
std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::voxelTrianglesSize_Back = nullptr;
std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::voxelTrianglesSize_Left = nullptr;
std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::voxelTrianglesSize_Right = nullptr;
std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::voxelTrianglesBase = base::StaticsInit::AddCbGeneral<std::shared_ptr<Array1D<float>>>(1/*initLayerIndex*/, []() {
	voxelTrianglesSize_Down = std::make_shared< Array1D<float>>(3 * 6, new float[3 * 6]{
		DLF.x, DLF.y, DLF.z,
		DLB.x, DLB.y, DLB.z,
		DRB.x, DRB.y, DRB.z,

		DLF.x, DLF.y, DLF.z,
		DRB.x, DRB.y, DRB.z,
		DRF.x, DRF.y, DRF.z
	});
	voxelTrianglesSize_Up = std::make_shared< Array1D<float>>(3 * 6, new float[3 * 6]{
		ULF.x, ULF.y, ULF.z,
		ULB.x, ULB.y, ULB.z,
		URB.x, URB.y, URB.z,

		ULF.x, ULF.y, ULF.z,
		URB.x, URB.y, URB.z,
		URF.x, URF.y, URF.z
	});
	voxelTrianglesSize_Front = std::make_shared< Array1D<float>>(3 * 6, new float[3 * 6]{
		DLF.x, DLF.y, DLF.z,
		ULF.x, ULF.y, ULF.z,
		URF.x, URF.y, URF.z,

		DLF.x, DLF.y, DLF.z,
		URF.x, URF.y, URF.z,
		DRF.x, DRF.y, DRF.z
	});
	voxelTrianglesSize_Back = std::make_shared< Array1D<float>>(3 * 6, new float[3 * 6]{
		DLB.x, DLB.y, DLB.z,
		ULB.x, ULB.y, ULB.z,
		URB.x, URB.y, URB.z,

		DLB.x, DLB.y, DLB.z,
		URB.x, URB.y, URB.z,
		DRB.x, DRB.y, DRB.z
	});
	voxelTrianglesSize_Left = std::make_shared< Array1D<float>>(3 * 6, new float[3 * 6]{
		DLF.x, DLF.y, DLF.z,
		ULB.x, ULB.y, ULB.z,
		ULF.x, ULF.y, ULF.z,

		DLF.x, DLF.y, DLF.z,
		DLB.x, DLB.y, DLB.z,
		ULB.x, ULB.y, ULB.z
	});
	voxelTrianglesSize_Right = std::make_shared< Array1D<float>> (3 * 6, new float[3 * 6] {
		DRF.x, DRF.y, DRF.z,
		URB.x, URB.y, URB.z,
		URF.x, URF.y, URF.z,

		DRF.x, DRF.y, DRF.z,
		DRB.x, DRB.y, DRB.z,
		URB.x, URB.y, URB.z
	});

	voxelTrianglesBase = base::ArrayUtil::concatenateArrays_sp<float>(
		voxelTrianglesSize_Down,
		voxelTrianglesSize_Up,
		voxelTrianglesSize_Front,
		voxelTrianglesSize_Back,
		voxelTrianglesSize_Left,
		voxelTrianglesSize_Right
	);
});

VoxelTrianglesUtil::VoxelTrianglesUtil() {
	//void
}

std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::getTransformedTriangles(Matrix4& parentTransform, Vector3& voxelGridOffset, float voxelSize, int gridX, int gridY, int gridZ) {
	return updateTransformedTriangles(
		parentTransform, voxelGridOffset, voxelSize, 
		gridX, gridY, gridZ, 
		std::make_shared< Array1D<float>>(voxelTrianglesBase->getCount()), voxelTrianglesBase);
}

std::shared_ptr<Array1D<float>> VoxelTrianglesUtil::updateTransformedTriangles(
	Matrix4& parentTransform, Vector3& voxelGridOffset, float voxelSize,
	int gridX, int gridY, int gridZ,
	std::shared_ptr<Array1D<float>> editTriangles, std::shared_ptr<Array1D<float>> templateTriangles)
{
	for (int i = 0; i < templateTriangles->getCount(); i += 3) {
		float vertexX = templateTriangles->getDirect(i + 0);
		float vertexY = templateTriangles->getDirect(i + 1);
		float vertexZ = templateTriangles->getDirect(i + 2);

		std::shared_ptr<base::PosRotSize3D> positional = base::Visual3DVoxelUtil::drawablePositionalToGlobalPositional(
			parentTransform,

			voxelSize, voxelSize, voxelSize,

			voxelGridOffset.x + gridX + vertexX,
			voxelGridOffset.y + gridY + vertexY,
			voxelGridOffset.z + gridZ + vertexZ
		);

		editTriangles->setDirect(i + 0, positional->pos.x);
		editTriangles->setDirect(i + 1, positional->pos.y);
		editTriangles->setDirect(i + 2, positional->pos.z);
	}

	return editTriangles;
}

VoxelTrianglesUtil::~VoxelTrianglesUtil() {
	//void
}

