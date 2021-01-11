#include "VEM_Base.h"
#include <rpg3D/gw/entity/template/voxelizedEditor/VoxelizedEditorTemplate.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <rpg3D/gw/view/entityEditor/IEntityEditorView_Edit.h>
#include <rpg3D/gw/view/entityEditor/util/CbListenerEntityEditorView.h>
#include <rpg3D/gw/entity/module/voxelizedEditor/util/IHandlerVoxelizedEditorModule.h>

using namespace rpg3D;
using namespace rpg3D::entityEditorModule;

VEM_Base::VEM_Base(
	IWorldEntity* entity,
	VoxelizedEditorTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	listener_entityEditorView = new CbListenerEntityEditorView();
	listener_entityEditorView->cb_onBtn = std::bind(&VEM_Base::entityEditorView_onBtn, this, std::placeholders::_1, std::placeholders::_2);
}

IWorldEntity* VEM_Base::getEntity() {
	return e;
}

void VEM_Base::entityEditorView_onBtn(IEntityEditorView_Edit* target, int uiButtonId) {
	if (uiButtonId == IListenerEntityEditorView::ButtonIds::CLOSE) {
		if (handler != nullptr) {
			handler->entityEditor_requestStopEditor();
		}
	}

	else {
		throw LogicException(LOC);
	}
}

void VEM_Base::startEdit(
	IHandlerVoxelizedEditorModule* handler,
	rpg3D::IEditableVoxelisedM_Edit* entityToEdit_editableEntity, IWorldEntity* optionalExistingEntityToEditB,
	IEntityEditorView_Edit* entityEditorView,
	bool isEditorStructureEnabled,
	std::shared_ptr<graphics::ICinematicController> cinematicController)
{
	if (this->handler != nullptr) {
		// Edit mode already active.
		throw LogicException(LOC);
	}
	
	if (entityToEdit_editableEntity == nullptr || entityEditorView == nullptr || handler == nullptr) {
		// Parameters error.
		throw LogicException(LOC);
	}

	this->handler = handler;
	this->entityToEdit_editableEntity = entityToEdit_editableEntity;
	this->optionalExistingEntityToEditB = optionalExistingEntityToEditB;
	this->entityEditorView = entityEditorView;
	this->isEditorStructureEnabled = isEditorStructureEnabled;
	this->cinematicController = cinematicController;

	this->entityToEdit_editableEntity->enableEditing(true, this->isEditorStructureEnabled, this->optionalExistingEntityToEditB);
	this->entityToEdit_editableEntity->setEditor(this);

	this->entityEditorView->setHandlerAndListener(this, listener_entityEditorView);
	this->entityEditorView->setEntityToEdit(entityToEdit_editableEntity);

	onStartEditPost();
}

void VEM_Base::onStartEditPost() {
	//void;
}

void VEM_Base::endEditIfAny() {
	if (entityToEdit_editableEntity == nullptr) {
		return;
	}

	onEndEditPre();

	this->entityEditorView->setEntityToEdit(nullptr);

	this->entityToEdit_editableEntity->setEditor(this);
	entityToEdit_editableEntity->disableEditing();

	handler = nullptr;
	entityToEdit_editableEntity = nullptr;
	optionalExistingEntityToEditB = nullptr;
	entityEditorView = nullptr;

	onEndEditPost();
}

void VEM_Base::onEndEditPre() {
	//void;
}

void VEM_Base::onEndEditPost() {
	//void;
}

rpg3D::IEditableVoxelisedM_Edit* VEM_Base::getEntityToEdit_editableEntity() {
	if (entityToEdit_editableEntity == nullptr) {
		// Not editing anything.
		throw LogicException(LOC);
	}

	return entityToEdit_editableEntity;
}

std::shared_ptr<graphics::ICinematicController> VEM_Base::getCinematicController() {
	if (entityToEdit_editableEntity == nullptr) {
		// Not editing anything.
		throw LogicException(LOC);
	}

	return cinematicController;
}

void VEM_Base::editableVoxelized_onEditOccured(rpg3D::IEditableVoxelisedM_Edit* editableVoxelized) {
	if (handler == nullptr) {
		throw LogicException(LOC);
	}

	handler->entityEditor_processOnEditOccured(editableVoxelized->getEntity());
}

VEM_Base::~VEM_Base() {
	if (listener_entityEditorView != nullptr) {
		delete listener_entityEditorView;
		listener_entityEditorView = nullptr;
	}
}
