#include "CSV_HistoryList.h"
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_TouchArea.h>

using namespace base;

MenuItemConfig* CSV_HistoryList::new_menuItemConfigs_contentArea_historyArea() {
	return new MenuItemConfig_ComposedVisual("historyArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_local.parent.historyAreaSize.width")
			->putAnimating("dHeight", (new MenuVisualAnimationProperty())
				->putState("_historyActive_", 1.0f, "_local.parent.historyAreaSize.dHeight", EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_historyActive_",
					1.0f, "_local.parent.previewAreaSize.width",
					EaseConfig::LINEAR
				)
				->putState("_",
					1.0f, "_local.parent.previewAreaSize.width",
					VisualValueConfig::OPERATION_SUBTRACT,
					1.0f, "_local.size.width",
					EaseConfig::LINEAR
				)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				// Use float because it will be automatically casted to boolean.
				->putState("_historyActive_", 1.0f, EaseConfig::LINEAR)
				->putState("in", 0.0f, EaseConfig::LINEAR)
				->putState("out", 0.0f, EaseConfig::LINEAR)
			)
		))
	);
		//asd_001; listHistoryColors
}

CSV_HistoryList::CSV_HistoryList(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	//void
}

CSV_HistoryList::~CSV_HistoryList() {
	//void
}
