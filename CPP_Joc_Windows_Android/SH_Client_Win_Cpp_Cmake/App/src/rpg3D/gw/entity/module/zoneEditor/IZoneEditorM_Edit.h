#pragma once

#include <base/gh.h>
#include <memory>

class IWorldEntity;
namespace graphics {
	class ICinematicController;
};
namespace rpg3D {
	class IEditableVoxelisedM_Edit;
};

namespace rpg3D {
class IZoneEditorM_Edit {
	pub virtual IWorldEntity* getEntity() = 0;

	pub virtual std::shared_ptr<graphics::ICinematicController> getCinematicController() = 0;

    pub virtual ~IZoneEditorM_Edit() = default;
};
};
