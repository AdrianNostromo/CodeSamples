#pragma once

#include <base/gh.h>
#include "EEV_Base.h"
#include <graphics/visual2d/drawable/ScrollList2D.h>
#include <base/view/colorSelector/IColorSelectorView.h>
#include <base/app/IAppUtils.h>
#include <base/view/dialog/listener/CbAutoListenerDialogView.h>

namespace rpg3D {
class EEV_ToolbarColors : public EEV_Base {priv typedef EEV_Base super;prot dCtor(EEV_ToolbarColors);
	// This limit is arbitrary. Can be increased some if needed (I feel like this should be kept small-ish).
	priv static const int VOXEL_MODEL_MAX_VERTEX_COLORS_COUNT;

	priv static const int LIST_ENTRY_TYPE_COLOR;
	priv static const int LIST_ENTRY_TYPE_ADD_NEW_COLOR;

	priv static const int COLOR_SELECTOR_BUTTON_ID_ADD;
	priv static const int COLOR_SELECTOR_BUTTON_ID_CANCEL;
	priv static const int COLOR_SELECTOR_BUTTON_ID_UPDATE;
	priv static const int COLOR_SELECTOR_BUTTON_ID_DELETE;

	priv base::ScrollList2D::CbListener scrollListEventListener{};
	priv base::ScrollList2D* colorsHScrollList = nullptr;
	
	priv static ArrayList<StateChangeDurations*>* listItemConfig_animationDurations;

	priv base::ListEntry2D* activeColorListEntry = nullptr;

	priv base::IColorSelectorView::CbListener colorSelectorEventListener{};
	priv base::CbAutoListenerDialogView dialogEventListener{};

	priv bool isColorsSelectorActive = false;

	prot static MenuItemConfig* new_menuItemConfigs_topArea_toolbarColors();

	prot static ArrayList<MenuItemConfig*>* listItemConfig_colorSelector;
	prot static ArrayList<MenuItemConfig*>* listItemConfig_addColor;
	
	priv base::IDialogView* dialogView = nullptr;
	priv base::IColorSelectorView* colorSelectorView = nullptr;

	pub explicit EEV_ToolbarColors(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	prot void onEntityToEditChanged() override;

	pub void setIsColorsSelectorActive(bool isColorsSelectorActive) final;

	prot void onSelectedStateChanged() override;
	pub std::string computeStateFlags(std::string baseState) override;

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	pub void onActiveDrawDynamicMaterialIndexChanged() final;
	pub void onNewEditorColor(int editorColorIndex, Color& col, bool updatesHistory) final;
	pub void onEditorColorChanged(int editorColorIndex, Color& oldCol, Color& newCol, bool updatesHistory) final;
	pub void onEditorColorRemoved(int editorColorIndex, Color& col, bool updatesHistory) final;

	priv void colorsHScrollList_onEntryTouched(base::List2D* list, base::ListEntry2D* listItem);

	priv void colorSelector_onDynamicButtonActivated(base::IColorSelectorView* view, base::IColorSelectorView::DynamicTextButtonConfig* dynamicButtonConfig, Color& activeColor);
	priv void colorSelector_onCloseRequested(base::IColorSelectorView* view);

	priv void dialog_onDynamicButtonActivated(base::IDialogView* view, base::DynamicTextButtonConfig* dynamicButtonConfig);
	priv void dialog_onCloseRequested(base::IDialogView* view);
	priv void dialog_onContentAreaTouched(base::IDialogView* view);

	priv void showColorFailedInUseDialog(int voxelsUsageCount);
	priv void showColorsCountLimitReached(int maxColorCount);

	priv base::ListEntry2D* createNewColorsListEntry(Color& diffuseColor, int editorColorIndex);
	priv void updateColorEntryActiveStateVisuals(base::ListEntry2D* listEntry, bool isActive);
	
	priv void cleanup_dialogViewIfAny();
	priv void cleanup_colorSelectorViewIfAny();

	prot void disposeMain() override;
	pub ~EEV_ToolbarColors() override;
};
};
