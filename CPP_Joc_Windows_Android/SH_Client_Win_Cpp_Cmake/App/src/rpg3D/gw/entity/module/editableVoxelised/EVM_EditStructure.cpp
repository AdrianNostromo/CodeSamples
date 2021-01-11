#include "EVM_EditStructure.h"
#include <graphics/visual3d/drawable/Container3D.h>
#include <base/visual3d/Visual3DUtil.h>
#include <worldGame3D/gw/entity/module/visual3D/IVisual3DModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>

using namespace rpg3D;

EVM_EditStructure::EVM_EditStructure(
	IWorldEntity* entity,
	EditableVoxelisedTemplate* t)
	: super(
		entity,
		t)
{
	//void
}

void EVM_EditStructure::createBMain() {
	super::createBMain();

	//void
}

void EVM_EditStructure::onIsEditingEnabledChanged() {
	super::onIsEditingEnabledChanged();

	if (this->isEditingEnabled && isEditorStructureEnabled) {
		createEditorStructure();
	} else {
		removeExistingEditorStructure();
	}
}

IContainer3D* EVM_EditStructure::getEditorStructure_rotator() {
	return vHolderA;
}

IContainer3D* EVM_EditStructure::getEditorStructure_childrensHolder() {
	return vHolderB;
}

void EVM_EditStructure::onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) {
	super::onTranslateVoxelsGrid(x, y, z, updatesHistory);

	syncEditorStructureVisualsIfNeeded();
}

void EVM_EditStructure::onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) {
	super::onEditEntity_voxelSize_changed(voxelSize, old_voxelSize, updatesHistory);

	syncEditorStructureVisualsIfNeeded();
}

void EVM_EditStructure::onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) {
	super::onVoxelsGridSizeChange(gSizeBefore, gSizeAfter);

	syncEditorStructureVisualsIfNeeded();
}

void EVM_EditStructure::createEditorStructure() {
	entityRootDrawable = visual3D->peekRootDrawableAsContainer();
	
	vHolderA = Visual3DUtil::newGroup(
		nullptr/*parent*/
	);

	vHolderB = Visual3DUtil::newGroup(
		vHolderA/*parent*/
	);

	// Move all children of entityRootDrawable back to vHolderB;
	while (entityRootDrawable->getChildrensCount() > 0) {
		IDrawableNode3D* vChild = entityRootDrawable->getChildAt(0);
		entityRootDrawable->removeChild(vChild);

		vHolderB->addChild(vChild);
	}

	entityRootDrawable->addChild(vHolderA);

	syncEditorStructureVisualsIfNeeded();
}

void EVM_EditStructure::removeExistingEditorStructure() {
	// Move all children of vHolderB back to entityRootDrawable;
	while (vHolderB->getChildrensCount() > 0) {
		IDrawableNode3D* vChild = vHolderB->getChildAt(0);
		vHolderB->removeChild(vChild);

		entityRootDrawable->addChild(vChild);
	}

	if (vHolderA != nullptr) {
		if (vHolderA->getParent() != nullptr) {
			vHolderA->getParent()->removeChild(vHolderA);
		}
		vHolderA->reservedDisposeMain();
		delete vHolderA;
		vHolderA = nullptr;
	}

	if (vHolderB != nullptr) {
		if (vHolderB->getParent() != nullptr) {
			vHolderB->getParent()->removeChild(vHolderB);
		}
		vHolderB->reservedDisposeMain();
		delete vHolderB;
		vHolderB = nullptr;
	}
}

void EVM_EditStructure::syncEditorStructureVisualsIfNeeded() {
	float entityVoxelSize = editingVoxelsVisual->getVoxelsGrid()->getVoxelSize();
	Vector3Int* entityGSize = editingVoxelsVisual->getVoxelsGrid()->peekGrid()->getCount();
	Vector3* entityGOffset = editingVoxelsVisual->getVoxelsGrid()->getVoxelGridOffset();

	float entityOffsettedCenterZ = (entityGOffset->z + (entityGSize->z / 2.0f)) * entityVoxelSize;

	// One holder is raised to half the entity computed height.
	vHolderA->setPosition(0.0f, 0.0f, entityOffsettedCenterZ);

	// The other holder is lowered by the same amount.
	vHolderB->setPosition(0.0f, 0.0f, -entityOffsettedCenterZ);
}

void EVM_EditStructure::disposeMain() {
	if (isEditStructureVisualsCreated) {
		isEditStructureVisualsCreated = false;

		removeExistingEditorStructure();
	}

	super::disposeMain();
}

EVM_EditStructure::~EVM_EditStructure() {
	//void
}
