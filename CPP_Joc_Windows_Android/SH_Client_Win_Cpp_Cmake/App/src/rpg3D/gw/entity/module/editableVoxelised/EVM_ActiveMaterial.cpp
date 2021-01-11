#include "EVM_ActiveMaterial.h"
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <rpg3D/gw/view/entityEditor/IEntityEditorView_Edit.h>

using namespace rpg3D;

EVM_ActiveMaterial::EVM_ActiveMaterial(
	IWorldEntity* entity,
	EditableVoxelisedTemplate* t)
	: super(
		entity,
		t)
{
	//void
}

int EVM_ActiveMaterial::peekActiveDynamicMaterialIndex() {
	return activeDynamicMaterialIndex;
}

void EVM_ActiveMaterial::setActiveDynamicMaterialIndex(int activeDynamicMaterialIndex) {
	if (this->activeDynamicMaterialIndex == activeDynamicMaterialIndex) {
		return;
	}

	this->activeDynamicMaterialIndex = activeDynamicMaterialIndex;

	onActiveDrawDynamicMaterialIndexChanged();
}

void EVM_ActiveMaterial::onActiveDrawDynamicMaterialIndexChanged() {
	if (entityEditorView != nullptr) {
		entityEditorView->onActiveDrawDynamicMaterialIndexChanged();
	}
}

void EVM_ActiveMaterial::onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory) {
	super::onEditorColorRemoved(editorColorIndex, col, updatesHistory);

	if (editorColorIndex <= activeDynamicMaterialIndex) {
		if (activeDynamicMaterialIndex >= peekEditingIndexedVoxelColors()->count()) {
			setActiveDynamicMaterialIndex(peekEditingIndexedVoxelColors()->count() - 1);
		} else if (activeDynamicMaterialIndex > editorColorIndex) {
			setActiveDynamicMaterialIndex(activeDynamicMaterialIndex - 1);
		}
	}
}

void EVM_ActiveMaterial::onIsEditingEnabledChanged() {
	super::onIsEditingEnabledChanged();

	if (isEditingEnabled) {
		if (peekEditingIndexedVoxelColors()->count() >= 1) {
			setActiveDynamicMaterialIndex(0);
		} else {
			setActiveDynamicMaterialIndex(-1);
		}
	}
}

EVM_ActiveMaterial::~EVM_ActiveMaterial() {
	//void
}
