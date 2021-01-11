#pragma once

#include <base/gh.h>
#include "MIGUIVAT_ListControls.h"
#include <base/interaction/CbTouchListener.h>

class ISprite2D;
namespace base {
	class EntityEvent;
};

namespace rpg3D {
class MIGUIVAT_Indicator : public MIGUIVAT_ListControls {priv typedef MIGUIVAT_ListControls super;pub dCtor(MIGUIVAT_Indicator);
	prot static MenuItemConfig* new_viewItemConfig_activeTool_indicator();
	priv static MenuItemConfig* new_viewItemConfig_activeTool_indicator_indicatorArea();

	priv CbTouchListener touchListener{};

	priv IInteractiveDrawable2D* taBg = nullptr;

	priv ISprite2D* toolIcon = nullptr;

	// Sellected or not.
	priv bool hasAnyManuallyActivatedTool = false;
	priv bool hasSelectedManualTool = false;

	pub explicit MIGUIVAT_Indicator(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);
	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	prot void onSelectedFocusEntityChange_pre() override;
	prot void onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) override;

	priv void selectedFocusEntity_onEvent(IEventListener& eventListener, base::EntityEvent& event);

	pub std::string computeStateFlags(std::string baseState) override;

    pub ~MIGUIVAT_Indicator() override;
};
};
