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
class IVoxelizedEditorM_Edit {
	pub virtual IWorldEntity* getEntity() = 0;

	pub virtual rpg3D::IEditableVoxelisedM_Edit* getEntityToEdit_editableEntity() = 0;

	pub virtual std::shared_ptr<graphics::ICinematicController> getCinematicController() = 0;

	pub virtual void editableVoxelized_onEditOccured(rpg3D::IEditableVoxelisedM_Edit* editableVoxelized) = 0;

    pub virtual ~IVoxelizedEditorM_Edit() = default;
};
};
