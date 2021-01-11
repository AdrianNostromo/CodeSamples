#include "EVM_History.h"
#include "history/HistoryActionVoxelSet.h"
#include "history/HistoryActionMaterialChange.h"
#include "history/HistoryActionGridOffsetManualTranslate.h"
#include "history/HistoryAction_voxelSize.h"
#include "history/HistoryAction_stackable_haulOffsetZM.h"
#include "history/HistoryActionMaterialCreate.h"
#include "history/HistoryActionMaterialRemove.h"
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorM_Edit.h>
#include <rpg3D/gw/view/entityEditor/IEntityEditorView_Edit.h>

using namespace rpg3D;

const int EVM_History::MAX_HISTORY_STEPS = 2000;

EVM_History::EVM_History(
	IWorldEntity* entity,
	EditableVoxelisedTemplate* t)
	: super(
		entity,
		t)
{
	//void
}

void EVM_History::pushHistory_VoxelSet(Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin, int oldEditorMatIndex, int newEditorMatIndex) {
	clearRedoHistory();

	HistoryActionVoxelSet* entry = new HistoryActionVoxelSet(gPos, postChangeGridBoundsMin, oldEditorMatIndex, newEditorMatIndex);

	activeHistoryStep = actionsHistoryStack.insertAfterDirect(activeHistoryStep, entry);

	clipHistoryToBounds();

	onHistoryStepsOrActiveChanged();
}

void EVM_History::pushHistory_MaterialCreate(int editorMatIndex, Color& diffuseColor) {
	clearRedoHistory();

	HistoryActionMaterialCreate* entry = new HistoryActionMaterialCreate(editorMatIndex, diffuseColor);

	activeHistoryStep = actionsHistoryStack.insertAfterDirect(activeHistoryStep, entry);

	clipHistoryToBounds();

	onHistoryStepsOrActiveChanged();
}

void EVM_History::pushHistory_MaterialChange(int editorMatIndex, Color& oldMatColor, Color& newMatColor) {
	clearRedoHistory();

	HistoryActionMaterialChange* entry = new HistoryActionMaterialChange(editorMatIndex, oldMatColor, newMatColor);

	activeHistoryStep = actionsHistoryStack.insertAfterDirect(activeHistoryStep, entry);

	clipHistoryToBounds();

	onHistoryStepsOrActiveChanged();
}

void EVM_History::pushHistory_MaterialRemove(int editorMatIndex, Color& col) {
	clearRedoHistory();

	HistoryActionMaterialRemove* entry = new HistoryActionMaterialRemove(editorMatIndex, col);

	activeHistoryStep = actionsHistoryStack.insertAfterDirect(activeHistoryStep, entry);

	clipHistoryToBounds();

	onHistoryStepsOrActiveChanged();
}

void EVM_History::pushHistory_GridOffsetManualTranslate(Vector3& translate) {
	clearRedoHistory();

	HistoryActionGridOffsetManualTranslate* entry = new HistoryActionGridOffsetManualTranslate(translate);

	activeHistoryStep = actionsHistoryStack.insertAfterDirect(activeHistoryStep, entry);

	clipHistoryToBounds();

	onHistoryStepsOrActiveChanged();
}

void EVM_History::pushHistory_voxelSize(float voxelSize, float old_voxelSize) {
	clearRedoHistory();

	HistoryAction_voxelSize* entry = new HistoryAction_voxelSize(voxelSize, old_voxelSize);

	activeHistoryStep = actionsHistoryStack.insertAfterDirect(activeHistoryStep, entry);

	clipHistoryToBounds();

	onHistoryStepsOrActiveChanged();

}

void EVM_History::pushHistory_stackable_haulOffsetZM(float haulOffsetZM, float old_haulOffsetZM) {
	clearRedoHistory();

	HistoryAction_stackable_haulOffsetZM* entry = new HistoryAction_stackable_haulOffsetZM(haulOffsetZM, old_haulOffsetZM);

	activeHistoryStep = actionsHistoryStack.insertAfterDirect(activeHistoryStep, entry);

	clipHistoryToBounds();

	onHistoryStepsOrActiveChanged();
}

void EVM_History::historyUndo() {
	if (activeHistoryStep == nullptr) {
		return;
	}

	// Undo the history step from activeHistoryStep.
	HistoryAction* historyAction = activeHistoryStep->data;
	if (historyAction->type == HistoryActionVoxelSet::TYPE) {
		HistoryActionVoxelSet* cHistoryAction = historyAction->castAs<HistoryActionVoxelSet*>(HistoryActionVoxelSet::TYPE);

		setVoxelMaterialIndex(
			cHistoryAction->gPos.x - cHistoryAction->postChangeGridBoundsMin.x,
			cHistoryAction->gPos.y - cHistoryAction->postChangeGridBoundsMin.y,
			cHistoryAction->gPos.z - cHistoryAction->postChangeGridBoundsMin.z,
			cHistoryAction->oldEditorMatIndex, false/*updatesHistory*/
		);
	} else if (historyAction->type == HistoryActionMaterialCreate::TYPE) {
		HistoryActionMaterialCreate* cHistoryAction = historyAction->castAs<HistoryActionMaterialCreate*>(HistoryActionMaterialCreate::TYPE);

		removeUnusedVoxelMaterial(cHistoryAction->editorMatIndex, false);
	} else if (historyAction->type == HistoryActionMaterialChange::TYPE) {
		HistoryActionMaterialChange* cHistoryAction = historyAction->castAs<HistoryActionMaterialChange*>(HistoryActionMaterialChange::TYPE);

		editVoxelMaterialDiffuseColor(cHistoryAction->editorMatIndex, cHistoryAction->oldMatColor, false);
	} else if (historyAction->type == HistoryActionMaterialRemove::TYPE) {
		HistoryActionMaterialRemove* cHistoryAction = historyAction->castAs<HistoryActionMaterialRemove*>(HistoryActionMaterialRemove::TYPE);

		createNewMaterialFromDiffuseColor(cHistoryAction->editorMatIndex, cHistoryAction->col, false);
	} else if (historyAction->type == HistoryActionGridOffsetManualTranslate::TYPE) {
		HistoryActionGridOffsetManualTranslate* cHistoryAction = historyAction->castAs<HistoryActionGridOffsetManualTranslate*>(HistoryActionGridOffsetManualTranslate::TYPE);

		translateVoxelsGrid(
			-cHistoryAction->translate.x,
			-cHistoryAction->translate.y,
			-cHistoryAction->translate.z,
			false
		);
	} else if (historyAction->type == HistoryAction_voxelSize::TYPE) {
		HistoryAction_voxelSize* cHistoryAction = historyAction->castAs<HistoryAction_voxelSize*>(HistoryAction_voxelSize::TYPE);

		setEditEntity_voxelSize(cHistoryAction->old_voxelSize, false);
	} else if (historyAction->type == HistoryAction_stackable_haulOffsetZM::TYPE) {
		HistoryAction_stackable_haulOffsetZM* cHistoryAction = historyAction->castAs<HistoryAction_stackable_haulOffsetZM*>(HistoryAction_stackable_haulOffsetZM::TYPE);

		setEditEntity_stackable_haulOffsetZM(cHistoryAction->old_haulOffsetZM, false);
	} else {
		throw LogicException(LOC);
	}

	activeHistoryStep = activeHistoryStep->prev;

	onHistoryStepsOrActiveChanged();
}

void EVM_History::historyRedo() {
	if (actionsHistoryStack.count() == 0) {
		return;
	} else if (activeHistoryStep == nullptr) {
		activeHistoryStep = actionsHistoryStack.getFirst();
	} else {
		if (activeHistoryStep->next == nullptr) {
			return;
		}

		activeHistoryStep = activeHistoryStep->next;
	}

	// Do the next history step from activeHistoryStep.
	HistoryAction* historyAction = activeHistoryStep->data;
	if (historyAction->type == HistoryActionVoxelSet::TYPE) {
		HistoryActionVoxelSet* cHistoryAction = historyAction->castAs<HistoryActionVoxelSet*>(HistoryActionVoxelSet::TYPE);

		setVoxelMaterialIndex(
			cHistoryAction->gPos.x, 
			cHistoryAction->gPos.y, 
			cHistoryAction->gPos.z, 
			cHistoryAction->newEditorMatIndex, false/*updatesHistory*/
		);
	} else if (historyAction->type == HistoryActionMaterialCreate::TYPE) {
		HistoryActionMaterialCreate* cHistoryAction = historyAction->castAs<HistoryActionMaterialCreate*>(HistoryActionMaterialCreate::TYPE);

		createNewMaterialFromDiffuseColor(cHistoryAction->editorMatIndex, cHistoryAction->diffuseColor, false);
	} else if (historyAction->type == HistoryActionMaterialChange::TYPE) {
		HistoryActionMaterialChange* cHistoryAction = historyAction->castAs<HistoryActionMaterialChange*>(HistoryActionMaterialChange::TYPE);

		editVoxelMaterialDiffuseColor(cHistoryAction->editorMatIndex, cHistoryAction->newMatColor, false);
	} else if (historyAction->type == HistoryActionMaterialRemove::TYPE) {
		HistoryActionMaterialRemove* cHistoryAction = historyAction->castAs<HistoryActionMaterialRemove*>(HistoryActionMaterialRemove::TYPE);

		removeUnusedVoxelMaterial(cHistoryAction->editorMatIndex, false);
	} else if (historyAction->type == HistoryActionGridOffsetManualTranslate::TYPE) {
		HistoryActionGridOffsetManualTranslate* cHistoryAction = historyAction->castAs<HistoryActionGridOffsetManualTranslate*>(HistoryActionGridOffsetManualTranslate::TYPE);

		translateVoxelsGrid(
			cHistoryAction->translate.x,
			cHistoryAction->translate.y,
			cHistoryAction->translate.z,
			false
		);
	} else if (historyAction->type == HistoryAction_voxelSize::TYPE) {
		HistoryAction_voxelSize* cHistoryAction = historyAction->castAs<HistoryAction_voxelSize*>(HistoryAction_voxelSize::TYPE);

		setEditEntity_voxelSize(cHistoryAction->new_voxelSize, false);
	} else if (historyAction->type == HistoryAction_stackable_haulOffsetZM::TYPE) {
		HistoryAction_stackable_haulOffsetZM* cHistoryAction = historyAction->castAs<HistoryAction_stackable_haulOffsetZM*>(HistoryAction_stackable_haulOffsetZM::TYPE);

		setEditEntity_stackable_haulOffsetZM(cHistoryAction->new_haulOffsetZM, false);
	} else {
		throw LogicException(LOC);
	}

	onHistoryStepsOrActiveChanged();
}

bool EVM_History::getIsHistryUndoAvailable() {
	return isHistryUndoAvailable;
}

bool EVM_History::getIsHistryRedoAvailable() {
	return isHistryRedoAvailable;
}

void EVM_History::onHistoryStepsOrActiveChanged() {
	bool newIsHistryUndoAvailable;
	if (activeHistoryStep == nullptr) {
		newIsHistryUndoAvailable = false;
	} else {
		newIsHistryUndoAvailable = true;
	}
	if (newIsHistryUndoAvailable != isHistryUndoAvailable) {
		isHistryUndoAvailable = newIsHistryUndoAvailable;

		onHistoryUndoAvailableChanged(isHistryUndoAvailable);
	}

	bool newIsHistryRedoAvailable;
	if (actionsHistoryStack.count() == 0 || (activeHistoryStep != nullptr && activeHistoryStep->next == nullptr)) {
		newIsHistryRedoAvailable = false;
	} else {
		newIsHistryRedoAvailable = true;
	}
	if (newIsHistryRedoAvailable != isHistryRedoAvailable) {
		isHistryRedoAvailable = newIsHistryRedoAvailable;

		onHistoryRedoAvailableChanged(isHistryRedoAvailable);
	}
}

void EVM_History::onHistoryUndoAvailableChanged(bool isHistryRedoAvailable) {
	if (entityEditorView != nullptr) {
		entityEditorView->onHistoryUndoAvailableChanged(isHistryRedoAvailable);
	}
}

void EVM_History::onHistoryRedoAvailableChanged(bool isHistryRedoAvailable) {
	if (entityEditorView != nullptr) {
		entityEditorView->onHistoryRedoAvailableChanged(isHistryRedoAvailable);
	}
}

void EVM_History::clearRedoHistory() {
	if (activeHistoryStep == nullptr) {
		ListDL<HistoryAction*>::Entry* entry;
		while ((entry = actionsHistoryStack.getFirst()) != nullptr) {
			delete entry->data;
			entry->data = nullptr;

			entry->remove();
		}
	} else {
		ListDL<HistoryAction*>::Entry* nextEntry;
		while ((nextEntry = activeHistoryStep->next) != nullptr) {
			delete nextEntry->data;
			nextEntry->data = nullptr;

			nextEntry->remove();
		}
	}
}

void EVM_History::onVoxelMaterialIndexChanged(
	Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin,
	int oldEditorMatIndex, int originalRegisteredVoxelMatIndex,
	int newEditorMatIndex, int newRegisteredVertexColorIndex,
	bool updatesHistory)
{
	super::onVoxelMaterialIndexChanged(
		gPos, postChangeGridBoundsMin,
		oldEditorMatIndex, originalRegisteredVoxelMatIndex,
		newEditorMatIndex, newRegisteredVertexColorIndex,
		updatesHistory
	);

	if (updatesHistory) {
		pushHistory_VoxelSet(gPos, postChangeGridBoundsMin, oldEditorMatIndex, newEditorMatIndex);
	}

	if (editorEntity_editor == nullptr) {
		throw LogicException(LOC);
	}
	editorEntity_editor->editableVoxelized_onEditOccured(this);
}

void EVM_History::onNewEditorColor(int editorColorIndex, Color& col, bool updatesHistory) {
	super::onNewEditorColor(editorColorIndex, col, updatesHistory);

	if (updatesHistory) {
		pushHistory_MaterialCreate(editorColorIndex, col);
	}
}

void EVM_History::onEditorColorChanged(int editorColorIndex, Color& oldCol, Color& newCol, bool updatesHistory) {
	super::onEditorColorChanged(editorColorIndex, oldCol, newCol, updatesHistory);

	if (updatesHistory) {
		pushHistory_MaterialChange(editorColorIndex, oldCol, newCol);
	}
}

void EVM_History::onEditorRegisteredMaterialChanged() {
	super::onEditorRegisteredMaterialChanged();

	if (editorEntity_editor == nullptr) {
		throw LogicException(LOC);
	}
	editorEntity_editor->editableVoxelized_onEditOccured(this);
}

void EVM_History::onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory) {
	super::onEditorColorRemoved(editorColorIndex, col, updatesHistory);

	if (updatesHistory) {
		pushHistory_MaterialRemove(editorColorIndex, col);
	}
}

void EVM_History::onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) {
	super::onTranslateVoxelsGrid(x, y, z, updatesHistory);

	if (updatesHistory) {
		Vector3 translateDelta{ x, y, z };
		pushHistory_GridOffsetManualTranslate(translateDelta);
	}

	if (editorEntity_editor == nullptr) {
		throw LogicException(LOC);
	}
	editorEntity_editor->editableVoxelized_onEditOccured(this);
}

void EVM_History::onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) {
	super::onEditEntity_voxelSize_changed(voxelSize, old_voxelSize, updatesHistory);

	if (updatesHistory) {
		pushHistory_voxelSize(voxelSize, old_voxelSize);
	}

	if (editorEntity_editor == nullptr) {
		throw LogicException(LOC);
	}
	editorEntity_editor->editableVoxelized_onEditOccured(this);
}

void EVM_History::onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) {
	super::onEditEntity_stackable_haulOffsetZM_changed(haulOffsetZM, old_haulOffsetZM, updatesHistory);

	if (updatesHistory) {
		pushHistory_stackable_haulOffsetZM(haulOffsetZM, old_haulOffsetZM);
	}

	if (editorEntity_editor == nullptr) {
		throw LogicException(LOC);
	}
	editorEntity_editor->editableVoxelized_onEditOccured(this);
}

void EVM_History::clipHistoryToBounds() {
	if (MAX_HISTORY_STEPS < 0) {
		return;
	}

	while (actionsHistoryStack.count() > MAX_HISTORY_STEPS) {
		// Remove the first element.
		ListDL<HistoryAction*>::Entry* entry = actionsHistoryStack.getFirst();

		if (activeHistoryStep == entry) {
			if (MAX_HISTORY_STEPS == 0 && actionsHistoryStack.count() == 1) {
				// This is a special case when the history system is disabled.
				activeHistoryStep = nullptr;
			} else {
				throw LogicException(LOC);
			}
		}

		entry->remove();
	}
}

void EVM_History::disposeMain() {
	activeHistoryStep = nullptr;

	ListDL<HistoryAction*>::Entry* entry;
	while ((entry = actionsHistoryStack.getFirst()) != nullptr) {
		delete entry->data;
		entry->data = nullptr;

		entry->remove();
	}

	super::disposeMain();
}

EVM_History::~EVM_History() {
	//void
}
