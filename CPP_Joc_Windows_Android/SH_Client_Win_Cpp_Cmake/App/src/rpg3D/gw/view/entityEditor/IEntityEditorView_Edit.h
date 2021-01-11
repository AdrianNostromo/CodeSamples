#pragma once

#include <base/gh.h>
#include <functional>
#include <base/math/Vector3Int.h>

namespace rpg3D {
	class IHandlerEntityEditorView;
	class IListenerEntityEditorView;
	class IVoxelizedEditorM_Edit;
	class IEditableVoxelisedM_Edit;
};

namespace rpg3D {
class IEntityEditorView_Edit {
	pub virtual void setHandlerAndListener(IVoxelizedEditorM_Edit* entityEditor, IListenerEntityEditorView* listener) = 0;
	pub virtual void setEntityToEdit(IEditableVoxelisedM_Edit* entityToEdit) = 0;

	pub virtual void setIsColorsSelectorActive(bool isColorsSelectorActive) = 0;

	pub virtual void onActiveDrawDynamicMaterialIndexChanged() = 0;
	pub virtual void onNewEditorColor(int editorColorIndex, Color& col, bool updatesHistory) = 0;
	pub virtual void onEditorColorChanged(int editorColorIndex, Color& oldCol, Color& newCol, bool updatesHistory) = 0;
	pub virtual void onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory) = 0;

	pub virtual void onHistoryUndoAvailableChanged(bool isHistryRedoAvailable) = 0;
	pub virtual void onHistoryRedoAvailableChanged(bool isHistryRedoAvailable) = 0;

	pub virtual void onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) = 0;
	pub virtual void onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) = 0;
	pub virtual void onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) = 0;
	pub virtual void onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) = 0;

	pub virtual ~IEntityEditorView_Edit() = default;
};
};
