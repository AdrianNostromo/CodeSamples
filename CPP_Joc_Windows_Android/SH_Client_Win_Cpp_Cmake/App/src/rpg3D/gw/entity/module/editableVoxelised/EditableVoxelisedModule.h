#pragma once

#include <base/gh.h>
#include "EVM_RotationResetter.h"

namespace rpg3D {
class EditableVoxelisedModule : public EVM_RotationResetter {priv typedef EVM_RotationResetter super;pub dCtor(EditableVoxelisedModule);
    pub explicit EditableVoxelisedModule(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);

    pub ~EditableVoxelisedModule() override;
};
};
