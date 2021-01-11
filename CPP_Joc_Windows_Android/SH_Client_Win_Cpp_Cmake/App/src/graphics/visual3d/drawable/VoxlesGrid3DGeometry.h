#pragma once

#include <base/gh.h>
#include <base/math/Vector3Int.h>
#include <graphics/visual/Color.h>
#include <base/list/ArrayList.h>
#include <base/list/Array3D.h>
#include <base/math/Bounds3DInt.h>
#include <memory>
#include <graphics/util/GLTypes.h>

class Visual3DModelMaterial;
namespace graphics {
	class TilingInformation;
};

class VoxlesGrid3DGeometry {pub dCtor(VoxlesGrid3DGeometry);
	prot class InvalidationFlags {
		// Compute structure.
		pub static const unsigned int gridOptimisation = 0;
		// Build quads.
		pub static const unsigned int vertices = 1;

		pub static const unsigned int LAST_USED_BIT_INDEX = vertices;
	};
	pub class IHandler {
		pub virtual void onGeometryInvalidation() = 0;

		pub virtual ~IHandler() = default;
	};
	pub class Material {pub dCtor(Material);
		pub Color color;

		pub explicit Material(Color& _color)
			: color(_color)
		{
			//void
		}
	};

	pub class Voxel {pub dCtor(Voxel);
		pub class SideConfig {pub dCtor(SideConfig);
			// Use isDiscarded instead of isUsed to be albe to use memset as an algorithm speed optimisation.
			pub bool isDiscarded = false;

			// This is an optimisation and requires a +1 to be manually added each time these values are read.
			// These were initialised and reset with a value 1.
			pub short faceExpansionA = 0;
			pub short faceExpansionB = 0;

			pub SideConfig() {
				//void
			}
		};
		pub class SidesConfig {pub dCtor(SidesConfig);
			// Has a size of 6;
			pub SideConfig sidesList[6];

			pub SidesConfig() {
				//void
			}
		};
		// <0; Not used.
		// >=0; Mat index;
		pub signed char matIndex;

		pub Voxel()
			: matIndex(-1)
		{
			//void
		}

		pub ~Voxel() {
			//void
		}
	};

	pub static const int MATERIALS_COUNT_LIMIT;

	pub static const int AXIS_X;
	pub static const int AXIS_Y;
	pub static const int AXIS_Z;

	pub static const Vector3 vFLD;
	pub static const Vector3 vFRD;
	pub static const Vector3 vFLU;
	pub static const Vector3 vFRU;

	pub static const Vector3 vBLD;
	pub static const Vector3 vBRD;
	pub static const Vector3 vBLU;
	pub static const Vector3 vBRU;

	priv IHandler * handler;

	priv ArrayList<Material*> materialsList{};

	// This is used to only create certain side in some situations.
	// This is usefull in some cases (eg. to not render the top and bottom sides of dirt that is covered by grass on top).
	priv int enabledSidesTriangulationBitMask = -1;
	// Use a small initial placeholder size.
	priv Array3D<Voxel> voxelsGrid{ 1, 1, 1 };
	priv Array3D<Voxel::SidesConfig> voxelsSideConfigsGrid{ 1, 1, 1 };

	priv float voxelSize = 1.0f;
	priv Vector3 voxelGridOffset{ 0.0f, 0.0f, 0.0f };

	priv graphics::GLTypes::Vertex_3DGenericTexture* verticesArray = nullptr;
	priv int verticesCount = 0;
	
	priv unsigned short* indicesArrayUnmanaged = nullptr;
	priv int indicesCount = 0;

	priv int invalidationBitMap = -1;

	priv Bounds3DInt tmp_gridAreaBounds{};
	priv Bounds3DInt tmp_activeGridAreaBounds{};

	pub explicit VoxlesGrid3DGeometry(IHandler* handler);
	
	pub Array3D<VoxlesGrid3DGeometry::Voxel>* peekGrid() { return &voxelsGrid; }

	pub int updateGeometry();
	pub int buildVerticesArray();

	pub graphics::GLTypes::Vertex_3DGenericTexture* getVerticesArray();
	pub int getVerticesCount();

	pub unsigned short* getIndicesArray();
	pub int getIndicesCount();

	pub ArrayList<Material*>* peekMaterialsList();
	pub Material* peekMaterial(int materialIndex);
	pub void clearMaterials();
	pub int pushNullMaterial();
	pub int pushMaterial(Color& color);
	pub int updateMaterialColor(int matId, Color& color);
	pub void removeUnusedMaterial(int matId);
	pub void removeAllMaterials();
	pub void setMaterialsListCloneMode(ArrayList<Material*>* materialsList);
	pub void setMaterialsListCloneMode(ArrayList<Visual3DModelMaterial*>* materialsList);

	pub void setVoxelsGrid(Array3D<signed char>* voxelGrid);
	pub void setVoxelsGrid(graphics::TilingInformation* tilingInformation);
	pub void setVoxelsGridCloneMode(Array3D<Voxel>& voxelGrid);
	pub int getVoxelMaterialIndex(int gX, int gY, int gZ);
	pub int setVoxel(int gX, int gY, int gZ, int matId, Vector3Int* postChangeGridBoundsMin);
	pub void setVoxelsRegion(Vector3Int& destGPos, Array3D<signed char>& srcGrid, Vector3Int* oPostChangeGridBoundsMin);

	pub void setMaterialsListAndVoxelsGridCloneMode(ArrayList<Material*>* materialsList, Array3D<Voxel>* voxelGrid);
	pub void setMaterialsListAndVoxelsGridCloneMode(ArrayList<Visual3DModelMaterial*>* materialsList, Array3D<signed char>* voxelGrid);

	priv static void computeUsedGridAreaBounds(Array3D<Voxel>* grid, Bounds3DInt& destActiveGridAreaBounds);

	pub float getVoxelSize() { return voxelSize; }
	pub int setVoxelSize(float voxelSize);

	pub Vector3* getVoxelGridOffset() { return &voxelGridOffset; }
	pub void setVoxelGridOffset(float gX, float gY, float gZ);
	pub void setVoxelGridOffset(Vector3& gridOffset);
	pub void translateVoxelGrid(float gX, float gY, float gZ);

	priv int update_optimiseGrid();
	priv int update_quads();

	priv void invalidate(int invalidationBitMap);
	
	priv static bool isVoxel(
		const int x, const int y, const int z, 
		Array3D<Voxel>* grid,
        ArrayList<Material*>& materialsList
	);
	
	//static std::string generateGridString(
	//	Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid);
	//static std::string generateGridUsedSidesString(
	//	Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
	//	ArrayList<Material*>& materialsList);
	priv static int optimizeVoxelGrid3DHiddenSides(
		Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
        ArrayList<Material*>& materialsList,
		int& discardedVoxelSidesCount,
		int enabledSidesTriangulationBitMask);
	priv static int optimizeVoxelGrid3DCombinedFaces(
		Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
		int& discardedVoxelSidesCount
	);
	priv static inline int expandVoxelFace(
		Voxel* startVoxel, Voxel::SidesConfig* sideConfigs,
		const int x, const int y, const int z,
		const int voxelSide,
		const int axisA, const int axisB,
		Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
		int& discardedVoxelSidesCount
	);
	priv static inline int growVoxelFaceOnAxis(
		Voxel* startVoxel, Voxel::SidesConfig* sideConfigs,
		const int x, const int y, const int z,
		const int voxelSide,
		Array3D<Voxel>* voxelsGrid, Array3D <Voxel::SidesConfig>* voxelsSideConfigsGrid,
		const int growAxisDirection, const int secondaryGrowthAxis,
		const int axisA, const int axisB,
		int& discardedVoxelSidesCount
	);
	
	pub void reservedDisposeMain();
	prot void disposeMain();
	prot void disposeVerticesAndIndices();
	pub ~VoxlesGrid3DGeometry();
};

