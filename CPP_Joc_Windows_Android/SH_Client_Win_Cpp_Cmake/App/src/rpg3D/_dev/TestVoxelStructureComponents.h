#pragma once

#include <base/list/Array3D.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include <base/list/ArrayList.h>

namespace rpg3D {
class TestVoxelStructureComponents {
public:
    static Array3D<signed char>* groundPatchGrid;
    static Array3D<signed char>* groundPatchGrid6x6;

    static ArrayList<Visual3DModelMaterial*>* grassAMaterialsList;
    static ArrayList<Visual3DModelMaterial*>* grassBMaterialsList;
    static ArrayList<Visual3DModelMaterial*>* waterAMaterialsList;
    static ArrayList<Visual3DModelMaterial*>* deepWaterAMaterialsList;

    static Array3D<signed char>* testStructureGrid;
    static ArrayList<Visual3DModelMaterial*>* testStructureMaterialsList;

private:
	static Array3D<signed char>* newGroundPatchGrid();
	static Array3D<signed char>* newGroundPatchGrid6x6();

	static ArrayList<Visual3DModelMaterial*>* newGrassAMaterialsList();
	static ArrayList<Visual3DModelMaterial*>* gewGrassBMaterialsList();
	static ArrayList<Visual3DModelMaterial*>* newWaterAMaterialsList();
	static ArrayList<Visual3DModelMaterial*>* newDeepWaterAMaterialsList();

	static Array3D<signed char>* newTestStructureGrid();
	static ArrayList<Visual3DModelMaterial*>* newTestStructureMaterialsList();

};
};
