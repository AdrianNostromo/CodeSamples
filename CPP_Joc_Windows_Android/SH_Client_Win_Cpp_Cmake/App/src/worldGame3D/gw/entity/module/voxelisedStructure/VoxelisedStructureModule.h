#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IVoxelisedStructureModule.h"
#include <worldGame3D/gw/entity/template/voxelisedStructure/VoxelisedStructureTemplate.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>

class IWorldEntity;
class IReferenceFrameModule;

class VoxelisedStructureModule : public EntityModule, public virtual IVoxelisedStructureModule {priv typedef EntityModule super;pub dCtor(VoxelisedStructureModule);
    pub VoxelisedStructureTemplate* t;

    priv Vector3 structureGridOffset{0.5f, 0.5f, -0.5f};
    priv float structureVoxelSize = 0.1f;
    priv Array3D<signed char> structureGrid{1, 1, 1};
    priv ArrayList<Visual3DModelMaterial*> structureMaterialsList{};

    priv IContainer3D* voxelisedConstructLayer = nullptr;

    priv VoxelGridDrawable3D* vGridVisual = nullptr;

    pub VoxelisedStructureModule(
        IWorldEntity* entity,
        VoxelisedStructureTemplate* t
    );
    prot void createBMain() override;

    pub void setStructure(
        Array3D<signed char>* srcStructureGrid,
        ArrayList<Visual3DModelMaterial*>* srcStructureMaterialsList) final;

    pub ~VoxelisedStructureModule() override;
};
