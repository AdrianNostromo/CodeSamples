#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include "IEditableVoxelisedM_Edit.h"

namespace rpg3D {
class IEditableVoxelisedModule : public virtual IEntityModule, virtual public IEditableVoxelisedM_Edit {priv typedef IComponent super;
    pub static int TYPE;
    DEC_componentBasics();

	//void

    pub ~IEditableVoxelisedModule() override = default;
};
};
