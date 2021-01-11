#include "EVM_Base.h"
#include <rpg3D/gw/entity/template/editableVoxelised/EditableVoxelisedTemplate.h>
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>

using namespace rpg3D;
using namespace rpg3D::editableEntityModule;

EVM_Base::EVM_Base(
	IWorldEntity* entity,
	EditableVoxelisedTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	//void
}

IWorldEntity* EVM_Base::getEntity() {
	return e;
}

EditableVoxelisedTemplate* EVM_Base::getTemplate() {
	return t;
}

void EVM_Base::enableEditing(bool isEditingActive, bool isEditorStructureEnabled, IWorldEntity* editableEntityB) {
	if (this->isEditingEnabled) {
		throw LogicException(LOC);
	}

	isEditingEnabled = true;

	if (stackable == nullptr) {
		stackable = e->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
	}

	this->isEditorStructureEnabled = isEditorStructureEnabled;
	this->editableEntityB = editableEntityB;

	onIsEditingEnabledChanged();
}

void EVM_Base::disableEditing() {
	if (!this->isEditingEnabled) {
		throw LogicException(LOC);
	}

	isEditingEnabled = false;
	editableEntityB = nullptr;
	
	onIsEditingEnabledChanged();
}

void EVM_Base::onIsEditingEnabledChanged() {
	//void
}

void EVM_Base::setEditor(IVoxelizedEditorM_Edit* editorEntity_editor) {
	this->editorEntity_editor = editorEntity_editor;
}

void EVM_Base::setEntityEditorView(IEntityEditorView_Edit* entityEditorView) {
	this->entityEditorView = entityEditorView;
}

bool EVM_Base::getIsEditorStructureEnabled() {
	if (!isEditingEnabled) {
		throw LogicException(LOC);
	}

	return isEditorStructureEnabled;
}

EVM_Base::~EVM_Base() {
	//void
}
