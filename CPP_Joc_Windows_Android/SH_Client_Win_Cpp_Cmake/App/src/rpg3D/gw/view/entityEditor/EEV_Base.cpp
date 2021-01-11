#include "EEV_Base.h"
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>

using namespace rpg3D;

EEV_Base::EEV_Base(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(app, viewItemConfigs, viewEaseDurationsSList, nullptr/*handlerAutoDisposal*/),
	appUtils(appUtils)
{
	//void
}

void EEV_Base::setHandlerAndListener(IVoxelizedEditorM_Edit* entityEditor, IListenerEntityEditorView* listener) {
	this->entityEditor = entityEditor;
	this->listener = listener;
}

void EEV_Base::setEntityToEdit(IEditableVoxelisedM_Edit* entityToEdit) {
	onEntityToEditChangePre();

	this->entityToEdit = entityToEdit;
	if (this->entityToEdit != nullptr) {
		entityToEdit_stackable = entityToEdit->getEntity()->getComponentAs< rpg3D::IStackableModule*>(true/*mustExist*/);
	} else {
		entityToEdit_stackable = nullptr;
	}

	onEntityToEditChanged();

	// Call this here so it is called after the colors bar is populated by "onEntityToEditChanged()".
	if (entityToEdit != nullptr) {
		entityToEdit->setEntityEditorView(this);
	}
}

void EEV_Base::onEntityToEditChangePre() {
	if (entityToEdit != nullptr) {
		entityToEdit->setEntityEditorView(nullptr);
	}
}

void EEV_Base::onEntityToEditChanged() {
	//void
}

base::IBlockingInputLayer* EEV_Base::getBlockingStreamLayer() {
	return blockingStreamLayer;
}

void EEV_Base::setBlockingStreamLayer(base::IBlockingInputLayer* blockingStreamLayer) {
	this->blockingStreamLayer = blockingStreamLayer;
}

void EEV_Base::onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) {
	//void
}

void EEV_Base::onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) {
	//void
}

void EEV_Base::onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) {
	//void
}

void EEV_Base::onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) {
	//void
}

std::string EEV_Base::computeStateFlags(std::string baseState) {
	std::string state = super::computeStateFlags(baseState);

	if (editorType == EditorType::EDIT_OTHER) {
		state = state + "_EditorType:edit_";
	} else if (editorType == EditorType::PAINT_OTHER) {
		state = state + "_EditorType:paint_";
	} else {
		throw LogicException(LOC);
	}

	return state;
}

EEV_Base::~EEV_Base() {
	//void
}
