#pragma once

#include <base/gh.h>
#include "MIGUIVAT_Selector.h"

class Sprite2D;
namespace base {
	class SpriteButton2D;
};
namespace rpg3D {
	class ToolEvent;
};

namespace rpg3D {
class MIGUIVAT_SecondaryControls : public MIGUIVAT_Selector {priv typedef MIGUIVAT_Selector super;pub dCtor(MIGUIVAT_SecondaryControls);
	priv class ControlEntry {pub dCtor(ControlEntry);
		pub Sprite2D* bg;
		pub base::SpriteButton2D* btn;

		pub int toolInstanceId = -1;
		pub int controlUid = -1;

		pub explicit ControlEntry(Sprite2D* bg, base::SpriteButton2D* btn)
			: bg(bg), btn(btn)
		{
			//void
		}
	};
    prot static MenuItemConfig* new_viewItemConfig_activeTool_secondaryControls();

	priv CbTouchListener touchListener{};

	priv ArrayList<ControlEntry> secondaryControlsArray{};

	priv bool isToolSecondaryControls = false;
	priv sp<Tool> selectedManualTool = nullptr;
	priv ArrayList<std::shared_ptr<IEventListener>> selectedManualTool_autoListenersList{};

    pub explicit MIGUIVAT_SecondaryControls(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	prot void onSelectedFocusEntityChange_pre() override;
	prot void onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) override;

	priv void selectedFocusEntity_onEvent(IEventListener& eventListener, base::EntityEvent& event);
	priv void selectedManualTool_extraActivationSecondaryControls(IEventListener& eventListener, rpg3D::ToolEvent& event);

	priv void syncSecondaryEntries();
	// Use a void* to avoid including ITool here.
	priv inline void updateEntryConfig(ControlEntry& controlEntry, void*/* rpg3D::ITool::ExtraActivationSecondaryControl* */ extraActivationSecondaryControl);
	priv ControlEntry* getEntryWithUid(int entryUid);

	pub std::string computeStateFlags(std::string baseState) override;

    pub ~MIGUIVAT_SecondaryControls() override;
};
};
