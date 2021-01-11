#pragma once

#include <base/gh.h>
#include "VEM_ColorsSelectorHandler.h"

namespace rpg3D {
class VoxelizedEditorModule : public VEM_ColorsSelectorHandler {priv typedef VEM_ColorsSelectorHandler super;pub dCtor(VoxelizedEditorModule);
    pub explicit VoxelizedEditorModule(
		IWorldEntity* entity,
		VoxelizedEditorTemplate* t);

    pub ~VoxelizedEditorModule() override;
};
};
