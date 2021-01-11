#pragma once

#include <base/gh.h>
#include "EVM_ActiveMaterial.h"

class VoxelGridDrawable3D;
class Visual3DModelMaterial;

namespace rpg3D {
class EVM_EntityOriginIndicator : public EVM_ActiveMaterial {priv typedef EVM_ActiveMaterial super;pub dCtor(EVM_EntityOriginIndicator);
    priv Array3D<signed char> voxelGrid{ 1, 1, 1 };

	priv Vector3 voxelGridOffset_x{0.5f, 0.0f, 0.5f};
	priv ArrayList<Visual3DModelMaterial*> materialsList_x{};

	priv Vector3 voxelGridOffset_y{0.0f, 0.5f, 0.5f};
	priv ArrayList<Visual3DModelMaterial*> materialsList_y{};

	priv Vector3 voxelGridOffset_z{0.0f, 0.0f, 0.5f};
	priv ArrayList<Visual3DModelMaterial*> materialsList_z{};

	priv bool isIndicatorVisaulsCreated = false;
	priv VoxelGridDrawable3D* indicatorVisual3D_x = nullptr;
	priv VoxelGridDrawable3D* indicatorVisual3D_y = nullptr;
	priv VoxelGridDrawable3D* indicatorVisual3D_z = nullptr;

	priv bool isEntityOriginIndicatorActive = false;

	pub explicit EVM_EntityOriginIndicator(
		IWorldEntity* entity,
		EditableVoxelisedTemplate* t);

	pub bool getIsEntityOriginIndicatorActive() final;
	pub void setIsEntityOriginIndicatorActive(bool isEntityOriginIndicatorActive) final;
	prot void virtual onIsEntityOriginIndicatorActiveChanged();

	pub void onIsEditingEnabledChanged() override;

	prot void onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) override;
	prot void onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) override;
	prot void onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) override;

	priv void updateVisualShowStateIfNeeded();
	priv void syncIndicatorScaleIfActive();

	priv void disposeExistingIndicatorVisuals();

	prot void disposeMain() override;
    pub ~EVM_EntityOriginIndicator() override;
};
};
