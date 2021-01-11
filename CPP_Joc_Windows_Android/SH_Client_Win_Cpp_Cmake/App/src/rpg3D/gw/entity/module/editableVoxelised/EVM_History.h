#pragma once

#include <base/gh.h>
#include "EVM_EntityStackableCarryPosIndicator.h"
#include <base/list/ListDL.h>
#include <base/MM.h>

namespace rpg3D {
	class HistoryAction;
};

namespace rpg3D {
class EVM_History : public EVM_EntityStackableCarryPosIndicator {priv typedef EVM_EntityStackableCarryPosIndicator super;pub dCtor(EVM_History);
	// ">0"; Fixed history entries count.
	// "-1" Unimited history entries count.
	// "0" History disabled;
	priv static const int MAX_HISTORY_STEPS;

	priv ListDL<HistoryAction*> actionsHistoryStack{LOC};
	priv ListDL<HistoryAction*>::Entry* activeHistoryStep = nullptr;

	priv bool isHistryUndoAvailable = false;
	priv bool isHistryRedoAvailable = false;
    
	pub explicit EVM_History(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);

	//asdA18;// call these;
	priv void pushHistory_VoxelSet(Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin, int oldEditorMatIndex, int newEditorMatIndex);
	priv void pushHistory_MaterialCreate(int editorMatIndex, Color& diffuseColor);
	priv void pushHistory_MaterialChange(int editorMatIndex, Color& oldMatColor, Color& newMatColor);
	priv void pushHistory_MaterialRemove(int editorMatIndex, Color& col);
	priv void pushHistory_GridOffsetManualTranslate(Vector3& translate);
	priv void pushHistory_voxelSize(float voxelSize, float old_voxelSize);
	priv void pushHistory_stackable_haulOffsetZM(float haulOffsetZM, float old_haulOffsetZM);

	pub void historyUndo() final;
	pub void historyRedo() final;

	pub bool getIsHistryUndoAvailable() final;
	pub bool getIsHistryRedoAvailable() final;

	priv void onHistoryStepsOrActiveChanged();
	prot virtual void onHistoryUndoAvailableChanged(bool isHistryRedoAvailable);
	prot virtual void onHistoryRedoAvailableChanged(bool isHistryRedoAvailable);
	
	priv void clearRedoHistory();

	prot void onVoxelMaterialIndexChanged(
		Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin,
		int oldEditorMatIndex, int originalRegisteredVoxelMatIndex,
		int newEditorMatIndex, int newRegisteredVertexColorIndex,
		bool updatesHistory) override;

	prot void onNewEditorColor(int editorColorIndex, Color& col, bool updatesHistory) override;
	prot void onEditorColorChanged(int editorColorIndex, Color& oldCol, Color& newCol, bool updatesHistory) override;
	prot void onEditorRegisteredMaterialChanged() override;
	prot void onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory) override;

	prot void onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) override;
	prot void onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) override;
	prot void onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) override;

	priv void clipHistoryToBounds();

	prot void disposeMain() override;
    pub ~EVM_History() override;
};
};
