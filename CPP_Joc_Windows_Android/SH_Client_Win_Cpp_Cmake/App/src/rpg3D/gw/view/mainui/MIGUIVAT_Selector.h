#pragma once

#include <base/gh.h>
#include "MIGUIVAT_Base.h"
#include <graphics/visual2d/drawable/ScrollList2D.h>
#include <base/interaction/CbTouchListener.h>

namespace base {
	class EntityEvent;
};
namespace rpg3D {
	class Tool;
};

namespace rpg3D {
class MIGUIVAT_Selector : public MIGUIVAT_Base {priv typedef MIGUIVAT_Base super;pub dCtor(MIGUIVAT_Selector);
	prot static MenuItemConfig* new_viewItemConfig_activeTool_selector();

	priv static ArrayList<MenuItemConfig*>* listItemConfig_tool;
	priv static ArrayList<StateChangeDurations*>* listItemConfig_animationDurations;

	priv CbTouchListener touchListener{};
	priv base::ScrollList2D::CbListener scrollListEventListener{};

	priv IInteractiveDrawable2D* taScreen = nullptr;
	priv base::ScrollList2D* slToolsSelect = nullptr;

	priv bool isSelectoListActive = false;
	priv bool isSelectoListMiniMode = false;
	priv base::ListEntry2D* activeListEntry = nullptr;
	priv bool isSlectorEntriesDirty = false;

    pub explicit MIGUIVAT_Selector(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);
	priv void slToolsSelect_onEntryTouched(base::List2D* list, base::ListEntry2D* listItem);
	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	prot void onSelectedFocusEntityChange_pre() override;
	prot void onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) override;

	priv void selectedFocusEntity_onEvent(IEventListener& eventListener, base::EntityEvent& event);

	priv base::ListEntry2D* createNewListEntry(
		std::string& atlasId, std::string& regionId, bool isActive, bool doAnimation, sp<Tool> tool,
		std::string& toolName_short, std::string& toolNameModifier_short);

	prot bool getIsToolSelectorActive();
	prot bool getIsToolSelectorActiveAndMinimode();
	prot void setIsToolSelectorActive(bool isActive, bool isMinimode);

	priv void showToolSelector();
	priv void hideToolSelectorIfActive();

	priv void updateSelectorEntries();

	pub void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) override;

	pub std::string computeStateFlags(std::string baseState) override;

    pub ~MIGUIVAT_Selector() override;
};
};
