#include "T_ExtraActivationListControls.h"
#include <base/exceptions/LogicException.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/event/ToolEvent.h>

using namespace rpg3D;

T_ExtraActivationListControls::T_ExtraActivationListControls(ToolConfigBase* config)
	: super(config)
{
	//void
}

TreeAVL<T_ExtraActivationListControls::ExtraActivationListControl>* T_ExtraActivationListControls::getExtraActivationListControlsList() {
	return extraActivationListControlsList;
}

T_ExtraActivationListControls::ExtraActivationListControl* T_ExtraActivationListControls::addExtraActivationListControl(
	std::string atlasId, std::string regionId,
	bool isSelected)
{
	if (extraActivationListControlsList == nullptr) {
		extraActivationListControlsList = new TreeAVL<ExtraActivationListControl>();
	}

	TreeAVL<ExtraActivationListControl>::Node* extraActivationControlNode = extraActivationListControlsList->insertEmplace(entryUidCounter++/*uid*/, atlasId, regionId);

	if (isSelected) {
		setExtraActivationListControlIsSelected(extraActivationControlNode, isSelected);
	}

	return &extraActivationControlNode->data;
}

void T_ExtraActivationListControls::setExtraActivationListControlIsSelected(TreeAVL<ExtraActivationListControl>::Node* extraActivationControlNode, bool isSelected) {
	if (extraActivationControlNode == nullptr) {
		throw LogicException(LOC);
	}

	if (extraActivationControlNode->data.isSelected == isSelected) {
		return;
	}

	int entryUid_deselected = -1;
	if (selectedExtraActivationListControl != nullptr) {
		entryUid_deselected = selectedExtraActivationListControl->uid;

		selectedExtraActivationListControl->isSelected = false;

		selectedExtraActivationListControl = nullptr;
	}
	
	int entryUid_selected = -1;
	if (isSelected) {
		selectedExtraActivationListControl = &extraActivationControlNode->data;

		selectedExtraActivationListControl->isSelected = true;

		entryUid_selected = selectedExtraActivationListControl->uid;
	}

	if (entryUid_deselected >= 0 || entryUid_selected >= 0) {
		// Dispatch a change event with the 2 uid values.
		events.dispatchEvent(*ToolEvent_extraActivationListControls_onSelectedChanged->setExtraControlChangeDelta(
			this,
			entryUid_deselected, entryUid_selected
		));
	}
}

T_ExtraActivationListControls::~T_ExtraActivationListControls() {
	if (extraActivationListControlsList != nullptr) {
		delete extraActivationListControlsList;
		extraActivationListControlsList = nullptr;
	}
}
