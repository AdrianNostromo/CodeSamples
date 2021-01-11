#include "EVM_EntityStackableCarryPosIndicator.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <base/visual3d/Visual3DUtil.h>
#include <base/visual3d/material/Visual3DModelMaterial_Color.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <worldGame3D/gw/entity/module/visual3D/Visual3DModule.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>

using namespace rpg3D;

EVM_EntityStackableCarryPosIndicator::EVM_EntityStackableCarryPosIndicator(
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

bool EVM_EntityStackableCarryPosIndicator::getIsEntityStackableCarryPosIndicatorActive() {
	return isEntityStackableCarryPosIndicatorActive;
}

void EVM_EntityStackableCarryPosIndicator::setIsEntityStackableCarryPosIndicatorActive(bool isEntityStackableCarryPosIndicatorActive) {
	if (this->isEntityStackableCarryPosIndicatorActive == isEntityStackableCarryPosIndicatorActive) {
		return;
	}

	this->isEntityStackableCarryPosIndicatorActive = isEntityStackableCarryPosIndicatorActive;
	onIsEntityStackableCarryPosIndicatorActiveChanged();
}

void EVM_EntityStackableCarryPosIndicator::onIsEntityStackableCarryPosIndicatorActiveChanged() {
	updateVisualShowStateIfNeeded();
}

void EVM_EntityStackableCarryPosIndicator::onIsEditingEnabledChanged() {
	super::onIsEditingEnabledChanged();

	if (isEntityStackableCarryPosIndicatorActive) {
		setIsEntityStackableCarryPosIndicatorActive(false);
	} else {
		updateVisualShowStateIfNeeded();
	}
}

void EVM_EntityStackableCarryPosIndicator::onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) {
	super::onEditEntity_stackable_haulOffsetZM_changed(haulOffsetZM, old_haulOffsetZM, updatesHistory);

	syncIndicatorPositionsIfActive();
}

void EVM_EntityStackableCarryPosIndicator::onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) {
	super::onEditEntity_voxelSize_changed(voxelSize, old_voxelSize, updatesHistory);

	if (!isIndicatorVisaulsCreated) {
		return;
	}

	indicatorVisual3D_x->getVoxelsGrid()->setVoxelSize(voxelSize);
	indicatorVisual3D_y->getVoxelsGrid()->setVoxelSize(voxelSize);
	indicatorVisual3D_z->getVoxelsGrid()->setVoxelSize(voxelSize);
}

void EVM_EntityStackableCarryPosIndicator::updateVisualShowStateIfNeeded() {
	bool isShowRequired;
	if (isEditingEnabled && isEntityStackableCarryPosIndicatorActive) {
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

			indicatorVisual3D_y = Visual3DUtil::newVoxelGrid(
				entityVoxelSize,
				&voxelGridOffset_y,
				&voxelGrid,
				&materialsList_y,
				nullptr/*tilingInformation*/
			);

			indicatorVisual3D_z = Visual3DUtil::newVoxelGrid(
				entityVoxelSize,
				&voxelGridOffset_z,
				&voxelGrid,
				&materialsList_z,
				nullptr/*tilingInformation*/
			);

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
			syncIndicatorPositionsIfActive();
		}
	}
}

void EVM_EntityStackableCarryPosIndicator::syncIndicatorScaleIfActive() {
	if (!isIndicatorVisaulsCreated) {
		return;
	}

	indicatorVisual3D_x->setScale(5.0f, 1.0f, 1.0f);
	indicatorVisual3D_y->setScale(1.0f, 5.0f, 1.0f);
	indicatorVisual3D_z->setScale(1.0f, 1.0f, 5.0f);
}

void EVM_EntityStackableCarryPosIndicator::syncIndicatorPositionsIfActive() {
	if (!isIndicatorVisaulsCreated) {
		return;
	}

	float haulOffsetZM = stackable->getHaulOffsetZM();

	indicatorVisual3D_x->setPosition(0.0f, 0.0f, haulOffsetZM);
	indicatorVisual3D_y->setPosition(0.0f, 0.0f, haulOffsetZM);
	indicatorVisual3D_z->setPosition(0.0f, 0.0f, haulOffsetZM);
}

void EVM_EntityStackableCarryPosIndicator::disposeExistingIndicatorVisuals() {
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

void EVM_EntityStackableCarryPosIndicator::disposeMain() {
	if (isIndicatorVisaulsCreated) {
		isIndicatorVisaulsCreated = false;

		disposeExistingIndicatorVisuals();
	}

	super::disposeMain();
}

EVM_EntityStackableCarryPosIndicator::~EVM_EntityStackableCarryPosIndicator() {
	//void
}
