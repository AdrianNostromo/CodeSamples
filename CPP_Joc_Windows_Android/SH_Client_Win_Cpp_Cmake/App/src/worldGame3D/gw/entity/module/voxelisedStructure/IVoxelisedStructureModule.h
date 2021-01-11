#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>
#include <base/list/Array3D.h>
#include <base/list/ArrayList.h>

class Visual3DModelMaterial;

class IVoxelisedStructureModule : public virtual IEntityModule {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

    pub virtual void setStructure(
        Array3D<signed char>* srcStructureGrid,
        ArrayList<Visual3DModelMaterial*>* srcStructureMaterialsList) = 0;

    pub ~IVoxelisedStructureModule() override = default;
};
