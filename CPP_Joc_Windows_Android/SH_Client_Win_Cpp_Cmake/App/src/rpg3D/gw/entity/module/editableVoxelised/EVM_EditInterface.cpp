#include "EVM_EditInterface.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/visual3D/IVisual3DModule.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <rpg3D/gw/view/entityEditor/IEntityEditorView_Edit.h>

using namespace rpg3D;

EVM_EditInterface::EVM_EditInterface(
	IWorldEntity* entity,
	EditableVoxelisedTemplate* t)
	: super(
		entity,
		t)
{
	//void
}

void EVM_EditInterface::createBMain() {
	super::createBMain();

	visual3D = e->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
	
	stackable = e->getComponentAs< IStackableModule*>(true/*mustExist*/);
}

void EVM_EditInterface::onIsEditingEnabledChanged() {
	super::onIsEditingEnabledChanged();

	if (!isEditingEnabled) {
		//void
	} else {
		editingVoxelsVisual = visual3D->getDrawableMustExistAs< VoxelGridDrawable3D*>("_VoxelGridGroup_");

		if (editableEntityB != nullptr) {
			editableEntityB_visual3D = editableEntityB->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
			editableEntityB_editingVoxelsVisual = editableEntityB_visual3D->getDrawableMustExistAs< VoxelGridDrawable3D*>("_VoxelGridGroup_");
		}

		editingIndexedVoxelColors.clear();
		// Wrap all materials that exist on the 3d model (Even if the materials are unused). The ywill be cleaned up below.
		ArrayList<VoxlesGrid3DGeometry::Material*>* materialsList = editingVoxelsVisual->getVoxelsGrid()->peekMaterialsList();
		for (int i = 0; i < materialsList->count(); i++) {
			VoxlesGrid3DGeometry::Material* mat = materialsList->getDirect(i);

			if (mat != nullptr) {
				editingIndexedVoxelColors.appendDirect(std::make_shared< WrappedIndexedVoxelColor>(mat->color, i));
			}
		}

		// Make all voxels that don't have a material to -1.
		// This is made in case some voxels have invalid material index numbers.
		removeInvalidVoxelIndexes();

		// Count each voxel to init the material usage value.
		initVoxelMaterialUsage();

		// Remove materials that are not used. Remove from the visual and this local list.
		// Keep the mat indexes after removal.
		purgeUnusedVisualMaterials();
	}
}

VoxelGridDrawable3D* EVM_EditInterface::peekEditingVoxelsVisual() {
	return editingVoxelsVisual;
}

ArrayList<std::shared_ptr<IEditableVoxelisedM_Edit::WrappedIndexedVoxelColor>>* EVM_EditInterface::peekEditingIndexedVoxelColors() {
	return &editingIndexedVoxelColors;
}

std::shared_ptr<EVM_EditInterface::WrappedIndexedVoxelColor> EVM_EditInterface::getEditorMaterialWithRegisteredVertexColorIndex(int registeredVertexColorIndex, int* oEditorMatIndex) {
	for (int i = 0; i < editingIndexedVoxelColors.size(); i++) {
		std::shared_ptr<WrappedIndexedVoxelColor> editingColor = editingIndexedVoxelColors.getDirect(i);
		if (editingColor->registeredVertexColorIndex == registeredVertexColorIndex) {
			if (oEditorMatIndex != nullptr) {
				*oEditorMatIndex = i;
			}

			return editingColor;
		}
	}

	*oEditorMatIndex = -1;
	return nullptr;
}

int EVM_EditInterface::getVoxelMaterialIndex(int gX, int gY, int gZ) {
	return editingVoxelsVisual->peekVoxelsGrid()->getVoxelMaterialIndex(gX, gY, gZ);
}

// editorMatIndex == -1; Will set the registeredMaterial of 0 which will make the voxel invisible
/// This logic will allow this function to work with sculpt mode later on if needed.
void EVM_EditInterface::setVoxelMaterialIndex(int gX, int gY, int gZ, int editorMatIndex, bool updatesHistory) {
	//Get the old material.
	int originalRegisteredVertexColorIndex = getVoxelMaterialIndex(gX, gY, gZ);
	int originalEditorMatIndex = -1;
	std::shared_ptr<WrappedIndexedVoxelColor> oldWrappedMat = (originalRegisteredVertexColorIndex >= 0) ? getEditorMaterialWithRegisteredVertexColorIndex(originalRegisteredVertexColorIndex, &originalEditorMatIndex) : nullptr;

	// Add the new material to the visual if needed.
	std::shared_ptr<WrappedIndexedVoxelColor> newWrappedMat = (editorMatIndex >= 0) ? editingIndexedVoxelColors.getDirect(editorMatIndex) : nullptr;

	if (newWrappedMat != nullptr) {
		newWrappedMat->voxelsUsageCount++;
		if (newWrappedMat->voxelsUsageCount == 1) {
			if (newWrappedMat->registeredVertexColorIndex >= 0) {
				// The material should not be registered because no voxels was using it.

				throw LogicException(LOC);
			}

			newWrappedMat->registeredVertexColorIndex = editingVoxelsVisual->peekVoxelsGrid()->pushMaterial(newWrappedMat->color);
			if (editableEntityB != nullptr) {
				editableEntityB_editingVoxelsVisual->peekVoxelsGrid()->pushMaterial(newWrappedMat->color);
			}
		}
	}

	// Update the voxel.
	int newRegisteredVertexColorIndex = (newWrappedMat != nullptr) ? newWrappedMat->registeredVertexColorIndex : -1;

	// Ignore if the voxel has the same material index.
	if (originalRegisteredVertexColorIndex == newRegisteredVertexColorIndex) {
		return;
	}

	Vector3Int gSizeBefore{ editingVoxelsVisual->peekVoxelsGrid()->peekGrid()->getCount() };
	Vector3 gOffsetBefore{ editingVoxelsVisual->peekVoxelsGrid()->getVoxelGridOffset() };

	Vector3Int postChangeGridBoundsMin{};
	editingVoxelsVisual->peekVoxelsGrid()->setVoxel(gX, gY, gZ, newRegisteredVertexColorIndex, &postChangeGridBoundsMin);
	if (editableEntityB != nullptr) {
		editableEntityB_editingVoxelsVisual->peekVoxelsGrid()->setVoxel(gX, gY, gZ, newRegisteredVertexColorIndex, nullptr/*oEditedGPosOffsetted*/);
	}

	Vector3Int gSizeAfter{ editingVoxelsVisual->peekVoxelsGrid()->peekGrid()->getCount() };
	if (!gSizeBefore.equals(gSizeAfter)) {
		onVoxelsGridSizeChange(gSizeBefore, gSizeAfter);
	}

	Vector3 gOffsetAdter{ editingVoxelsVisual->peekVoxelsGrid()->getVoxelGridOffset() };
	if (!gOffsetBefore.equals(gOffsetAdter)) {
		onTranslateVoxelsGrid(
			gOffsetAdter.x - gOffsetBefore.x,
			gOffsetAdter.y - gOffsetBefore.y,
			gOffsetAdter.z - gOffsetBefore.z,
			false
			);
	}

	// Remove old material if unused any more.
	if (oldWrappedMat != nullptr) {
		oldWrappedMat->voxelsUsageCount--;
		if (oldWrappedMat->voxelsUsageCount == 0) {
			if (oldWrappedMat->registeredVertexColorIndex < 0) {
				// The old mat should be registered at this point because it was used.

				throw LogicException(LOC);
			}

			editingVoxelsVisual->getVoxelsGrid()->removeUnusedMaterial(oldWrappedMat->registeredVertexColorIndex);
			if (editableEntityB != nullptr) {
				editableEntityB_editingVoxelsVisual->getVoxelsGrid()->removeUnusedMaterial(oldWrappedMat->registeredVertexColorIndex);
			}

			oldWrappedMat->registeredVertexColorIndex = -1;
		}
	}

	Vector3Int gPos{ gX, gY, gZ };
	onVoxelMaterialIndexChanged(
		gPos, postChangeGridBoundsMin,
		originalEditorMatIndex, originalRegisteredVertexColorIndex, 
		editorMatIndex, newRegisteredVertexColorIndex,
		updatesHistory
	);
}

void EVM_EditInterface::onVoxelMaterialIndexChanged(
	Vector3Int& gPos, Vector3Int& postChangeGridBoundsMin,
	int oldEditorMatIndex, int originalRegisteredVoxelMatIndex,
	int newEditorMatIndex, int newRegisteredVertexColorIndex,
	bool updatesHistory)
{
	//void
}

void EVM_EditInterface::setVoxelsGridOffset(float x, float y, float z, bool updatesHistory) {
	Vector3* gOffset = peekEditingVoxelsVisual()->getVoxelsGrid()->getVoxelGridOffset();
	translateVoxelsGrid(
		x - gOffset->x,
		y - gOffset->y,
		z - gOffset->z,
		updatesHistory
	);
}

void EVM_EditInterface::translateVoxelsGrid(float x, float y, float z, bool updatesHistory) {
	if (x == 0 && y == 0 && z == 0) {
		return;
	}
	
	editingVoxelsVisual->getVoxelsGrid()->translateVoxelGrid(x, y, z);

	onTranslateVoxelsGrid(x, y, z, updatesHistory);
}

void EVM_EditInterface::onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) {
	if (entityEditorView != nullptr) {
		entityEditorView->onTranslateVoxelsGrid(x, y, z, updatesHistory);
	}
}

void EVM_EditInterface::setEditEntity_voxelSize(float voxelSize, bool updatesHistory) {
	float old_voxelSize = editingVoxelsVisual->getVoxelsGrid()->getVoxelSize();
	if (voxelSize == old_voxelSize) {
		return;
	}

	editingVoxelsVisual->getVoxelsGrid()->setVoxelSize(voxelSize);

	onEditEntity_voxelSize_changed(voxelSize, old_voxelSize, updatesHistory);
}

void EVM_EditInterface::increaseEditEntity_voxelSize(float delta_voxelSize, bool updatesHistory) {
	setEditEntity_voxelSize(editingVoxelsVisual->getVoxelsGrid()->getVoxelSize() + delta_voxelSize, updatesHistory);
}

void EVM_EditInterface::onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) {
	if (entityEditorView != nullptr) {
		entityEditorView->onEditEntity_voxelSize_changed(voxelSize, old_voxelSize, updatesHistory);
	}
}

void EVM_EditInterface::setEditEntity_stackable_haulOffsetZM(float haulOffsetZM, bool updatesHistory) {
	float old_haulOffsetZM = stackable->getHaulOffsetZM();
	if (haulOffsetZM == old_haulOffsetZM) {
		return;
	}

	stackable->setHaulOffsetZM(haulOffsetZM);

	onEditEntity_stackable_haulOffsetZM_changed(haulOffsetZM, old_haulOffsetZM, updatesHistory);
}

void EVM_EditInterface::increaseEditEntity_stackable_haulOffsetZM(float delta_haulOffsetZM, bool updatesHistory) {
	setEditEntity_stackable_haulOffsetZM(stackable->getHaulOffsetZM() + delta_haulOffsetZM, updatesHistory);
}

void EVM_EditInterface::onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) {
	if (entityEditorView != nullptr) {
		entityEditorView->onEditEntity_stackable_haulOffsetZM_changed(haulOffsetZM, old_haulOffsetZM, updatesHistory);
	}
}

void EVM_EditInterface::onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) {
	if (entityEditorView != nullptr) {
		entityEditorView->onVoxelsGridSizeChange(gSizeBefore, gSizeAfter);
	}
}

int EVM_EditInterface::createNewMaterialFromDiffuseColor(int editorColIndex, Color& col, bool updatesHistory) {
	int index = (editorColIndex >= 0) ? editorColIndex : editingIndexedVoxelColors.size();
	editingIndexedVoxelColors.insertDirect(index, std::make_shared<WrappedIndexedVoxelColor>(col, -1));

	onNewEditorColor(index, col, updatesHistory);

	return index;
}

void EVM_EditInterface::onNewEditorColor(int editorColorIndex, Color& col, bool updatesHistory) {
	if (entityEditorView != nullptr) {
		entityEditorView->onNewEditorColor(editorColorIndex, col, updatesHistory);
	}
}

void EVM_EditInterface::editVoxelMaterialDiffuseColor(int editorColIndex, Color& diffuseColor, bool updatesHistory) {
	// Edit the editor version of the color.
	std::shared_ptr<WrappedIndexedVoxelColor> wrappedMat = editingIndexedVoxelColors.getDirect(editorColIndex);

	if (wrappedMat->color.equals(diffuseColor)) {
		return;
	}

	Color oldCol{ wrappedMat->color };
	wrappedMat->color.set(diffuseColor);
 
	if (wrappedMat->registeredVertexColorIndex >= 0) {
		// If the color is registered, edit the visual version of the color.
		editingVoxelsVisual->getVoxelsGrid()->updateMaterialColor(wrappedMat->registeredVertexColorIndex, diffuseColor);
		if (editableEntityB != nullptr) {
			editableEntityB_editingVoxelsVisual->getVoxelsGrid()->updateMaterialColor(wrappedMat->registeredVertexColorIndex, diffuseColor);
		}

		onEditorRegisteredMaterialChanged();
	}

	onEditorColorChanged(editorColIndex, oldCol, diffuseColor, updatesHistory);
}

void EVM_EditInterface::onEditorRegisteredMaterialChanged() {
	//void
}

void EVM_EditInterface::onEditorColorChanged(int editorColorIndex, Color& oldCol, Color& newCol, bool updatesHistory) {
	if (entityEditorView != nullptr) {
		entityEditorView->onEditorColorChanged(editorColorIndex, oldCol, newCol, updatesHistory);
	}
}

void EVM_EditInterface::removeUnusedVoxelMaterial(int editorColIndex, bool updatesHistory) {
	std::shared_ptr<WrappedIndexedVoxelColor> wrappedMat = editingIndexedVoxelColors.getDirect(editorColIndex);
	if (wrappedMat->voxelsUsageCount > 0) {
		// The material is in use. A check should already have happened.

		throw LogicException(LOC);
	}

	if (wrappedMat->registeredVertexColorIndex >= 0) {
		// Unused material should not be registered.

		throw LogicException(LOC);
	}

	Color col{ wrappedMat->color};
	editingIndexedVoxelColors.remove(editorColIndex);

	onEditorColorRemoved(editorColIndex, col, updatesHistory);
}

void EVM_EditInterface::onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory) {
	if (entityEditorView != nullptr) {
		entityEditorView->onEditorColorRemoved(editorColorIndex, col, updatesHistory);
	}
}

bool EVM_EditInterface::isEditingEntitySingleVoxelRemaining() {
	Array3D<VoxlesGrid3DGeometry::Voxel>* voxelsGrid = editingVoxelsVisual->peekVoxelsGrid()->peekGrid();

	if (voxelsGrid->getCountA() > 1 || voxelsGrid->getCountB() > 1 || voxelsGrid->getCountC() > 1) {
		return false;
	}

	return true;
}

void EVM_EditInterface::removeInvalidVoxelIndexes() {
	Array3D<VoxlesGrid3DGeometry::Voxel>* voxelsGrid = editingVoxelsVisual->peekVoxelsGrid()->peekGrid();
	for (int x = 0; x < voxelsGrid->getLengthA(); x++) {
		for (int y = 0; y < voxelsGrid->getLengthB(); y++) {
			for (int z = 0; z < voxelsGrid->getLengthC(); z++) {
				VoxlesGrid3DGeometry::Voxel* voxel = voxelsGrid->getPointer(x, y, z);

				if (voxel->matIndex != 0) {
					if (voxel->matIndex < 0 || editingVoxelsVisual->getVoxelsGrid()->peekMaterial(voxel->matIndex) == nullptr) {
						// Do a manual update because the voxel is not rendered so no visuals update required.
						//editingVoxelsVisual->peekVoxelsGrid()->setVoxel(x, y, z, -1);
						voxel->matIndex = -1;
					}
				}
			}
		}
	}
}

void EVM_EditInterface::initVoxelMaterialUsage() {
	Array3D<VoxlesGrid3DGeometry::Voxel>* voxelsGrid = editingVoxelsVisual->peekVoxelsGrid()->peekGrid();
	for (int x = 0; x < voxelsGrid->getLengthA(); x++) {
		for (int y = 0; y < voxelsGrid->getLengthB(); y++) {
			for (int z = 0; z < voxelsGrid->getLengthC(); z++) {
				VoxlesGrid3DGeometry::Voxel* voxel = voxelsGrid->getPointer(x, y, z);

				if (voxel->matIndex >= 0) {
					std::shared_ptr<EVM_EditInterface::WrappedIndexedVoxelColor> indexedVoxelColor = getEditorMaterialWithRegisteredVertexColorIndex(voxel->matIndex, nullptr);
					if (indexedVoxelColor == nullptr) {
						throw LogicException(LOC);
					}

					indexedVoxelColor->voxelsUsageCount++;
				}
			}
		}
	}
}

void EVM_EditInterface::purgeUnusedVisualMaterials() {
	for (int i = editingIndexedVoxelColors.size()-1; i>=0; i--) {
		std::shared_ptr<WrappedIndexedVoxelColor> editingColor = editingIndexedVoxelColors.getDirect(i);

		if (editingColor->voxelsUsageCount == 0 && editingColor->registeredVertexColorIndex >= 0) {
			// Unused material exists on visual, remove it from both lists.
			editingVoxelsVisual->getVoxelsGrid()->removeUnusedMaterial(i);
			if (editableEntityB != nullptr) {
				editableEntityB_editingVoxelsVisual->getVoxelsGrid()->removeUnusedMaterial(i);
			}

			editingIndexedVoxelColors.remove(i);
		}
	}
}

worldGame3D::IVisual3DModule* EVM_EditInterface::getVisual3D() {
	return visual3D;
}

EVM_EditInterface::~EVM_EditInterface() {
	//void
}
