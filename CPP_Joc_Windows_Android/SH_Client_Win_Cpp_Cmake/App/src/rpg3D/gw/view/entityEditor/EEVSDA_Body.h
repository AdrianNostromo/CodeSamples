#pragma once

#include <base/gh.h>
#include "EEVSDA_Base.h"
#include <graphics/visual2d/drawable/ToggleSwitch2D.h>
#include <base/math/Vector3Int.h>

namespace base {
	class BitmapLabel;
	class SpriteButton2D;
}

namespace rpg3D {
class EEVSDA_Body : public EEVSDA_Base {priv typedef EEVSDA_Base super;pub dCtor(EEVSDA_Body);
	priv class ValBounds {
		pub static const float gridOffset_x_min;
		pub static const float gridOffset_x_max;
		pub static const float gridOffset_y_min;
		pub static const float gridOffset_y_max;
		pub static const float gridOffset_z_min;
		pub static const float gridOffset_z_max;

		pub static const float voxelSize_min;
		pub static const float voxelSize_max;

		pub static const float haulOffsetZM_min;
		pub static const float haulOffsetZM_max;

	};

	prot static MenuItemConfig* new_viewItemConfig_settingsDrawerA_body();

	prot static MenuItemConfig* new_viewItemConfig_settingsDrawerA_body_gridSize();
	prot static MenuItemConfig* new_viewItemConfig_settingsDrawerA_body_origin();
	prot static MenuItemConfig* new_viewItemConfig_settingsDrawerA_body_gridOffset();
	prot static MenuItemConfig* new_viewItemConfig_settingsDrawerA_body_voxelSize();
	prot static MenuItemConfig* new_viewItemConfig_settingsDrawerA_body_carryOffsetZ();
	prot static MenuItemConfig* new_viewItemConfig_settingsDrawerA_body_carryOriginIndicator();

	priv CbTouchListener touchListener{};
	priv base::ToggleSwitch2D::CbListener toggleSwitchListener{};

	priv IInteractiveDrawable2D* taBg = nullptr;

	priv base::ToggleSwitch2D* tbtnShowOrigin = nullptr;

	priv base::SpriteButton2D* btnGridOffsetXDecrement = nullptr;
	priv base::SpriteButton2D* btnGridOffsetXIncrement = nullptr;
	priv base::SpriteButton2D* btnGridOffsetYDecrement = nullptr;
	priv base::SpriteButton2D* btnGridOffsetYIncrement = nullptr;
	priv base::SpriteButton2D* btnGridOffsetZDecrement = nullptr;
	priv base::SpriteButton2D* btnGridOffsetZIncrement = nullptr;

	priv bool isInitialDataSync_voxelSize = false;
	priv float syncData_voxelSize = 0.0f;
	priv base::BitmapLabel* voxelSize_dlblSubtitle = nullptr;
	priv base::SpriteButton2D* btnDecreaseVoxelSize = nullptr;
	priv base::SpriteButton2D* btnIncreaseVoxelSize = nullptr;

	priv bool isInitialDataSync_stackable_haulOffsetZM = false;
	priv float syncData_stackable_haulOffsetZM = 0.0f;
	priv base::BitmapLabel* carryOffsetZ_dlblSubtitle = nullptr;
	priv base::SpriteButton2D* btnDecreaseCarryOffsetZ = nullptr;
	priv base::SpriteButton2D* btnIncreaseCarryOffsetZ = nullptr;

	priv base::BitmapLabel* dlblGSizeX = nullptr;
	priv base::BitmapLabel* dlblGSizeY = nullptr;
	priv base::BitmapLabel* dlblGSizeZ = nullptr;

	priv base::BitmapLabel* dlblGridOffsetX = nullptr;
	priv base::BitmapLabel* dlblGridOffsetY = nullptr;
	priv base::BitmapLabel* dlblGridOffsetZ = nullptr;

	priv base::ToggleSwitch2D* tbtnShowCarryOrigin = nullptr;

	priv bool isInitialDataSync_gSize = false;
	priv Vector3Int syncData_gSize{};

	priv bool isInitialDataSync_gOffset = false;
	priv Vector3 syncData_gOffset{};

    pub explicit EEVSDA_Body(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	priv void v2d_onSelectedStateChanged(base::IToggleControl2D* target);

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	prot void onEntityToEditChanged() override;
	pub void onTranslateVoxelsGrid(float x, float y, float z, bool updatesHistory) override;
	pub void onEditEntity_voxelSize_changed(float voxelSize, float old_voxelSize, bool updatesHistory) override;
	pub void onEditEntity_stackable_haulOffsetZM_changed(float haulOffsetZM, float old_haulOffsetZM, bool updatesHistory) override;
	pub void onVoxelsGridSizeChange(Vector3Int& gSizeBefore, Vector3Int& gSizeAfter) override;

	priv void syncIsPossible_gridSize();
	priv void syncIsPossible_gridOffset();
	priv void syncIsPossible_voxelSize();
	priv void syncIsPossible_stackable_haulOffsetZM();

    pub ~EEVSDA_Body() override;
};
};
