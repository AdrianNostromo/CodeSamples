#pragma once

#include <base/gh.h>
#include "EEVSettingsDrawerA.h"
#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>

namespace base {
	class SpriteButton2D;
};

namespace rpg3D {
class EEV_GeneralInterface : public EEVSettingsDrawerA {priv typedef EEVSettingsDrawerA super;pub dCtor(EEV_GeneralInterface);
	prot static MenuItemConfig* new_viewItemConfig_topArea();
	prot static MenuItemConfig* new_viewItemConfig_bottomArea();

	priv CbTouchListener touchListener{};

	priv base::SpriteButton2D* btnBack;
	priv base::SpriteButton2D* btnSettingsDrawerA;

	priv base::SpriteButton2D* btnResetRotation;

	priv base::SpriteButton2D* btnRedo;
	priv base::SpriteButton2D* btnUndo;

	priv base::SpriteButton2D* btnSettingsDrawrA;

	pub explicit EEV_GeneralInterface(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	prot void onEntityToEditChanged() override;

	pub void onHistoryUndoAvailableChanged(bool isHistryRedoAvailable) override;
	pub void onHistoryRedoAvailableChanged(bool isHistryRedoAvailable) override;

	pub std::string computeStateFlags(std::string baseState) override;

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	pub ~EEV_GeneralInterface() override;
};
};
