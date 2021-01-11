#pragma once

#include <base/gh.h>

class IWorldEntity;

namespace rpg3D {
class IHandlerVoxelizedEditorModule {
	pub virtual void entityEditor_requestStopEditor() = 0;
	pub virtual void entityEditor_processOnEditOccured(IWorldEntity* entityToEdit) = 0;

    pub virtual ~IHandlerVoxelizedEditorModule() = default;
};
};
