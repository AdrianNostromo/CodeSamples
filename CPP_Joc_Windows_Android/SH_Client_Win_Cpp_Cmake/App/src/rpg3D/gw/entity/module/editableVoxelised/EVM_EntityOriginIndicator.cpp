#include "EVM_EntityOriginIndicator.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <base/visual3d/Visual3DUtil.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>

using namespace rpg3D;

EVM_EntityOriginIndicator::EVM_EntityOriginIndicator(
	IWorldEntity* entity,
	EditableVoxelisedTemplate* t)
	: super(
		entity,
		t)
{

	voxelGrid.setDirect(0, 0, 0, 0/*matIndex*/);

	materialsList_x.appendDirect(new Visual3DModelMaterial_Color(
		new Color("#FF0000FF")
	));

	materialsList_y.appendDirect(new Visual3DModelMaterial_Color(
		new Color("#00FF00FF")
	));

	materialsList_z.appendDirect(new Visual3DModelMaterial_Color(
		new Color("#0000FFFF")
	));
}

bool EVM_EntityOriginIndicator::getIsEntityOriginIndicatorActive() {
	return isEntityOriginIndicatorActive;
}

void EVM_EntityOriginIndicator::setIsEntityOriginIndicatorActive(bool isEntityOriginIndicatorActive) {
	if (this->isEntityOriginIndicatorActive == isEntityOriginIndicatorActive) {
		return;
	}

	this->isEntityOriginIndicatorActive = isEntityOriginIndicatorActive;
	onIsEntityOriginIndicatorActiveChanged();
}

void EVM_EntityOriginIndicator::onIsEntityOriginIndicatorActiveChanged() {
	updateVisualShowStateIfNeeded();
}

void EVM_EntityOriginIndicator::onIsEditingEnabledChanged() {
	super::onIsEditingEnabledChanged();

	if (isEntityOriginIndicatorActive) {
		setIsEntityOriginIndicatorActive(false);
	} else {
		updateVisualShowStateIfNeeded();
	}
}

void EVM_EntityOriginIndicator::onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) {
	super::onTranslateVoxelsGrid(x, y, z, updatesHistory);

	syncIndicatorScaleIfActive();
}

void EVM_EntityOriginIndicator::onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) {
	super::onEditEntity_voxelSize_changed(voxelSize, old_voxelSize, updatesHistory);

	if (!isIndicatorVisaulsCreated) {
		return;
	}

	indicatorVisual3D_x->getVoxelsGrid()->setVoxelSize(voxelSize);
	indicatorVisual3D_y->getVoxelsGrid()->setVoxelSize(voxelSize);
	indicatorVisual3D_z->getVoxelsGrid()->setVoxelSize(voxelSize);
}

void EVM_EntityOriginIndicator::onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) {
	super::onVoxelsGridSizeChange(gSizeBefore, gSizeAfter);

	syncIndicatorScaleIfActive();
}

void EVM_EntityOriginIndicator::updateVisualShowStateIfNeeded() {
	bool isShowRequired;
	if (isEditingEnabled && isEntityOriginIndicatorActive) {
		isShowRequired = true;
	} else {
		isShowRequired = false;
	}

	if (!isShowRequired) {
		if (isIndicatorVisaulsCreated) {
			isIndicatorVisaulsCreated = false;

			disposeExistingIndicatorVisuals();
		}
	} else {
		if (!isIndicatorVisaulsCreated) {
			isIndicatorVisaulsCreated = true;

			float entityVoxelSize = peekEditingVoxelsVisual()->getVoxelsGrid()->getVoxelSize();

			worldGame3D::IVisual3DModule* visual3D = e->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);

			indicatorVisual3D_x = Visual3DUtil::newVoxelGrid(
				entityVoxelSize,
				&voxelGridOffset_x,
				&voxelGrid,
				&materialsList_x,
				nullptr/*tilingInformation*/
			);
			indicatorVisual3D_x->setPosition(0.0f, 0.0f, 0.0f);

			indicatorVisual3D_y = Visual3DUtil::newVoxelGrid(
				entityVoxelSize,
				&voxelGridOffset_y,
				&voxelGrid,
				&materialsList_y,
				nullptr/*tilingInformation*/
			);
			indicatorVisual3D_y->setPosition(0.0f, 0.0f, 0.0f);

			indicatorVisual3D_z = Visual3DUtil::newVoxelGrid(
				entityVoxelSize,
				&voxelGridOffset_z,
				&voxelGrid,
				&materialsList_z,
				nullptr/*tilingInformation*/
			);
			indicatorVisual3D_z->setPosition(0.0f, 0.0f, 0.0f);

			if (getIsEditorStructureEnabled()) {
				getEditorStructure_childrensHolder()->addChild(indicatorVisual3D_x);
				getEditorStructure_childrensHolder()->addChild(indicatorVisual3D_y);
				getEditorStructure_childrensHolder()->addChild(indicatorVisual3D_z);
			} else {
				visual3D->peekRootDrawableAsContainer()->addChild(indicatorVisual3D_x);
				visual3D->peekRootDrawableAsContainer()->addChild(indicatorVisual3D_y);
				visual3D->peekRootDrawableAsContainer()->addChild(indicatorVisual3D_z);
			}

			syncIndicatorScaleIfActive();
		}
	}
}

void EVM_EntityOriginIndicator::syncIndicatorScaleIfActive() {
	if (!isIndicatorVisaulsCreated) {
		return;
	}

	Vector3Int* entityGSize = peekEditingVoxelsVisual()->getVoxelsGrid()->peekGrid()->getCount();
	Vector3* entityGOffset = peekEditingVoxelsVisual()->getVoxelsGrid()->getVoxelGridOffset();

	// Remove 0.5 so the edge of the axes is at the edge of the entity. Add 2 to make the axes extend 2 voxel distances outside the entity.
	indicatorVisual3D_x->setScale(entityGSize->x + entityGOffset->x - 0.5f + 2.0f, 1.0f, 1.0f);
	indicatorVisual3D_y->setScale(1.0f, entityGSize->y + entityGOffset->y - 0.5f + 2.0f, 1.0f);
	indicatorVisual3D_z->setScale(1.0f, 1.0f, entityGSize->z + entityGOffset->z - 0.5f + 2.0f);
}

void EVM_EntityOriginIndicator::disposeExistingIndicatorVisuals() {
	if (indicatorVisual3D_x->getParent() != nullptr) {
		indicatorVisual3D_x->getParent()->removeChild(indicatorVisual3D_x);
	}
	indicatorVisual3D_x->reservedDisposeMain();
	delete indicatorVisual3D_x;
	indicatorVisual3D_x = nullptr;

	if (indicatorVisual3D_y->getParent() != nullptr) {
		indicatorVisual3D_y->getParent()->removeChild(indicatorVisual3D_y);
	}
	indicatorVisual3D_y->reservedDisposeMain();
	delete indicatorVisual3D_y;
	indicatorVisual3D_y = nullptr;

	if (indicatorVisual3D_z->getParent() != nullptr) {
		indicatorVisual3D_z->getParent()->removeChild(indicatorVisual3D_z);
	}
	indicatorVisual3D_z->reservedDisposeMain();
	delete indicatorVisual3D_z;
	indicatorVisual3D_z = nullptr;
}

void EVM_EntityOriginIndicator::disposeMain() {
	if (isIndicatorVisaulsCreated) {
		isIndicatorVisaulsCreated = false;

		disposeExistingIndicatorVisuals();
	}

	super::disposeMain();
}

EVM_EntityOriginIndicator::~EVM_EntityOriginIndicator() {
	//void
}
