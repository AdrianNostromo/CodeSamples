#pragma once

#include <base/gh.h>
#include "EVM_Base.h"
#include <graphics/visual/Color.h>
#include <base/math/Vector3Int.h>

class VoxelGridDrawable3D;
namespace rpg3D {
	class IStackableModule;
};

namespace rpg3D {
class EVM_EditInterface : public editableEntityModule::EVM_Base {priv typedef editableEntityModule::EVM_Base super;pub dCtor(EVM_EditInterface);
	prot worldGame3D::IVisual3DModule* visual3D = nullptr;
	
	prot rpg3D::IStackableModule* stackable = nullptr;
	// This is a dynamic list.
	// It contains all editor materials.
	// Only the used materials will be added to the visual.
	// This list doesn't mirror the visual voxel colors list because the visual colors list may be split eventually over multiple materials.
	priv ArrayList<std::shared_ptr<WrappedIndexedVoxelColor>> editingIndexedVoxelColors{};
	prot VoxelGridDrawable3D* editingVoxelsVisual = nullptr;

	// This entity also receive edit commands. This is used to also edit the player entity when the dummy entity is edited.
	priv worldGame3D::IVisual3DModule* editableEntityB_visual3D = nullptr;
	priv VoxelGridDrawable3D* editableEntityB_editingVoxelsVisual = nullptr;

    pub explicit EVM_EditInterface(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);
	prot void createBMain() override;

	pub void onIsEditingEnabledChanged() override;

	pub VoxelGridDrawable3D* peekEditingVoxelsVisual() final;

	pub ArrayList<std::shared_ptr<WrappedIndexedVoxelColor>>* peekEditingIndexedVoxelColors() final;
	pub std::shared_ptr<WrappedIndexedVoxelColor> getEditorMaterialWithRegisteredVertexColorIndex(int registeredVertexColorIndex, int* oEditorMatIndex) final;

	// editorColIndex == -1; Append.
	pub int createNewMaterialFromDiffuseColor(int editorColIndex, Color& col, bool updatesHistory) final;
	prot virtual void onNewEditorColor(int editorColorIndex, Color& col, bool updatesHistory);

	pub void editVoxelMaterialDiffuseColor(int editorColIndex, Color& diffuseColor, bool updatesHistory) final;
	prot virtual void onEditorRegisteredMaterialChanged();
	prot virtual void onEditorColorChanged(int editorColorIndex, Color& oldCol, Color& newCol, bool updatesHistory);

	pub void removeUnusedVoxelMaterial(int editorColIndex, bool updatesHistory) final;
	prot virtual void onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory);

	pub bool isEditingEntitySingleVoxelRemaining() final;

	pub int getVoxelMaterialIndex(int gX, int gY, int gZ) final;
	pub void setVoxelMaterialIndex(int gX, int gY, int gZ, int editorMatIndex, bool updatesHistory) final;
	prot virtual void onVoxelMaterialIndexChanged(
		Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin,
		int oldEditorMatIndex, int originalRegisteredVoxelMatIndex,
		int newEditorMatIndex, int newRegisteredVertexColorIndex,
		bool updatesHistory);

	pub void setVoxelsGridOffset(float x, float y, float z, bool updatesHistory) final;
	prot void translateVoxelsGrid(float x, float y, float z, bool updatesHistory);
	prot virtual void onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory);
	
	pub void setEditEntity_voxelSize(float voxelSize, bool updatesHistory) final;
	prot void increaseEditEntity_voxelSize(float delta_voxelSize, bool updatesHistory);
	prot virtual void onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory);
	
	pub void setEditEntity_stackable_haulOffsetZM(float haulOffsetZM, bool updatesHistory) final;
	prot void increaseEditEntity_stackable_haulOffsetZM(float delta_haulOffsetZM, bool updatesHistory);
	prot virtual void onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory);
	
	// This is automatic always and doesn't update history.
	prot virtual void onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter);

	priv void removeInvalidVoxelIndexes();
	priv void initVoxelMaterialUsage();
	priv void purgeUnusedVisualMaterials();

	pub worldGame3D::IVisual3DModule* getVisual3D() final;

	pub ~EVM_EditInterface() override;
};
};
