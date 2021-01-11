#include "VoxlesGrid3DGeometry.h"
#include "graphics/visual3d/util/VoxelSides.h"
#include <base/math/Vector3.h>
#include <base/math/util/BoolUtil.h>
#include <sstream>
#include <base/exceptions/LogicException.h>
#include <base/app/config/AppConfig.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <chrono>
#include <base/log/GlobalAccessLogHandler.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_LinearX.h>
#include <graphics/visual3d/drawable/voxelsGrid/tiling/TilingInformation_GridXY.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <graphics/util/GraphicsUtil.h>

const int VoxlesGrid3DGeometry::MATERIALS_COUNT_LIMIT = 100;

const int VoxlesGrid3DGeometry::AXIS_X = 1;
const int VoxlesGrid3DGeometry::AXIS_Y = 2;
const int VoxlesGrid3DGeometry::AXIS_Z = 3;

const Vector3 VoxlesGrid3DGeometry::vFLD{ -0.5f, -0.5f, -0.5f };
const Vector3 VoxlesGrid3DGeometry::vFRD{ 0.5f, -0.5f, -0.5f };
const Vector3 VoxlesGrid3DGeometry::vFLU{ -0.5f, -0.5f, 0.5f };
const Vector3 VoxlesGrid3DGeometry::vFRU{ 0.5f, -0.5f, 0.5f };

const Vector3 VoxlesGrid3DGeometry::vBLD{ -0.5f, 0.5f, -0.5f };
const Vector3 VoxlesGrid3DGeometry::vBRD{ 0.5f, 0.5f, -0.5f };
const Vector3 VoxlesGrid3DGeometry::vBLU{ -0.5f, 0.5f, 0.5f };
const Vector3 VoxlesGrid3DGeometry::vBRU{ 0.5f, 0.5f, 0.5f };

VoxlesGrid3DGeometry::VoxlesGrid3DGeometry(IHandler* _handler)
	: handler(_handler)
{
	materialsList.reserve(10);
}

int VoxlesGrid3DGeometry::updateGeometry() {
	if (invalidationBitMap == 0) {
		// Update should never be called if not needed.
		return -1;
	}

	if (BoolUtil::isBitAtIndex(invalidationBitMap, InvalidationFlags::gridOptimisation)) {
		if (update_optimiseGrid() != 0) {
			return -2;
		}

		if (!BoolUtil::isBitAtIndex(invalidationBitMap, InvalidationFlags::vertices)) {
			return -3;
		}
	}

	if (BoolUtil::isBitAtIndex(invalidationBitMap, InvalidationFlags::vertices)) {
		if (update_quads() != 0) {
			return -4;
		}
	}

	invalidationBitMap = 0;

	return 0;
}

int VoxlesGrid3DGeometry::update_optimiseGrid() {
	// Reset all sides to initial values.
	memset(voxelsSideConfigsGrid.data, 0, sizeof(Voxel::SidesConfig) * (voxelsSideConfigsGrid.count.x * voxelsSideConfigsGrid.count.y * voxelsSideConfigsGrid.count.z));

	//std::string originalGridStruncture = generateGridString(grid, voxelsSideConfigsGrid);
	//std::cout << "Original grid structure: \n" << originalGridStruncture << "\n" << std::endl;

	int discardedVoxelSidesCount = 0;

	// Remove hidden voxel sides.
	if (optimizeVoxelGrid3DHiddenSides(&voxelsGrid, &voxelsSideConfigsGrid, materialsList, discardedVoxelSidesCount, enabledSidesTriangulationBitMask) != 0) {
		return -2;
	}

	//std::string optimisedGridStruncture = generateGridString(grid, voxelsSideConfigsGrid);
	//std::cout << "Optimised faces grid structure: \n" << optimisedGridStruncture << "\n" << std::endl;

	// Process materials.
	int r = optimizeVoxelGrid3DCombinedFaces(
		&voxelsGrid, &voxelsSideConfigsGrid,
		discardedVoxelSidesCount
	);
	if (r != 0) {
		return -3;
	}

	//std::string optimisedGridSides = generateGridUsedSidesString(grid);
	//std::cout << "Optimised grid structure: \n" << optimisedGridSides << "\n" << std::endl;

	int newQuadsCount = (voxelsGrid.count.x * voxelsGrid.count.y * voxelsGrid.count.z) * 6 - discardedVoxelSidesCount;

	// Recreate quads list if needed.
	if (verticesCount != newQuadsCount * 4) {
		disposeVerticesAndIndices();

		verticesCount = newQuadsCount * 4;
		if (verticesCount > 0) {
			verticesArray = new graphics::GLTypes::Vertex_3DGenericTexture[verticesCount];
		}

		indicesCount = newQuadsCount * 6;
		if (indicesCount > graphics::GraphicsUtil::genericIndices_count) {
			throw LogicException(LOC);
		}
		indicesArrayUnmanaged = graphics::GraphicsUtil::genericIndices;
	}

	return 0;
}

int VoxlesGrid3DGeometry::update_quads() {
	// Convert the faces to quads list.
	if (buildVerticesArray() != 0) {
		return -6;
	}

	return 0;
}

graphics::GLTypes::Vertex_3DGenericTexture* VoxlesGrid3DGeometry::getVerticesArray() {
	return verticesArray;
}

int VoxlesGrid3DGeometry::getVerticesCount() {
	return verticesCount;
}

unsigned short* VoxlesGrid3DGeometry::getIndicesArray() {
	return indicesArrayUnmanaged;
}

int VoxlesGrid3DGeometry::getIndicesCount() {
	return indicesCount;
}

int VoxlesGrid3DGeometry::buildVerticesArray() {
	int vertexIndex = 0;

	int countX = voxelsGrid.count.x;
	int countY = voxelsGrid.count.y;
	int countZ = voxelsGrid.count.z;
	Voxel* voxelsGrid_data = voxelsGrid.data;
	Voxel::SidesConfig* voxelsSideConfigsGrid_data = voxelsSideConfigsGrid.data;

	for (int x = 0; x < countX; x++) {
		for (int y = 0; y < countY; y++) {
			for (int z = 0; z < countZ; z++) {
				int index = z * (countX * countY) + y * countX + x;

				Voxel* voxel = &voxelsGrid_data[index];
				Voxel::SidesConfig* sideConfigs = &voxelsSideConfigsGrid_data[index];

				if (voxel->matIndex < 0) {
					continue;
				}

				const Voxel::SideConfig* voxelSideConfig;
				int sideId;

				Material* mat = materialsList.getDirect(voxel->matIndex);

				if (mat == nullptr) {
					// An error occured. The entity template doesn't have any materials.
					// Only time this occured is when a entity template had a VoxelisedStructureTemplate but it didn't use it (it used a template grid visual).
					throw LogicException(LOC);
				}


				graphics::GLTypes::Vertex_3DGenericTexture* v1;
				graphics::GLTypes::Vertex_3DGenericTexture* v2;
				graphics::GLTypes::Vertex_3DGenericTexture* v3;
				graphics::GLTypes::Vertex_3DGenericTexture* v4;

				sideId = VoxelSides::Up;
				voxelSideConfig = &sideConfigs->sidesList[sideId];
				if (!voxelSideConfig->isDiscarded) {
					v1 = &verticesArray[vertexIndex++];
					v1->pos.x = (vFLU.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v1->pos.y = (vFLU.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v1->pos.z = (vFLU.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v1->normal.x = 0.0f;
					v1->normal.y = 0.0f;
					v1->normal.z = 1.0f;
					v1->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v1->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v1->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v1->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v1->uv.x = -1;//Not used
					v1->uv.y = -1;//Not used
					v1->a_diffuseTextureIndex = -1;//Not used

					v2 = &verticesArray[vertexIndex++];
					v2->pos.x = (vFRU.x + voxelGridOffset.x + x + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v2->pos.y = (vFRU.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v2->pos.z = (vFRU.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v2->normal.x = 0.0f;
					v2->normal.y = 0.0f;
					v2->normal.z = 1.0f;
					v2->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v2->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v2->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v2->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v2->uv.x = -1;//Not used
					v2->uv.y = -1;//Not used
					v2->a_diffuseTextureIndex = -1;//Not used

					v3 = &verticesArray[vertexIndex++];
					v3->pos.x = (vBRU.x + voxelGridOffset.x + x + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v3->pos.y = (vBRU.y + voxelGridOffset.y + y + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v3->pos.z = (vBRU.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v3->normal.x = 0.0f;
					v3->normal.y = 0.0f;
					v3->normal.z = 1.0f;
					v3->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v3->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v3->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v3->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v3->uv.x = -1;//Not used
					v3->uv.y = -1;//Not used
					v3->a_diffuseTextureIndex = -1;//Not used

					v4 = &verticesArray[vertexIndex++];
					v4->pos.x = (vBLU.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v4->pos.y = (vBLU.y + voxelGridOffset.y + y + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v4->pos.z = (vBLU.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v4->normal.x = 0.0f;
					v4->normal.y = 0.0f;
					v4->normal.z = 1.0f;
					v4->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v4->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v4->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v4->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v4->uv.x = -1;//Not used
					v4->uv.y = -1;//Not used
					v4->a_diffuseTextureIndex = -1;//Not used
				}

				sideId = VoxelSides::Down;
				voxelSideConfig = &sideConfigs->sidesList[sideId];
				if (!voxelSideConfig->isDiscarded) {
					v1 = &verticesArray[vertexIndex++];
					v1->pos.x = (vBLD.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v1->pos.y = (vBLD.y + voxelGridOffset.y + y + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v1->pos.z = (vBLD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v1->normal.x = 0.0f;
					v1->normal.y = 0.0f;
					v1->normal.z = -1.0f;
					v1->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v1->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v1->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v1->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v1->uv.x = -1;//Not used
					v1->uv.y = -1;//Not used
					v1->a_diffuseTextureIndex = -1;//Not used

					v2 = &verticesArray[vertexIndex++];
					v2->pos.x = (vBRD.x + voxelGridOffset.x + x + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v2->pos.y = (vBRD.y + voxelGridOffset.y + y + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v2->pos.z = (vBRD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v2->normal.x = 0.0f;
					v2->normal.y = 0.0f;
					v2->normal.z = -1.0f;
					v2->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v2->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v2->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v2->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v2->uv.x = -1;//Not used
					v2->uv.y = -1;//Not used
					v2->a_diffuseTextureIndex = -1;//Not used

					v3 = &verticesArray[vertexIndex++];
					v3->pos.x = (vFRD.x + voxelGridOffset.x + x + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v3->pos.y = (vFRD.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v3->pos.z = (vFRD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v3->normal.x = 0.0f;
					v3->normal.y = 0.0f;
					v3->normal.z = -1.0f;
					v3->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v3->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v3->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v3->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v3->uv.x = -1;//Not used
					v3->uv.y = -1;//Not used
					v3->a_diffuseTextureIndex = -1;//Not used

					v4 = &verticesArray[vertexIndex++];
					v4->pos.x = (vFLD.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v4->pos.y = (vFLD.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v4->pos.z = (vFLD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v4->normal.x = 0.0f;
					v4->normal.y = 0.0f;
					v4->normal.z = -1.0f;
					v4->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v4->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v4->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v4->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v4->uv.x = -1;//Not used
					v4->uv.y = -1;//Not used
					v4->a_diffuseTextureIndex = -1;//Not used
				}

				sideId = VoxelSides::Left;
				voxelSideConfig = &sideConfigs->sidesList[sideId];
				if (!voxelSideConfig->isDiscarded) {
					v1 = &verticesArray[vertexIndex++];
					v1->pos.x = (vBLD.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v1->pos.y = (vBLD.y + voxelGridOffset.y + y + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v1->pos.z = (vBLD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v1->normal.x = -1.0f;
					v1->normal.y = 0.0f;
					v1->normal.z = 0.0f;
					v1->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v1->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v1->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v1->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v1->uv.x = -1;//Not used
					v1->uv.y = -1;//Not used
					v1->a_diffuseTextureIndex = -1;//Not used

					v2 = &verticesArray[vertexIndex++];
					v2->pos.x = (vFLD.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v2->pos.y = (vFLD.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v2->pos.z = (vFLD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v2->normal.x = -1.0f;
					v2->normal.y = 0.0f;
					v2->normal.z = 0.0f;
					v2->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v2->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v2->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v2->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v2->uv.x = -1;//Not used
					v2->uv.y = -1;//Not used
					v2->a_diffuseTextureIndex = -1;//Not used

					v3 = &verticesArray[vertexIndex++];
					v3->pos.x = (vFLU.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v3->pos.y = (vFLU.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v3->pos.z = (vFLU.z + voxelGridOffset.z + z + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v3->normal.x = -1.0f;
					v3->normal.y = 0.0f;
					v3->normal.z = 0.0f;
					v3->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v3->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v3->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v3->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v3->uv.x = -1;//Not used
					v3->uv.y = -1;//Not used
					v3->a_diffuseTextureIndex = -1;//Not used

					v4 = &verticesArray[vertexIndex++];
					v4->pos.x = (vBLU.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v4->pos.y = (vBLU.y + voxelGridOffset.y + y + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v4->pos.z = (vBLU.z + voxelGridOffset.z + z + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v4->normal.x = -1.0f;
					v4->normal.y = 0.0f;
					v4->normal.z = 0.0f;
					v4->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v4->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v4->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v4->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v4->uv.x = -1;//Not used
					v4->uv.y = -1;//Not used
					v4->a_diffuseTextureIndex = -1;//Not used
				}

				sideId = VoxelSides::Right;
				voxelSideConfig = &sideConfigs->sidesList[sideId];
				if (!voxelSideConfig->isDiscarded) {
					v1 = &verticesArray[vertexIndex++];
					v1->pos.x = (vBRU.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v1->pos.y = (vBRU.y + voxelGridOffset.y + y + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v1->pos.z = (vBRU.z + voxelGridOffset.z + z + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v1->normal.x = 1.0f;
					v1->normal.y = 0.0f;
					v1->normal.z = 0.0f;
					v1->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v1->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v1->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v1->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v1->uv.x = -1;//Not used
					v1->uv.y = -1;//Not used
					v1->a_diffuseTextureIndex = -1;//Not used

					v2 = &verticesArray[vertexIndex++];
					v2->pos.x = (vFRU.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v2->pos.y = (vFRU.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v2->pos.z = (vFRU.z + voxelGridOffset.z + z + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v2->normal.x = 1.0f;
					v2->normal.y = 0.0f;
					v2->normal.z = 0.0f;
					v2->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v2->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v2->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v2->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v2->uv.x = -1;//Not used
					v2->uv.y = -1;//Not used
					v2->a_diffuseTextureIndex = -1;//Not used

					v3 = &verticesArray[vertexIndex++];
					v3->pos.x = (vFRD.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v3->pos.y = (vFRD.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v3->pos.z = (vFRD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v3->normal.x = 1.0f;
					v3->normal.y = 0.0f;
					v3->normal.z = 0.0f;
					v3->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v3->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v3->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v3->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v3->uv.x = -1;//Not used
					v3->uv.y = -1;//Not used
					v3->a_diffuseTextureIndex = -1;//Not used

					v4 = &verticesArray[vertexIndex++];
					v4->pos.x = (vBRD.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v4->pos.y = (vBRD.y + voxelGridOffset.y + y + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v4->pos.z = (vBRD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v4->normal.x = 1.0f;
					v4->normal.y = 0.0f;
					v4->normal.z = 0.0f;
					v4->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v4->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v4->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v4->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v4->uv.x = -1;//Not used
					v4->uv.y = -1;//Not used
					v4->a_diffuseTextureIndex = -1;//Not used
				}

				sideId = VoxelSides::Front;
				voxelSideConfig = &sideConfigs->sidesList[sideId];
				if (!voxelSideConfig->isDiscarded) {
					v1 = &verticesArray[vertexIndex++];
					v1->pos.x = (vFLD.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v1->pos.y = (vFLD.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v1->pos.z = (vFLD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v1->normal.x = 0.0f;
					v1->normal.y = -1.0f;
					v1->normal.z = 0.0f;
					v1->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v1->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v1->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v1->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v1->uv.x = -1;//Not used
					v1->uv.y = -1;//Not used
					v1->a_diffuseTextureIndex = -1;//Not used

					v2 = &verticesArray[vertexIndex++];
					v2->pos.x = (vFRD.x + voxelGridOffset.x + x + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v2->pos.y = (vFRD.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v2->pos.z = (vFRD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v2->normal.x = 0.0f;
					v2->normal.y = -1.0f;
					v2->normal.z = 0.0f;
					v2->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v2->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v2->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v2->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v2->uv.x = -1;//Not used
					v2->uv.y = -1;//Not used
					v2->a_diffuseTextureIndex = -1;//Not used

					v3 = &verticesArray[vertexIndex++];
					v3->pos.x = (vFRU.x + voxelGridOffset.x + x + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v3->pos.y = (vFRU.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v3->pos.z = (vFRU.z + voxelGridOffset.z + z + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v3->normal.x = 0.0f;
					v3->normal.y = -1.0f;
					v3->normal.z = 0.0f;
					v3->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v3->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v3->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v3->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v3->uv.x = -1;//Not used
					v3->uv.y = -1;//Not used
					v3->a_diffuseTextureIndex = -1;//Not used

					v4 = &verticesArray[vertexIndex++];
					v4->pos.x = (vFLU.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v4->pos.y = (vFLU.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v4->pos.z = (vFLU.z + voxelGridOffset.z + z + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v4->normal.x = 0.0f;
					v4->normal.y = -1.0f;
					v4->normal.z = 0.0f;
					v4->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v4->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v4->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v4->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v4->uv.x = -1;//Not used
					v4->uv.y = -1;//Not used
					v4->a_diffuseTextureIndex = -1;//Not used
				}

				sideId = VoxelSides::Back;
				voxelSideConfig = &sideConfigs->sidesList[sideId];
				if (!voxelSideConfig->isDiscarded) {
					v1 = &verticesArray[vertexIndex++];
					v1->pos.x = (vBLU.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v1->pos.y = (vBLU.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v1->pos.z = (vBLU.z + voxelGridOffset.z + z + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v1->normal.x = 0.0f;
					v1->normal.y = 1.0f;
					v1->normal.z = 0.0f;
					v1->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v1->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v1->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v1->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v1->uv.x = -1;//Not used
					v1->uv.y = -1;//Not used
					v1->a_diffuseTextureIndex = -1;//Not used

					v2 = &verticesArray[vertexIndex++];
					v2->pos.x = (vBRU.x + voxelGridOffset.x + x + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v2->pos.y = (vBRU.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v2->pos.z = (vBRU.z + voxelGridOffset.z + z + (voxelSideConfig->faceExpansionB + 1) - 1.0f) * voxelSize;
					v2->normal.x = 0.0f;
					v2->normal.y = 1.0f;
					v2->normal.z = 0.0f;
					v2->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v2->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v2->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v2->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v2->uv.x = -1;//Not used
					v2->uv.y = -1;//Not used
					v2->a_diffuseTextureIndex = -1;//Not used

					v3 = &verticesArray[vertexIndex++];
					v3->pos.x = (vBRD.x + voxelGridOffset.x + x + (voxelSideConfig->faceExpansionA + 1) - 1.0f) * voxelSize;
					v3->pos.y = (vBRD.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v3->pos.z = (vBRD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v3->normal.x = 0.0f;
					v3->normal.y = 1.0f;
					v3->normal.z = 0.0f;
					v3->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v3->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v3->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v3->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v3->uv.x = -1;//Not used
					v3->uv.y = -1;//Not used
					v3->a_diffuseTextureIndex = -1;//Not used

					v4 = &verticesArray[vertexIndex++];
					v4->pos.x = (vBLD.x + voxelGridOffset.x + x + 0.0f) * voxelSize;
					v4->pos.y = (vBLD.y + voxelGridOffset.y + y + 0.0f) * voxelSize;
					v4->pos.z = (vBLD.z + voxelGridOffset.z + z + 0.0f) * voxelSize;
					v4->normal.x = 0.0f;
					v4->normal.y = 1.0f;
					v4->normal.z = 0.0f;
					v4->diffuseColor.r = static_cast<unsigned char>(mat->color.r * 255.0f);
					v4->diffuseColor.g = static_cast<unsigned char>(mat->color.g * 255.0f);
					v4->diffuseColor.b = static_cast<unsigned char>(mat->color.b * 255.0f);
					v4->diffuseColor.a = static_cast<unsigned char>(mat->color.a * 255.0f);
					v4->uv.x = -1;//Not used
					v4->uv.y = -1;//Not used
					v4->a_diffuseTextureIndex = -1;//Not used
				}
			}
		}
	}

	//std::cout << "Voxel model quads count:" << quadsCount << std::endl;

	return 0;
}

bool VoxlesGrid3DGeometry::isVoxel(
	const int x, const int y, const int z, Array3D<Voxel>* grid, ArrayList<Material*>& materialsList)
{
	Voxel* v = grid->getPointer(x, y, z, nullptr);
	if (v == nullptr || v->matIndex < 0 || materialsList.getDirect(v->matIndex) == nullptr) {
		return false;
	}

	return true;
}

//std::string VoxlesGrid3DGeometry::generateGridString(
//	Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid)
//{
//	std::stringstream ss;
//
//	for (int gPosX = 0; gPosX < voxelsGrid->count.x; gPosX++) {
//		for (int gPosY = 0; gPosY < voxelsGrid->count.y; gPosY++) {
//			for (int gPosZ = 0; gPosZ < voxelsGrid->count.z; gPosZ++) {
//				Voxel* v = voxelsGrid->getPointer(gPosX, gPosY, gPosZ);
//				Voxel::SidesConfig* sideConfigs = voxelsSideConfigsGrid->getPointer(gPosX, gPosY, gPosZ);
//
//				if (!sideConfigs->isAnySideUsed()) {
//					ss << 'X';
//				} else {
//					ss << v->matIndex;
//				}
//			}
//
//			ss << "\n";
//		}
//
//		ss << "\n\n";
//	}
//
//	return ss.str();
//}

//std::string VoxlesGrid3DGeometry::generateGridUsedSidesString(
//	Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid, 
//	ArrayList<Material*>& materialsList)
//{
//	std::stringstream ss;
//
//	for (int gPosX = 0; gPosX < voxelsGrid->count.x; gPosX++) {
//		for (int gPosY = 0; gPosY < voxelsGrid->count.y; gPosY++) {
//			for (int gPosZ = 0; gPosZ < voxelsGrid->count.z; gPosZ++) {
//                Voxel* v = voxelsGrid->getPointer(gPosX, gPosY, gPosZ);
//				Voxel::SidesConfig* sideConfigs = voxelsSideConfigsGrid->getPointer(gPosX, gPosY, gPosZ);
//
//				if (v->matIndex >= 0 && materialsList.getDirect(v->matIndex) != nullptr) {
//					ss << ((v->matIndex >= 0) ? (6 - sideConfigs->discardedSidesCount) : 0) << ';';
//				} else {
//					ss << 0 << ';';
//				}
//			}
//
//			ss << "\n";
//		}
//
//		ss << "\n\n";
//	}
//
//	return ss.str();
//}

int VoxlesGrid3DGeometry::optimizeVoxelGrid3DHiddenSides(
	Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
	ArrayList<Material*>& materialsList,
	int& discardedVoxelSidesCount,
	int enabledSidesTriangulationBitMask)
{
	int countX = voxelsGrid->count.x;
	int countY = voxelsGrid->count.y;
	int countZ = voxelsGrid->count.z;
	Voxel* voxelsGrid_data = voxelsGrid->data;
	Voxel::SidesConfig* voxelsSideConfigsGrid_data = voxelsSideConfigsGrid->data;

	for (int x = 0; x < countX; x++) {
		for (int y = 0; y < countY; y++) {
			for (int z = 0; z < countZ; z++) {
				int index = z * (countX * countY) + y * countX + x;

				Voxel* voxel = &voxelsGrid_data[index];
				Voxel::SidesConfig* sideConfigs = &voxelsSideConfigsGrid_data[index];

				if (voxel->matIndex < 0) {
					discardedVoxelSidesCount += 6;

					continue;
				}

				// Up
				if (!sideConfigs->sidesList[VoxelSides::Up].isDiscarded
					&& ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_zMax) == 0
						|| (((z + 1) < countZ) && voxelsGrid_data[index + countX * countY].matIndex >= 0)))
				{
					sideConfigs->sidesList[VoxelSides::Up].isDiscarded = true;
					discardedVoxelSidesCount++;
				}
				// Down
				if (!sideConfigs->sidesList[VoxelSides::Down].isDiscarded
					&& ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_zMin) == 0
						|| (((z - 1) >= 0) && voxelsGrid_data[index - countX * countY].matIndex >= 0)))
				{
					sideConfigs->sidesList[VoxelSides::Down].isDiscarded = true;
					discardedVoxelSidesCount++;
				}
				// Left
				if (!sideConfigs->sidesList[VoxelSides::Left].isDiscarded
					&& ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_xMin) == 0
						|| (((x - 1) >= 0) && voxelsGrid_data[index - 1].matIndex >= 0)))
				{
					sideConfigs->sidesList[VoxelSides::Left].isDiscarded = true;
					discardedVoxelSidesCount++;
				}
				// Right
				if (!sideConfigs->sidesList[VoxelSides::Right].isDiscarded
					&& ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_xMax) == 0
						|| (((x + 1) < countX) && voxelsGrid_data[index + 1].matIndex >= 0)))
				{
					sideConfigs->sidesList[VoxelSides::Right].isDiscarded = true;
					discardedVoxelSidesCount++;
				}
				// Front
				if (!sideConfigs->sidesList[VoxelSides::Front].isDiscarded
					&& ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_yMin) == 0
						|| (((y - 1) >= 0) && voxelsGrid_data[index - countX].matIndex >= 0)))
				{
					sideConfigs->sidesList[VoxelSides::Front].isDiscarded = true;
					discardedVoxelSidesCount++;
				}
				// Back
				if (!sideConfigs->sidesList[VoxelSides::Back].isDiscarded
					&& ((enabledSidesTriangulationBitMask & VoxelSides::BitGroup_yMax) == 0
						|| (((y + 1) < countY) && voxelsGrid_data[index + countX].matIndex >= 0)))
				{
					sideConfigs->sidesList[VoxelSides::Back].isDiscarded = true;
					discardedVoxelSidesCount++;
				}

				// Voxels are not removed, they just aren't rendered. If they are removed, extra inner voxel faces are rendered.
				/*if(voxel->voxelSides == 0) {
					row[y] = null;
				}*/
			}
		}
	}

	return 0;
}

int VoxlesGrid3DGeometry::optimizeVoxelGrid3DCombinedFaces(
	Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
	int& discardedVoxelSidesCount)
{
	int countX = voxelsGrid->count.x;
	int countY = voxelsGrid->count.y;
	int countZ = voxelsGrid->count.z;
	Voxel* voxelsGrid_data = voxelsGrid->data;
	Voxel::SidesConfig* voxelsSideConfigsGrid_data = voxelsSideConfigsGrid->data;

	for (int x = 0; x < countX; x++) {
		for (int y = 0; y < countY; y++) {
			for (int z = 0; z < countZ; z++) {
				int index = z * (countX * countY) + y * countX + x;

				Voxel* voxel = &voxelsGrid_data[index];
				Voxel::SidesConfig* sideConfigs = &voxelsSideConfigsGrid_data[index];

				if (voxel->matIndex < 0) {
					continue;
				}

				// Up
				if (!sideConfigs->sidesList[VoxelSides::Up].isDiscarded) {
					if (expandVoxelFace(
						voxel, sideConfigs,
						x, y, z,
						VoxelSides::Up,
						AXIS_X, AXIS_Y,
						voxelsGrid, voxelsSideConfigsGrid,
						discardedVoxelSidesCount
						) != 0) {
						return -1;
					}
				}
				// Down
				if (!sideConfigs->sidesList[VoxelSides::Down].isDiscarded) {
					if (expandVoxelFace(
						voxel, sideConfigs,
						x, y, z,
						VoxelSides::Down,
						AXIS_X, AXIS_Y,
						voxelsGrid, voxelsSideConfigsGrid,
						discardedVoxelSidesCount
						) != 0) {
						return -2;
					}
				}
				// Left
				if (!sideConfigs->sidesList[VoxelSides::Left].isDiscarded) {
					if (expandVoxelFace(
						voxel, sideConfigs,
						x, y, z,
						VoxelSides::Left,
						AXIS_Y, AXIS_Z,
						voxelsGrid, voxelsSideConfigsGrid,
						discardedVoxelSidesCount
						) != 0) {
						return -3;
					}
				}
				// Right
				if (!sideConfigs->sidesList[VoxelSides::Right].isDiscarded) {
					if (expandVoxelFace(
						voxel, sideConfigs,
						x, y, z,
						VoxelSides::Right,
						AXIS_Y, AXIS_Z,
						voxelsGrid, voxelsSideConfigsGrid,
						discardedVoxelSidesCount
						) != 0) {
						return -4;
					}
				}
				// Front
				if (!sideConfigs->sidesList[VoxelSides::Front].isDiscarded) {
					if (expandVoxelFace(
						voxel, sideConfigs,
						x, y, z,
						VoxelSides::Front,
						AXIS_X, AXIS_Z,
						voxelsGrid, voxelsSideConfigsGrid,
						discardedVoxelSidesCount
						) != 0) {
						return -5;
					}
				}
				// Back
				if (!sideConfigs->sidesList[VoxelSides::Back].isDiscarded) {
					if (expandVoxelFace(
						voxel, sideConfigs,
						x, y, z,
						VoxelSides::Back,
						AXIS_X, AXIS_Z,
						voxelsGrid, voxelsSideConfigsGrid,
						discardedVoxelSidesCount
						) != 0) {
						return -6;
					}
				}
			}
		}
	}

	return 0;
}

inline int VoxlesGrid3DGeometry::expandVoxelFace(
	Voxel* startVoxel, Voxel::SidesConfig* sideConfigs,
	const int x, const int y, const int z,
	const int voxelSide,
	const int axisA, const int axisB,
	Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
	int& discardedVoxelSidesCount)
{
	// 0 : both axis;
	// 1 : fixed to axisA;
	// 2 : fixed to axisB;
	int axisGrowType = 0;

	while (true) {
		if (axisGrowType == 0) {
			int r = growVoxelFaceOnAxis(
				startVoxel, sideConfigs, x, y, z,
				voxelSide,
				voxelsGrid, voxelsSideConfigsGrid,
				axisA, axisB, axisA, axisB,
				discardedVoxelSidesCount
				);
			bool growResultA;
			if (r == 0) {
				growResultA = false;
			} else if (r == 1) {
				growResultA = true;
			} else {
				return -1;
			}

			r = growVoxelFaceOnAxis(
				startVoxel, sideConfigs, x, y, z,
				voxelSide,
				voxelsGrid, voxelsSideConfigsGrid,
				axisB, axisA, axisA, axisB,
				discardedVoxelSidesCount
				);
			bool growResultB;
			if (r == 0) {
				growResultB = false;
			} else if (r == 1) {
				growResultB = true;
			} else {
				return -1;
			}

			if (!growResultA || !growResultB) {
				if (!growResultA && !growResultB) {
					// Grow ended.

					break;
				} else if (!growResultA) {
					axisGrowType = 2;
				} else {//!growResultB
					axisGrowType = 1;
				}
			}
		} else if (axisGrowType == 1) {
			int r = growVoxelFaceOnAxis(
				startVoxel, sideConfigs, x, y, z,
				voxelSide,
				voxelsGrid, voxelsSideConfigsGrid,
				axisA, axisB, axisA, axisB,
				discardedVoxelSidesCount
				);
			bool growResultA;
			if (r == 0) {
				growResultA = false;
			} else if (r == 1) {
				growResultA = true;
			} else {
				return -1;
			}

			if (!growResultA) {
				break;
			}
		} else {//axisGrowType == 2
			int r = growVoxelFaceOnAxis(
				startVoxel, sideConfigs,
				x, y, z,
				voxelSide,
				voxelsGrid, voxelsSideConfigsGrid,
				axisB, axisA, axisA, axisB,
				discardedVoxelSidesCount
				);
			bool growResultB;
			if (r == 0) {
				growResultB = false;
			} else if (r == 1) {
				growResultB = true;
			} else {
				return -1;
			}

			if (!growResultB) {
				break;
			}
		}
	}

	return 0;
}

// <0; Error code.
// 0; Grow result false.
// 1; Grow result true.
inline int VoxlesGrid3DGeometry::growVoxelFaceOnAxis(
	Voxel* startVoxel, Voxel::SidesConfig* sideConfigs,
	const int x, const int y, const int z,
	const int voxelSide,
	Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
	const int growAxisDirection, const int secondaryGrowthAxis,
	const int axisA, const int axisB,
	int& discardedVoxelSidesCount
	) {
	// Enumerate all needed voxels and check if expansion is possible.
	int faceGrowVoxelsCount;
	int faceLineOffset;

	Voxel::SideConfig& startVoxelSide = sideConfigs->sidesList[voxelSide];
	if (growAxisDirection == axisA) {
		faceGrowVoxelsCount = (startVoxelSide.faceExpansionB + 1);
		faceLineOffset = (startVoxelSide.faceExpansionA + 1);
	} else {//growAxisDirection == axisB
		faceGrowVoxelsCount = (startVoxelSide.faceExpansionA + 1);
		faceLineOffset = (startVoxelSide.faceExpansionB + 1);
	}

	// This needs to check an entire line or column that it is the desired matId so it can be expanded on.
	// Need to run the loop once to check and another time to update the discarded sides.
	for (int i = 0; i < faceGrowVoxelsCount; i++) {
		int nextGX = x;
		int nextGY = y;
		int nextGZ = z;

		if (secondaryGrowthAxis == AXIS_X) {
			nextGX += i;
		} else if (secondaryGrowthAxis == AXIS_Y) {
			nextGY += i;
		} else if (secondaryGrowthAxis == AXIS_Z) {
			nextGZ += i;
		} else {
			return -1;
		}

		if (growAxisDirection == AXIS_X) {
			nextGX += faceLineOffset;
		} else if (growAxisDirection == AXIS_Y) {
			nextGY += faceLineOffset;
		} else if (growAxisDirection == AXIS_Z) {
			nextGZ += faceLineOffset;
		} else {
			return -2;
		}

		if (nextGX < 0 || nextGX >= voxelsGrid->count.x || nextGY < 0 || nextGY >= voxelsGrid->count.y || nextGZ < 0 || nextGZ >= voxelsGrid->count.z) {
			return 0;
		}

		int nextVoxelIndex = nextGZ * (voxelsGrid->count.x * voxelsGrid->count.y) + nextGY * voxelsGrid->count.x + nextGX;
		Voxel* nextVoxel = &voxelsGrid->data[nextVoxelIndex];
		if (nextVoxel->matIndex < 0) {
			return 0;
		}
		if (nextVoxel->matIndex != startVoxel->matIndex) {
			return 0;
		}

		Voxel::SidesConfig* nextSideConfigs = &voxelsSideConfigsGrid->data[nextVoxelIndex];
		if (nextSideConfigs->sidesList[voxelSide].isDiscarded) {
			return 0;
		}
	}

	for (int i = 0; i < faceGrowVoxelsCount; i++) {
		int nextGX = x;
		int nextGY = y;
		int nextGZ = z;

		if (secondaryGrowthAxis == AXIS_X) {
			nextGX += i;
		} else if (secondaryGrowthAxis == AXIS_Y) {
			nextGY += i;
		} else if (secondaryGrowthAxis == AXIS_Z) {
			nextGZ += i;
		} else {
			return -1;
		}

		if (growAxisDirection == AXIS_X) {
			nextGX += faceLineOffset;
		} else if (growAxisDirection == AXIS_Y) {
			nextGY += faceLineOffset;
		} else if (growAxisDirection == AXIS_Z) {
			nextGZ += faceLineOffset;
		} else {
			return -2;
		}

		int nextVoxelIndex = nextGZ * (voxelsGrid->count.x * voxelsGrid->count.y) + nextGY * voxelsGrid->count.x + nextGX;
		Voxel::SidesConfig* nextSideConfigs = &voxelsSideConfigsGrid->data[nextVoxelIndex];

		nextSideConfigs->sidesList[voxelSide].isDiscarded = true;
		discardedVoxelSidesCount++;
	}

	// Expand the start voxel side by 1.
	if (growAxisDirection == axisA) {
		startVoxelSide.faceExpansionA++;
	} else {//growAxisDirection == axisB
		startVoxelSide.faceExpansionB++;
	}

	return 1;
}

void VoxlesGrid3DGeometry::invalidate(int invalidationBitMap) {
	this->invalidationBitMap = BoolUtil::combine(
		this->invalidationBitMap,
		invalidationBitMap
		);

	handler->onGeometryInvalidation();
}

ArrayList<VoxlesGrid3DGeometry::Material*>* VoxlesGrid3DGeometry::peekMaterialsList() {
	return &materialsList;
}

VoxlesGrid3DGeometry::Material* VoxlesGrid3DGeometry::peekMaterial(int materialIndex) {
	return materialsList.getDirect(materialIndex, nullptr);
}

void VoxlesGrid3DGeometry::clearMaterials() {
	for (int i = materialsList.size() - 1; i >= 0; i--) {
		Material* mat = materialsList.getDirect(0);
		materialsList.remove(0);

		if (mat) {
			delete mat;
		}
	}
}

int VoxlesGrid3DGeometry::pushNullMaterial() {
	int newMatId = materialsList.size();

	if (newMatId >= MATERIALS_COUNT_LIMIT) {
		// Too many materials created. Maybe increase the limit.
		throw LogicException(LOC);
	}

	materialsList.appendDirect(nullptr);

	return newMatId;
}

int VoxlesGrid3DGeometry::pushMaterial(Color& color) {
	int newMatId = materialsList.size();

	if (newMatId >= MATERIALS_COUNT_LIMIT) {
		// Too many materials created. Maybe increase the limit.
		throw LogicException(LOC);
	}

	materialsList.appendDirect(new Material(color));

	return newMatId;
}

int VoxlesGrid3DGeometry::updateMaterialColor(int matId, Color& color) {
	Material* mat = materialsList.getDirect(matId);
	if (!mat) {
		throw LogicException(LOC);
	}

	mat->color.set(color);

	invalidate(BoolUtil::setBitAtIndexDirect(0,
		InvalidationFlags::vertices
	));

	return 0;
}

void VoxlesGrid3DGeometry::removeUnusedMaterial(int matId) {
	Material* mat = materialsList.getDirect(matId);
	if (mat == nullptr) {
		throw LogicException(LOC);
	}

	materialsList.setDirect(matId, nullptr);
	delete mat;

	// Remove trailing null entries.
	// Leave the first listEntry to it's original value of 0;
	while (materialsList.size() > 1 && materialsList.getDirect(materialsList.size() - 1, nullptr) == nullptr) {
		materialsList.remove(materialsList.size() - 1);
	}
}

void VoxlesGrid3DGeometry::removeAllMaterials() {
	while (materialsList.count() > 0) {
		Material* mat = materialsList.getDirect(materialsList.count() - 1, nullptr);
		materialsList.remove(materialsList.count() - 1);
		if (mat != nullptr) {
			delete mat;
		}
	}
}

void VoxlesGrid3DGeometry::setMaterialsListCloneMode(ArrayList<Material*>* materialsList) {
	removeAllMaterials();

	for (int i = 0; i < materialsList->count(); i++) {
		Material* mat = materialsList->getDirect(i);
		if (mat != nullptr) {
			this->materialsList.appendDirect(new Material(mat->color));
		} else {
			this->materialsList.appendDirect(nullptr);
		}
	}

	invalidate(BoolUtil::setBitAtIndexDirect(0,
		InvalidationFlags::vertices
	));
}

void VoxlesGrid3DGeometry::setMaterialsListCloneMode(ArrayList<Visual3DModelMaterial*>* materialsList) {
	removeAllMaterials();

	int matId;
	for (int i = 0; i < materialsList->size(); i++) {
		Visual3DModelMaterial* mat = *materialsList->getPointer(i);

		Visual3DModelMaterial_Color* lMat_color;
		if (!mat) {
			matId = pushNullMaterial();
		} else if ((lMat_color = dynamic_cast<Visual3DModelMaterial_Color*>(mat))) {
			matId = pushMaterial(*lMat_color->diffuseColor);
		} else {
			throw LogicException(LOC);
		}
	}
}

void VoxlesGrid3DGeometry::setVoxelsGrid(Array3D<signed char>* _voxelGrid) {
	// Update the voxels.
	voxelsGrid.resizeNoElementsMove(_voxelGrid->getLengthA(), _voxelGrid->getLengthB(), _voxelGrid->getLengthC());
	voxelsSideConfigsGrid.resizeNoElementsMove(_voxelGrid->getLengthA(), _voxelGrid->getLengthB(), _voxelGrid->getLengthC());

	for (int x = 0; x < _voxelGrid->getLengthA(); x++) {
		for (int y = 0; y < _voxelGrid->getLengthB(); y++) {
			for (int z = 0; z < _voxelGrid->getLengthC(); z++) {
				int matId = _voxelGrid->getDirect(x, y, z);

				Voxel* voxel = voxelsGrid.getPointer(x, y, z);
				voxel->matIndex = matId;
			}
		}
	}

	invalidate(BoolUtil::setBitAtIndexDirect(0,
		InvalidationFlags::gridOptimisation,
		InvalidationFlags::vertices
	));
}

void VoxlesGrid3DGeometry::setVoxelsGrid(graphics::TilingInformation* tilingInformation) {
	if (tilingInformation->getType() == graphics::TilingInformation_LinearX::TYPE) {
		graphics::TilingInformation_LinearX* cTilingInformation = tilingInformation->castAs<graphics::TilingInformation_LinearX*>(graphics::TilingInformation_LinearX::TYPE);

		enabledSidesTriangulationBitMask = cTilingInformation->enabledSidesTriangulationBitMask;

		Vector3Int voxelsGridSize{
			cTilingInformation->tileVoxelsCount.x * cTilingInformation->tilingLengthCount,
			cTilingInformation->tileVoxelsCount.y,
			cTilingInformation->tileVoxelsCount.z
		};

		voxelsGrid.resizeNoElementsMove(voxelsGridSize.x, voxelsGridSize.y, voxelsGridSize.z);
		voxelsSideConfigsGrid.resizeNoElementsMove(voxelsGridSize.x, voxelsGridSize.y, voxelsGridSize.z);

		for (int tileIndex = 0; tileIndex < cTilingInformation->tilingLengthCount; tileIndex++) {
			for (int x = 0; x < cTilingInformation->tileVoxelsCount.x; x++) {
				for (int y = 0; y < cTilingInformation->tileVoxelsCount.y; y++) {
					for (int z = 0; z < cTilingInformation->tileVoxelsCount.z; z++) {
						int matId = cTilingInformation->tileConfig->voxelsGrid->getDirect(x, y, z);

						voxelsGrid.getReference(tileIndex * cTilingInformation->tileVoxelsCount.x + x, y, z).matIndex = matId;
					}
				}
			}
		}
	} else if (tilingInformation->getType() == graphics::TilingInformation_GridXY::TYPE) {
		graphics::TilingInformation_GridXY* cTilingInformation = tilingInformation->castAs<graphics::TilingInformation_GridXY*>(graphics::TilingInformation_GridXY::TYPE);

		enabledSidesTriangulationBitMask = cTilingInformation->enabledSidesTriangulationBitMask;

		Vector3Int voxelsGridSize{
			cTilingInformation->tileVoxelsCount.x * cTilingInformation->tilingGrid->getLengthA(),
			cTilingInformation->tileVoxelsCount.y * cTilingInformation->tilingGrid->getLengthB(),
			cTilingInformation->tileVoxelsCount.z
		};

		voxelsGrid.resizeNoElementsMove(voxelsGridSize.x, voxelsGridSize.y, voxelsGridSize.z);
		voxelsSideConfigsGrid.resizeNoElementsMove(voxelsGridSize.x, voxelsGridSize.y, voxelsGridSize.z);

		Vector2Int tilingCount{ cTilingInformation->tilingGrid->getLengthA(), cTilingInformation->tilingGrid->getLengthB() };
		for (int tileX = 0; tileX < tilingCount.x; tileX++) {
			for (int tileY = 0; tileY < tilingCount.y; tileY++) {
				int tileTypeIndex = cTilingInformation->tilingGrid->getDirect(tileX, tileY);

				graphics::TilingInformation::TileConfig* tileConfig = tileTypeIndex >= 0 ? cTilingInformation->tilesConfigList->getPointer(tileTypeIndex) : nullptr;
				if (tileConfig != nullptr && tileConfig->voxelsGrid != nullptr) {
					// Copy the tile grid.
					for (int x = 0; x < cTilingInformation->tileVoxelsCount.x; x++) {
						for (int y = 0; y < cTilingInformation->tileVoxelsCount.y; y++) {
							for (int z = 0; z < cTilingInformation->tileVoxelsCount.z; z++) {
								int matId = tileConfig->voxelsGrid->getDirect(x, y, z);

								voxelsGrid.getReference(
									tileX * cTilingInformation->tileVoxelsCount.x + x,
									tileY * cTilingInformation->tileVoxelsCount.y + y,
									z
									).matIndex = matId;
							}
						}
					}
				} else {
					// Fill with -1 index.
					for (int x = 0; x < cTilingInformation->tileVoxelsCount.x; x++) {
						for (int y = 0; y < cTilingInformation->tileVoxelsCount.y; y++) {
							for (int z = 0; z < cTilingInformation->tileVoxelsCount.z; z++) {
								voxelsGrid.getReference(
									tileX * cTilingInformation->tileVoxelsCount.x + x,
									tileY * cTilingInformation->tileVoxelsCount.y + y,
									z
									).matIndex = -1;
							}
						}
					}
				}
			}
		}

		if (cTilingInformation->geometryGridOffset != nullptr) {
			setVoxelGridOffset(*cTilingInformation->geometryGridOffset);
		}
	} else {
		throw LogicException(LOC);
	}

	invalidate(BoolUtil::setBitAtIndexDirect(0,
		InvalidationFlags::gridOptimisation,
		InvalidationFlags::vertices
	));
}

void VoxlesGrid3DGeometry::setVoxelsGridCloneMode(Array3D<Voxel>& _voxelGrid) {
	// Update the voxels.
	voxelsGrid.resizeNoElementsMove(_voxelGrid.getLengthA(), _voxelGrid.getLengthB(), _voxelGrid.getLengthC());
	voxelsSideConfigsGrid.resizeNoElementsMove(_voxelGrid.getLengthA(), _voxelGrid.getLengthB(), _voxelGrid.getLengthC());

	for (int x = 0; x < _voxelGrid.getLengthA(); x++) {
		for (int y = 0; y < _voxelGrid.getLengthB(); y++) {
			for (int z = 0; z < _voxelGrid.getLengthC(); z++) {
				Voxel* srcVoxel = _voxelGrid.getPointer(x, y, z);

				Voxel* destVoxel = voxelsGrid.getPointer(x, y, z);
				destVoxel->matIndex = srcVoxel->matIndex;
			}
		}
	}
	invalidate(BoolUtil::setBitAtIndexDirect(0,
		InvalidationFlags::gridOptimisation,
		InvalidationFlags::vertices
	));
}

int VoxlesGrid3DGeometry::getVoxelMaterialIndex(int gX, int gY, int gZ) {
	if (gX < 0 || gY < 0 || gZ < 0
		|| gX >= voxelsGrid.count.x
		|| gY >= voxelsGrid.count.y
		|| gZ >= voxelsGrid.count.z)
	{
		// If voxel is out of bounds, return a -1 mat index;
		return -1;
	}

	Voxel* voxel = voxelsGrid.getPointer(gX, gY, gZ);
	if (voxel == nullptr) {
		throw LogicException(LOC);
	}

	return voxel->matIndex;
}

int VoxlesGrid3DGeometry::setVoxel(int gX, int gY, int gZ, int matId, Vector3Int* postChangeGridBoundsMin) {
	if (matId != -1 && (matId < 0 || matId >= materialsList.size())) {
		// MatId is not in the range [-1, materialsList.size());
		return -1;
	}

	bool isInvalidationRequired = false;

	// If out of bounds, pre-expand the grid in that direction.
	if (gX < 0 || gY < 0 || gZ < 0
		|| gX >= voxelsGrid.count.x || gY >= voxelsGrid.count.y || gZ >= voxelsGrid.count.z)
	{
		if (matId == -1) {
			// Trying to clear a voxel that out of the grid range. There is no change to the grid.
			return 0;
		}

		tmp_gridAreaBounds.set(
			0, voxelsGrid.count.x,
			0, voxelsGrid.count.y,
			0, voxelsGrid.count.z
		);

		tmp_activeGridAreaBounds.set(
			tmp_gridAreaBounds
		)->unionA(
			gX, gX + 1,
			gY, gY + 1,
			gZ, gZ + 1
		);

		voxelsGrid.resizeMarginOffsets(tmp_activeGridAreaBounds);
		voxelsSideConfigsGrid.resizeMarginOffsets(tmp_activeGridAreaBounds);

		// Update the grid size to a new smaller/larger size.

		setVoxelGridOffset(
			voxelGridOffset.x + tmp_activeGridAreaBounds.min.x,
			voxelGridOffset.y + tmp_activeGridAreaBounds.min.y,
			voxelGridOffset.z + tmp_activeGridAreaBounds.min.z
			);

		gX -= tmp_activeGridAreaBounds.min.x;
		gY -= tmp_activeGridAreaBounds.min.y;
		gZ -= tmp_activeGridAreaBounds.min.z;

		if (postChangeGridBoundsMin != nullptr) {
			postChangeGridBoundsMin->set(tmp_activeGridAreaBounds.min);
		}

		isInvalidationRequired = true;

		if (AppConfig::IS_DEBUG && AppConfig::IS_DEBUG_EXTRA_LOGIC_MESSAGES) {
			GlobalAccessLogHandler::devLogHandler->postDebug("Entity voxels grid resized 1. New size (x: " + std::to_string(tmp_activeGridAreaBounds.getLenX()) + ", y: " + std::to_string(tmp_activeGridAreaBounds.getLenY()) + ", z: " + std::to_string(tmp_activeGridAreaBounds.getLenZ()) + "), Old size (x: " + std::to_string(tmp_gridAreaBounds.getLenX()) + ", y: " + std::to_string(tmp_gridAreaBounds.getLenY()) + ", z: " + std::to_string(tmp_gridAreaBounds.getLenZ()) + "), Offset (x: " + std::to_string(tmp_activeGridAreaBounds.min.x) + ", y: " + std::to_string(tmp_activeGridAreaBounds.min.y) + ", z: " + std::to_string(tmp_activeGridAreaBounds.min.z) + ")");
		}
	}

	Voxel* voxel = voxelsGrid.getPointer(gX, gY, gZ);
	if (voxel == nullptr) {
		return -1;
	}

	int old_matIndex = voxel->matIndex;
	if (matId != voxel->matIndex) {
		voxel->matIndex = matId;

		isInvalidationRequired = true;
	}

	bool isOnBounds = false;
	if (gX == 0 || gY == 0 || gZ == 0
		|| gX == voxelsGrid.count.x - 1 || gY == voxelsGrid.count.y - 1 || gZ == voxelsGrid.count.z - 1)
	{
		isOnBounds = true;
	}
	if (isOnBounds && ((old_matIndex < 0 && matId >= 0) || (old_matIndex >= 0 && matId < 0))) {
		// Compute grid bounds.
		tmp_gridAreaBounds.set(
			0, voxelsGrid.count.x,
			0, voxelsGrid.count.y,
			0, voxelsGrid.count.z
		);

		computeUsedGridAreaBounds(&voxelsGrid, tmp_activeGridAreaBounds);

		if (!tmp_gridAreaBounds.equals(tmp_activeGridAreaBounds)) {
			// Update the grid size to a new smaller/larger size.
			voxelsGrid.resizeMarginOffsets(tmp_activeGridAreaBounds);
			voxelsSideConfigsGrid.resizeMarginOffsets(tmp_activeGridAreaBounds);

			setVoxelGridOffset(
				voxelGridOffset.x + tmp_activeGridAreaBounds.min.x,
				voxelGridOffset.y + tmp_activeGridAreaBounds.min.y,
				voxelGridOffset.z + tmp_activeGridAreaBounds.min.z
				);

			gX -= tmp_activeGridAreaBounds.min.x;
			(void)gX;
			gY -= tmp_activeGridAreaBounds.min.y;
			(void)gY;
			gZ -= tmp_activeGridAreaBounds.min.z;
			(void)gZ;

			if (postChangeGridBoundsMin != nullptr) {
				postChangeGridBoundsMin->set(tmp_activeGridAreaBounds.min);
			}

			isInvalidationRequired = true;

			if (AppConfig::IS_DEBUG && AppConfig::IS_DEBUG_EXTRA_LOGIC_MESSAGES) {
				GlobalAccessLogHandler::devLogHandler->postDebug("Entity voxels grid resized 2. New size (x: " + std::to_string(tmp_activeGridAreaBounds.getLenX()) + ", y: " + std::to_string(tmp_activeGridAreaBounds.getLenY()) + ", z: " + std::to_string(tmp_activeGridAreaBounds.getLenZ()) + "), Old size (x: " + std::to_string(tmp_gridAreaBounds.getLenX()) + ", y: " + std::to_string(tmp_gridAreaBounds.getLenY()) + ", z: " + std::to_string(tmp_gridAreaBounds.getLenZ()) + "), Offset (x: " + std::to_string(tmp_activeGridAreaBounds.min.x) + ", y: " + std::to_string(tmp_activeGridAreaBounds.min.y) + ", z: " + std::to_string(tmp_activeGridAreaBounds.min.z) + ")");
			}
		}
	}

	if (isInvalidationRequired) {
		invalidate(BoolUtil::setBitAtIndexDirect(0,
			InvalidationFlags::gridOptimisation,
			InvalidationFlags::vertices
		));
	}

	return 0;
}

void VoxlesGrid3DGeometry::setVoxelsRegion(Vector3Int& _destGPos, Array3D<signed char>& srcGrid, Vector3Int* oPostChangeGridBoundsMin) {
	Vector3Int destGPos{ _destGPos };

	for (int gX = 0; gX < srcGrid.getCountA(); gX++) {
		for (int gY = 0; gY < srcGrid.getCountB(); gY++) {
			for (int gZ = 0; gZ < srcGrid.getCountC(); gZ++) {
				int matId = srcGrid.getDirect(gX, gY, gZ);

				Vector3Int postChangeGridBoundsMin{};
				setVoxel(
					destGPos.x + gX/*gX*/, destGPos.y + gY/*gY*/, destGPos.z + gZ/*gZ*/,
					matId/*matId*/, &postChangeGridBoundsMin/*postChangeGridBoundsMin*/
				);

				destGPos.x -= postChangeGridBoundsMin.x;
				destGPos.y -= postChangeGridBoundsMin.y;

				if (oPostChangeGridBoundsMin != nullptr) {
					oPostChangeGridBoundsMin->add(postChangeGridBoundsMin);
				}
			}
		}
	}
}

void VoxlesGrid3DGeometry::setMaterialsListAndVoxelsGridCloneMode(ArrayList<Material*>* materialsList, Array3D<Voxel>* _voxelGrid) {
	setMaterialsListCloneMode(materialsList);

	setVoxelsGridCloneMode(*_voxelGrid);
}

void VoxlesGrid3DGeometry::setMaterialsListAndVoxelsGridCloneMode(ArrayList<Visual3DModelMaterial*>* materialsList, Array3D<signed char>* _voxelGrid) {
	setMaterialsListCloneMode(materialsList);

	setVoxelsGrid(_voxelGrid);
}

void VoxlesGrid3DGeometry::computeUsedGridAreaBounds(Array3D<Voxel>* grid, Bounds3DInt& destActiveGridAreaBounds) {
	bool isFirstProcessed = false;

	for (int x = 0; x < grid->count.x; x++) {
		for (int y = 0; y < grid->count.y; y++) {
			for (int z = 0; z < grid->count.z; z++) {
				// Src value.
				Voxel* voxel = grid->getPointer(x, y, z);

				if (voxel->matIndex >= 0) {
					if (!isFirstProcessed) {
						isFirstProcessed = true;

						destActiveGridAreaBounds.set(
							x, x + 1,
							y, y + 1,
							z, z + 1
							);
					} else {
						// Add "+1" because the bounds include a size component.
						destActiveGridAreaBounds.unionA(
							x, x + 1,
							y, y + 1,
							z, z + 1
							);
					}
				}
			}
		}
	}
}

int VoxlesGrid3DGeometry::setVoxelSize(float voxelSize) {
	if (this->voxelSize == voxelSize) {
		return 0;
	}

	this->voxelSize = voxelSize;

	invalidate(BoolUtil::setBitAtIndexDirect(0,
		InvalidationFlags::vertices
	));

	return 0;
}

void VoxlesGrid3DGeometry::setVoxelGridOffset(float gX, float gY, float gZ) {
	if (voxelGridOffset.x == gX && voxelGridOffset.y == gY && voxelGridOffset.z == gZ) {
		return;
	}

	voxelGridOffset.set(gX, gY, gZ);

	invalidate(BoolUtil::setBitAtIndexDirect(0,
		InvalidationFlags::vertices
	));
}

void VoxlesGrid3DGeometry::setVoxelGridOffset(Vector3& gridOffset) {
	return setVoxelGridOffset(gridOffset.x, gridOffset.y, gridOffset.z);
}

void VoxlesGrid3DGeometry::translateVoxelGrid(float gX, float gY, float gZ) {
	setVoxelGridOffset(
		voxelGridOffset.x + gX,
		voxelGridOffset.y + gY,
		voxelGridOffset.z + gZ
		);
}

void VoxlesGrid3DGeometry::reservedDisposeMain() {
	disposeMain();
	disposeVerticesAndIndices();
}

void VoxlesGrid3DGeometry::disposeMain() {
	removeAllMaterials();
}

void VoxlesGrid3DGeometry::disposeVerticesAndIndices() {
	if (verticesArray) {
		delete[] verticesArray;
		verticesArray = nullptr;

		verticesCount = 0;
	}
	// Indices are currently unmanaged, don't dispose.
	/*if (indicesArrayUnmanaged) {
		delete[] indicesArrayUnmanaged;
		indicesArrayUnmanaged = nullptr;

		indicesCount = 0;
	}*/
}

VoxlesGrid3DGeometry::~VoxlesGrid3DGeometry() {
	//void
}
