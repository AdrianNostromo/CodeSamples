#include "ZoneEditorView.h"
#include <base/statics/StaticsInit.h>
#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>

using namespace rpg3D;

ArrayList<MenuItemConfig*>* ZoneEditorView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::DECELERATION)
				->putState("_out_", 0.0f, EaseConfig::ACCELERATION)
			)
		))

		->appendDirect_chain(new_viewItemConfig_topArea())
		/*asd_01; ->appendDirect_chain(new_viewItemConfig_bottomArea())
		->appendDirect_chain(new_viewItemConfig_settingsDrawerA())*/;
});

ArrayList<StateChangeDurations*>* ZoneEditorView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_IN_STATE_CHANGE_DURATION))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_COMPLEX_DURATION_S))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_LEAVE_SCREEN_DURATION_S));
});

ZoneEditorView::ZoneEditorView(
	IApp* app,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewAnimationDurations,
		appUtils
	)
{
	//void
}

/**
 * @param doAnimation
 * @param viewData
 * @param menuAnimationListener
 * @param cinematicController
 * @param playerEntity
 * @param editPos
 */
void ZoneEditorView::show(
	bool doAnimation, ViewDataBase* viewData, IAnimationListener* menuAnimationListener,
	base::IBlockingInputLayer* blockingStreamLayer)
{
	// Set the data before the super call because it will start the animation logic that uses the values already.
	setBlockingStreamLayer(blockingStreamLayer);

	super::show(doAnimation, viewData, menuAnimationListener);
}

ZoneEditorView::~ZoneEditorView() {
	//void
}
