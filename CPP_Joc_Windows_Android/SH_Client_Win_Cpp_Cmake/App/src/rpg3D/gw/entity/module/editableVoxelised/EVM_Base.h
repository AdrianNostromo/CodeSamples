#pragma once

#include <base/gh.h>
#include "IEditableVoxelisedModule.h"
#include <worldGame3D/gw/entity/module/EntityModule.h>

namespace rpg3D {
	class IStackableModule;
};

namespace rpg3D {
namespace editableEntityModule {
class EVM_Base : public EntityModule, virtual public IEditableVoxelisedModule {priv typedef EntityModule super;pub dCtor(EVM_Base);
	pub EditableVoxelisedTemplate* t;

	// This is set on first edit start.
	prot rpg3D::IStackableModule* stackable = nullptr;

	prot bool isEditingEnabled = false;
	prot bool isEditorStructureEnabled = false;
	prot IWorldEntity* editableEntityB = nullptr;

	prot IEntityEditorView_Edit* entityEditorView = nullptr;
	prot IVoxelizedEditorM_Edit* editorEntity_editor = nullptr;

    pub explicit EVM_Base(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);

	pub IWorldEntity* getEntity() final;
	pub EditableVoxelisedTemplate* getTemplate() final;

	pub void enableEditing(bool isEditingActive, bool isEditorStructureEnabled, IWorldEntity* editableEntityB) final;
	pub void disableEditing() final;
	prot virtual void onIsEditingEnabledChanged();

	pub void setEditor(IVoxelizedEditorM_Edit* editorEntity_editor) final;

	pub void setEntityEditorView(IEntityEditorView_Edit* entityEditorView) final;

	pub bool getIsEditorStructureEnabled() final;

    pub ~EVM_Base() override;
};
};
};
