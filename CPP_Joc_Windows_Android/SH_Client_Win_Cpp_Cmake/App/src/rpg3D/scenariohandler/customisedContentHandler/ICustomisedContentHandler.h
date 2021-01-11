#pragma once

#include <base/gh.h>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>
#include <memory>

class Visual3DModelMaterial;
class VoxlesGrid3DGeometry;
namespace base {
    class IGameWorld;
    class GWEvent;
};

namespace rpg3D {
class ICustomisedContentHandler {
    pub class EntityMaterialsAndVoxelsGrid {pub dCtor(EntityMaterialsAndVoxelsGrid);
        pub Array3D<signed char>* voxelGrid = nullptr;
        pub ArrayList<Visual3DModelMaterial*>* materialsList = nullptr;

        // These are used to keep the above variables active.
        // Don't use these 2 variables.
        pub std::shared_ptr < Array3D<signed char>> _spRef_voxelGrid = nullptr;
        pub std::shared_ptr<ArrayList<Visual3DModelMaterial*>> _spRef_materialsList = nullptr;

        pub explicit EntityMaterialsAndVoxelsGrid() 
        {};
    };

    pub static base::GWEvent* GWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded;
    pub static base::GWEvent* GWEvent_PlayerUnitMaterialsAndVoxelsGrid_edited;

    pub virtual std::shared_ptr<EntityMaterialsAndVoxelsGrid> getPlayerUnitMaterialsAndVoxelsGrid() = 0;
    pub virtual void updatePlayerUnitMaterialsAndVoxelsGrid(VoxlesGrid3DGeometry* voxels3DGeometry, bool dispatchGWUpdateEvent) = 0;

    pub virtual void setActiveGameWorld(base::IGameWorld* activeGameWorld) = 0;

    pub virtual ~ICustomisedContentHandler() = default;
};
};
