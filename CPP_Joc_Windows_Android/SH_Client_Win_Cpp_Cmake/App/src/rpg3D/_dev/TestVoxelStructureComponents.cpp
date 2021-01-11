#include <worldGame3D/gw/voxelisedstructure/VSVoxelType.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include "TestVoxelStructureComponents.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace rpg3D;

Array3D<signed char>* TestVoxelStructureComponents::groundPatchGrid = newGroundPatchGrid();
Array3D<signed char>* TestVoxelStructureComponents::groundPatchGrid6x6 = newGroundPatchGrid6x6();

ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::grassAMaterialsList = newGrassAMaterialsList();
ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::grassBMaterialsList = gewGrassBMaterialsList();
ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::waterAMaterialsList = newWaterAMaterialsList();
ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::deepWaterAMaterialsList = newDeepWaterAMaterialsList();

Array3D<signed char>* TestVoxelStructureComponents::testStructureGrid = newTestStructureGrid();
ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::testStructureMaterialsList = newTestStructureMaterialsList();

Array3D<signed char>* TestVoxelStructureComponents::newGroundPatchGrid() {
    const signed char O = 0;
    (void)O;
    const signed char W = 1;

    Array3D<signed char>* grid = newt Array3D<signed char>(
        10, 10, 1,
        false, false, false,
        new signed char[10 * 10 * 1] {
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W
        }
    );

    return grid;
}

Array3D<signed char>* TestVoxelStructureComponents::newGroundPatchGrid6x6() {
    const signed char O = 0;
    (void)O;
    const signed char W = 1;

    Array3D<signed char>* grid = newt Array3D<signed char>(
        6, 6, 1,
        false, false, false,
        new signed char[6 * 6 * 1] {
            W,W,W,W,W,W,
            W,W,W,W,W,W,
            W,W,W,W,W,W,
            W,W,W,W,W,W,
            W,W,W,W,W,W,
            W,W,W,W,W,W
        }
    );

    return grid;
}

ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::newGrassAMaterialsList() {
    ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(
        nullptr
    );
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#78C214FF")
    ));

    return materialsList;
}

ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::gewGrassBMaterialsList() {
    ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(
        nullptr
    );
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#70B80DFF")
    ));

    return materialsList;
}

ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::newWaterAMaterialsList() {
    ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(
        nullptr
    );
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#71BFF5FF")
    ));

    return materialsList;
}

ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::newDeepWaterAMaterialsList() {
    ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();
    materialsList->appendDirect(
        nullptr
    );
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#4ba0dbFF")
    ));

    return materialsList;
}

Array3D<signed char>* TestVoxelStructureComponents::newTestStructureGrid() {
    const signed char O = VSVoxelType::Empty;
    const signed char W = VSVoxelType::Wall;

    Array3D<signed char>* grid = newt Array3D<signed char>(
        30, 30, 1,
        false, false, true,
        new signed char[30 * 30 * 1] {
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,

            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,
            O,O,O,O,O,O,O,O,O,O, W,W,W,W,W,W,W,W,W,W, O,O,O,O,O,O,O,O,O,O,

            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W,
            W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W, W,W,W,W,W,W,W,W,W,W
        }
    );

    return grid;
}

ArrayList<Visual3DModelMaterial*>* TestVoxelStructureComponents::newTestStructureMaterialsList() {
    ArrayList<Visual3DModelMaterial*>* materialsList = newt ArrayList<Visual3DModelMaterial*>();

    // VSVoxelType::Empty
    materialsList->appendDirect(
        nullptr
    );
    // VSVoxelType::Wall
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#7DB45BFF")
    ));
    // VSVoxelType::ElectricCable_Zero
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#C2C2C2FF")
    ));
    // VSVoxelType::ElectricCable_Positive
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#16BBD1FF")
    ));
    // VSVoxelType::SolarCellMaterial_Idle
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#174C9AFF")
    ));
    // VSVoxelType::SolarCellMaterial_Charged
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#16BBD1FF")
    ));
    // VSVoxelType::IonEngineMaterial_Zero
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#FFFFFFFF")
    ));
    // VSVoxelType::IonEngineMaterial_Active
    materialsList->appendDirect(new Visual3DModelMaterial_Color(
        new Color("#16BBD1FF")
    ));

    return materialsList;
}
