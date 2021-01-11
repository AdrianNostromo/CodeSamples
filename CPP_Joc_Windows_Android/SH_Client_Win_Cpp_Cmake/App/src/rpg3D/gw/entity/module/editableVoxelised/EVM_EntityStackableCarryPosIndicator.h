#pragma once

#include <base/gh.h>
#include "EVM_EntityOriginIndicator.h"

namespace rpg3D {
class EVM_EntityStackableCarryPosIndicator : public EVM_EntityOriginIndicator {priv typedef EVM_EntityOriginIndicator super;pub dCtor(EVM_EntityStackableCarryPosIndicator);
	priv Array3D<signed char> voxelGrid{ 1, 1, 1 };

	priv Vector3 voxelGridOffset_x{ 0.5f, 0.0f, 0.5f };
	priv ArrayList<Visual3DModelMaterial*> materialsList_x{};

	priv Vector3 voxelGridOffset_y{ 0.0f, 0.5f, 0.5f };
	priv ArrayList<Visual3DModelMaterial*> materialsList_y{};

	priv Vector3 voxelGridOffset_z{ 0.0f, 0.0f, 0.5f };
	priv ArrayList<Visual3DModelMaterial*> materialsList_z{};

	priv bool isIndicatorVisaulsCreated = false;
	priv VoxelGridDrawable3D* indicatorVisual3D_x = nullptr;
	priv VoxelGridDrawable3D* indicatorVisual3D_y = nullptr;
	priv VoxelGridDrawable3D* indicatorVisual3D_z = nullptr;

	priv bool isEntityStackableCarryPosIndicatorActive = false;

	pub explicit EVM_EntityStackableCarryPosIndicator(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);

	pub bool getIsEntityStackableCarryPosIndicatorActive() final;
	pub void setIsEntityStackableCarryPosIndicatorActive(bool isEntityStackableCarryPosIndicatorActive) final;
	prot void virtual onIsEntityStackableCarryPosIndicatorActiveChanged();

	pub void onIsEditingEnabledChanged() override;

	prot void onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) override;
	prot void onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) override;

	priv void updateVisualShowStateIfNeeded();
	priv void syncIndicatorScaleIfActive();
	priv void syncIndicatorPositionsIfActive();

	priv void disposeExistingIndicatorVisuals();

	prot void disposeMain() override;
    pub ~EVM_EntityStackableCarryPosIndicator() override;
};
};
