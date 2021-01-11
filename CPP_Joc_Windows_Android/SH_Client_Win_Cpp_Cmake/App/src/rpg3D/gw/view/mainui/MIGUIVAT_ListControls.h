#pragma once

#include <base/gh.h>
#include "MIGUIVAT_SecondaryControls.h"
#include <graphics/visual2d/drawable/ScrollList2D.h>

namespace base {
    class ListEntry2D;
};
namespace rpg3D {
    class ToolEvent;
};

namespace rpg3D {
class MIGUIVAT_ListControls : public MIGUIVAT_SecondaryControls {priv typedef MIGUIVAT_SecondaryControls super;pub dCtor(MIGUIVAT_ListControls);
    priv static const int LIST_ENTRY_TYPE_GENERAL_SELECTABLE;

    prot static MenuItemConfig* new_viewItemConfig_activeTool_listControls();

    priv static ArrayList<StateChangeDurations*>* listItemConfig_animationDurations;
    prot static ArrayList<MenuItemConfig*>* listItemConfig_generalSelectable;

    priv base::ListEntry2D* activeListEntry = nullptr;

    priv base::ScrollList2D::CbListener scrollListEventListener{};
    priv base::ScrollList2D* hScrollList = nullptr;

    priv bool isToolListControls = false;
    priv sp<Tool> selectedManualTool = nullptr;
    priv ArrayList<std::shared_ptr<IEventListener>> selectedManualTool_autoListenersList{};

    pub explicit MIGUIVAT_ListControls(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);
    prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

    priv void updateGeneralEntryActiveStateVisuals(base::ListEntry2D* listEntry, bool isActive);

    priv void hScrollList_onEntryTouched(base::List2D* list, base::ListEntry2D* listItem);

    prot void onSelectedFocusEntityChange_pre() override;
    prot void onSelectedFocusEntityChanged(IWorldEntity* selectedFocusEntity, ArrayList<std::shared_ptr<IEventListener>>& selectedFocusEntity_autoListenersList) override;

    priv void selectedFocusEntity_onEvent(IEventListener& eventListener, base::EntityEvent& event);
    priv void selectedManualTool_extraActivationListControls(IEventListener& eventListener, rpg3D::ToolEvent& event);

    priv void syncHScrollListEntries();

    pub std::string computeStateFlags(std::string baseState) override;

    pub ~MIGUIVAT_ListControls() override;
};
};
