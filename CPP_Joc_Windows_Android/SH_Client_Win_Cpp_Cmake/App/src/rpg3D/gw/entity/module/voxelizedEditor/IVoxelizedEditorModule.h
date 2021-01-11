#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include "IVoxelizedEditorM_Edit.h"
#include "IVoxelizedEditorM_View.h"

namespace rpg3D {
	class IEditableVoxelisedModule;
	class IEntityEditorView_Edit;
	class IHandlerVoxelizedEditorModule;
};

namespace rpg3D {
namespace entityEditorModule {
class IVoxelizedEditorModule : public virtual IEntityModule, virtual public IVoxelizedEditorM_Edit, virtual public IVoxelizedEditorM_View {priv typedef IComponent super;
	pub static int TYPE;
	DEC_componentBasics();

	pub virtual void startEdit(
		IHandlerVoxelizedEditorModule* handler,
		rpg3D::IEditableVoxelisedM_Edit* entityToEdit_editableEntity, IWorldEntity* optionalExistingEntityToEditB,
		IEntityEditorView_Edit* entityEditorView,
		bool isEditorStructureEnabled,
		std::shared_ptr<graphics::ICinematicController> cinematicController) = 0;
    pub virtual void endEditIfAny() = 0;

    pub ~IVoxelizedEditorModule() override = default;
};
};
};
