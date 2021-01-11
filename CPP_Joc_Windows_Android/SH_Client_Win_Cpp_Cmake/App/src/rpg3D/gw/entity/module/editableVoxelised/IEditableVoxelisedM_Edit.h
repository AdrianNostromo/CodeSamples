#pragma once

#include <base/gh.h>
#include <base/math/collision/Ray.h>
#include <base/math/Vector3Int.h>
#include <graphics/visual/Color.h>
#include <memory>
#include <base/list/ArrayList.h>

class IContainer3D;
class VoxelGridDrawable3D;
namespace worldGame3D {
	class IVisual3DModule;
};
namespace rpg3D {
	class EditableVoxelisedTemplate;
	class IEntityEditorView_Edit;
	class IVoxelizedEditorM_Edit;
};

namespace rpg3D {
class IEditableVoxelisedM_Edit {
	pub class TouchVoxelTarget {
		pub Vector3Int gPos;
		//VoxelSides
		pub int touchVoxelSide;

		pub TouchVoxelTarget(Vector3Int& gPos, int touchVoxelSide)
			: gPos(gPos), touchVoxelSide(touchVoxelSide)
		{
			//void
		}
	};

	pub class WrappedIndexedVoxelColor {
		pub Color color;

		// -1; Doesn't exist on the material.
		// >= 0; Exists on the material
		pub int registeredVertexColorIndex = -1;

		pub int voxelsUsageCount = 0;

		pub WrappedIndexedVoxelColor(Color& color, int registeredVertexColorIndex)
			: color(color),
			registeredVertexColorIndex(registeredVertexColorIndex)
		{
			//void
		}
	};

	pub virtual IWorldEntity* getEntity() = 0;
	pub virtual EditableVoxelisedTemplate* getTemplate() = 0;

	pub virtual void enableEditing(bool isEditingActive, bool isEditorStructureEnabled, IWorldEntity* editableEntityB) = 0;
	pub virtual void disableEditing() = 0;

	pub virtual void setEntityEditorView(IEntityEditorView_Edit* entityEditorView) = 0;
	pub virtual void setEditor(IVoxelizedEditorM_Edit* editorEntity_editor) = 0;
	
	pub virtual bool getIsEditorStructureEnabled() = 0;

	pub virtual IContainer3D* getEditorStructure_rotator() = 0;
	pub virtual IContainer3D* getEditorStructure_childrensHolder() = 0;

	pub virtual worldGame3D::IVisual3DModule* getVisual3D() = 0;

	pub virtual std::shared_ptr<TouchVoxelTarget> getClosestVoxelForRay(Ray& ray, bool computeTouchSide) = 0;

	pub virtual bool isEditingEntitySingleVoxelRemaining() = 0;

	pub virtual int getVoxelMaterialIndex(int gX, int gY, int gZ) = 0;
	pub virtual void setVoxelMaterialIndex(int gX, int gY, int gZ, int editorMatIndex, bool updatesHistory) = 0;

	pub virtual ArrayList<std::shared_ptr<WrappedIndexedVoxelColor>>* peekEditingIndexedVoxelColors() = 0;
	pub virtual std::shared_ptr<WrappedIndexedVoxelColor> getEditorMaterialWithRegisteredVertexColorIndex(int registeredVertexColorIndex, int* oEditorMatIndex) = 0;

	pub virtual int peekActiveDynamicMaterialIndex() = 0;
	pub virtual void setActiveDynamicMaterialIndex(int activeDynamicMaterialIndex) = 0;

	pub virtual int createNewMaterialFromDiffuseColor(int editorColIndex, Color& col, bool updatesHistory) = 0;
	pub virtual void editVoxelMaterialDiffuseColor(int editorColIndex, Color& diffuseColor, bool updatesHistory) = 0;
	pub virtual void removeUnusedVoxelMaterial(int editorColIndex, bool updatesHistory) = 0;

	pub virtual bool getIsHistryUndoAvailable() = 0;
	pub virtual bool getIsHistryRedoAvailable() = 0;
	
	pub virtual void historyUndo() = 0;
	pub virtual void historyRedo() = 0;

	pub virtual void resetEditEntityRotation() = 0;

	pub virtual VoxelGridDrawable3D* peekEditingVoxelsVisual() = 0;
	
	pub virtual void setEditEntity_voxelSize(float voxelSize, bool updatesHistory) = 0;

	pub virtual void setEditEntity_stackable_haulOffsetZM(float haulOffsetZM, bool updatesHistory) = 0;

	pub virtual void setVoxelsGridOffset(float x, float y, float z, bool updatesHistory) = 0;

	pub virtual bool getIsEntityOriginIndicatorActive() = 0;
	pub virtual void setIsEntityOriginIndicatorActive(bool isEntityOriginIndicatorActive) = 0;

	pub virtual bool getIsEntityStackableCarryPosIndicatorActive() = 0;
	pub virtual void setIsEntityStackableCarryPosIndicatorActive(bool isEntityStackableCarryPosIndicatorActive) = 0;

    pub virtual ~IEditableVoxelisedM_Edit() = default;
};
};
