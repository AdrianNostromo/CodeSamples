#pragma once

#include <base/gh.h>
#include "IVoxelizedEditorModule.h"
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <base/input/util/Touch.h>

namespace rpg3D {
	class VoxelizedEditorTemplate;
	class CbListenerEntityEditorView;
	class IHandlerVoxelizedEditorModule;
};

namespace rpg3D {
namespace entityEditorModule {
class VEM_Base : public EntityModule, virtual public IVoxelizedEditorModule {priv typedef EntityModule super;pub dCtor(VEM_Base);
	pub VoxelizedEditorTemplate* t;

	priv CbListenerEntityEditorView* listener_entityEditorView = nullptr;

	prot IHandlerVoxelizedEditorModule* handler = nullptr;

	prot rpg3D::IEditableVoxelisedM_Edit* entityToEdit_editableEntity = nullptr;
	prot IWorldEntity* optionalExistingEntityToEditB = nullptr;
	prot IEntityEditorView_Edit* entityEditorView = nullptr;
	prot bool isEditorStructureEnabled = false;
	priv std::shared_ptr<graphics::ICinematicController> cinematicController = nullptr;

    pub explicit VEM_Base(
		IWorldEntity* entity,
		VoxelizedEditorTemplate* t);

	pub IWorldEntity* getEntity() final;

	priv void entityEditorView_onBtn(IEntityEditorView_Edit* target, int uiButtonId);

	pub void startEdit(
		IHandlerVoxelizedEditorModule* handler,
		rpg3D::IEditableVoxelisedM_Edit* entityToEdit_editableEntity, IWorldEntity* optionalExistingEntityToEditB,
		IEntityEditorView_Edit* entityEditorView,
		bool isEditorStructureEnabled,
		std::shared_ptr<graphics::ICinematicController> cinematicController) final;
	prot virtual void onStartEditPost();
	pub void endEditIfAny() final;
	prot virtual void onEndEditPre();
	prot virtual void onEndEditPost();

	pub rpg3D::IEditableVoxelisedM_Edit* getEntityToEdit_editableEntity() final;
	pub std::shared_ptr<graphics::ICinematicController> getCinematicController() final;
	pub void editableVoxelized_onEditOccured(rpg3D::IEditableVoxelisedM_Edit* editableVoxelized) final;

    pub ~VEM_Base() override;
};
};
};
