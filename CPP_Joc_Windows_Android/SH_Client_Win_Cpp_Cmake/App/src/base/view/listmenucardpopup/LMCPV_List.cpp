#include "LMCPV_List.h"
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/config/sounds/SoundConfigsList.h>

using namespace base;

/*
// NOTE - Example code only. Must be instantiated in "local" when used.
public static final MenuItemConfig[] DEFAULT_listEntryConfig = new MenuItemConfig[] {
		new MenuItemConfig_Data("size", new VisualPropertiesConfig()
				->put("width",
						1f, "_dataMap", "entryWidthDip",
						VisualValueConfig.OPERATION_MULTIPLY,
						1f, "_dpm.width"
				)
				->put("height", 38f, "_dpm.width")
		),

		new MenuItemConfig_Visual("layer", null, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
		),
		new MenuItemConfig_Visual("bg", "layer", VisualCreateConfig.newSprite("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
				->put("width", 1f, "_local.size", "width")
				->put("height", 1f, "_local.size", "height")
				->put("anchorX", 0.0f, null, null)
				->put("anchorY", 0.0f, null, null)
				->put("x", 0f, null, null)
				->put("y", 0f, null, null)
				// Use a alpha of 0 because there is already a white background. If alpha is not 0, there will be fade artefacts on menu show,hide.
				->put("alpha", 0.0f)
		),
		new MenuItemConfig_Visual("touchArea", "layer", VisualCreateConfig.newTouchAreaAudible(null, null), new VisualPropertiesConfig()
				->put("width", 1f, "_local.size", "width")
				->put("height", 1f, "_local.size", "height")
				->put("anchorX", 0.0f)
				->put("anchorY", 0.0f)
				->put("x", 0f)
				->put("y", 0f)
		),
		new MenuItemConfig_Visual("lblText", "layer", VisualCreateConfig.newLabel(FontInstances.FC_MSVPH_Common_Regular_AaN.usageFontID, "TYPE"), new VisualPropertiesConfig()
				->put("anchorX", 0.5f, null, null)
				->put("anchorY", 0.5f, null, null)
				->put("x", 0.5f, "_local.size", "width")
				->put("y", 0.5f, "_local.size", "height")
		)
};
public static final StateChangeDurations[] DEFAULT_listEntryAnimationDurations = new StateChangeDurations[] {
		new StateChangeDurations("_in_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
		new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
		new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
};
*/

MenuItemConfig* LMCPV_List::new_menuItemConfigs_contentArea_optionsList() {
	return new MenuItemConfig_SmartItem("optionsList", "layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual* { return new List2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
		->put("x", 0.0f, "_dpm.width")
		->put("y", 0.0f, "_dpm.width")

		->put("anchorX", 0.0f)
		->put("anchorY", 1.0f)

		->put("entryActivateSoundConfig_id", &SoundConfigsList::UserInterface_Btn_Down->managedPath->path)
		->put("entryActivateSound_volume", 0.5f)

		//->put("entriesAlignmentH", List2D::AlignmentH.Undefined)
		//->put("entriesAlignmentV", List2D::AlignmentH.Undefined)
		
		->put("entriesSpacingX", 0.0f)
		->put("entriesSpacingY", 0.0f)
		->put("listPaddingTop", 15.0f, "_dpm.width")
		->put("listPaddingBottom", 15.0f, "_dpm.width")
		->put("listPaddingLeft", 0.0f, "_dpm.width")
		->put("listPaddingRight", 0.0f, "_dpm.width")
		
		//->put("rowsCount", 1)// Horizontal list.
		->put("columnsCount", 1) // Vertical list

		->put("autoListSizeToContent", true)
		//->put("manualListSizeWidth", 144f, "_dpm.width")
		//->put("manualListSizeHeight", 144f, "_dpm.width")

		// Manully set.
		//->put("bgTintColor", Color.valueOf("#FFFFFFFF"))
		//->put("bgAlpha", 1f)
	);
};

LMCPV_List::LMCPV_List(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	listEventListener.cb_onEntryTouched = std::bind(&LMCPV_List::optionsList_onEntryTouched, this, std::placeholders::_1, std::placeholders::_2);
}

void LMCPV_List::optionsList_onEntryTouched(List2D* list, ListEntry2D* listItem) {
	if (!isActive()) {
		return;
	}

	if (list == optionsList) {
		helperCallListEntryActivated(list, listItem);
	} else {
		GlobalAccessLogHandler::devLogHandler->post_debug("Error", "Touch event received for a unknown target.");

		return;
	}
}

void LMCPV_List::helperCallListEntryActivated(List2D* list, ListEntry2D* listEntry) {
	if (listener != nullptr) {
		listener->onListEntryActivated(this, list, listEntry);
	}
}

void LMCPV_List::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	optionsList = getItemMustExistAs< List2D* >("_root.contentArea.optionsList");
	optionsList->setListener(&listEventListener);
}

ListEntry2D* LMCPV_List::createNewListEntry(int index, ArrayList<MenuItemConfig*>* itemInstanceConfigOverride, ArrayList<StateChangeDurations*>* easeDurationsSList, Map1D<std::string, std::shared_ptr<base::IWrappedValue>>* extraDataMap) {
	ListEntry2D* entry = optionsList->createNewEntry(itemInstanceConfigOverride, easeDurationsSList, index, extraDataMap);

	return entry;
}

void LMCPV_List::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	if (viewConfig != nullptr) {
		optionsList->setBgTintColor(viewConfig->bgColor);
	}
}

void LMCPV_List::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		//void
	} else {
		//void
	}
}

LMCPV_List::~LMCPV_List() {
	//void
}
