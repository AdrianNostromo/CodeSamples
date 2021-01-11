#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include "MainSidebarView.h"
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <graphics/visual/Color.h>
#include <base/exceptions/LogicException.h>
#include <functional>
#include <base/statics/StaticsInit.h>

using namespace sh;

ArrayList<MenuItemConfig*>* MainSidebarView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 0.0f)

			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("screenTa", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("x", 16.0f, "_dpm.width")
			->put("width",
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-16.0f, "_dpm.width"
			)
			->put("height", 1.0f, "props.height")
			//The "visible" is manually update in code.
		))

		->appendDirect_chain(new_viewItemConfig_panel())

		->appendDirect_chain(new MenuItemConfig_Visual("screenDimmer", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x",
				1.0f, "panel.layer.x",
				VisualValueConfig::OPERATION_ADD,
				1.0f, "panel.props.width"
			)
			->put("y", 0.0f)
			// Set "width" after "x" because it uses "x" in logic.
			->put("width", 
				1.0f, "props.width",
				VisualValueConfig::OPERATION_ADD,
				-1.0f, "_self.x"
			)
			->put("height", 1.0f, "_appSize.height")
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_._screenDimmer_", AppStyles::SCREEN_DIMMER_ALPHA, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				// Use float because it will be automatically casted to boolean.
				->putState("_in_._screenDimmer_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->put("tintColor", Color("#000000FF"))
		));
});

ArrayList<StateChangeDurations*>* MainSidebarView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_COMPLEX_DURATION_S));
});

MainSidebarView::MainSidebarView(
    IApp* app,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewAnimationDurations, appGame, appUtils)
{
    touchListener.cb_onCursorDown = std::bind(&MainSidebarView::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
    
	menuAnimationListener.cb_onAnimationFinished = std::bind(&MainSidebarView::onAnimationFinished, this, std::placeholders::_1);
}

void MainSidebarView::createContent() {
    super::createContent();

    (screenTa = getItemOptionalAs<IInteractiveDrawable2D*>("screenTa"))->addTouchListener(&touchListener);

    //asdB; soundConfigBgDown = appAssets->getSoundManager()->hookNewSoundItem(&SoundConfig::button_down);
}

void MainSidebarView::onScreenTaTouchDown() {
    super::onScreenTaTouchDown();

	hide(true, &menuAnimationListener);
}

bool MainSidebarView::v2d_onCursorDown(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
    if(!isActive()) {
        return false;
    }

    if(dynamic_cast<void*>(target) == dynamic_cast<void*>(screenTa)) {
        if(soundConfigBgDown != nullptr) {
            // soundConfigBgDown.play();
        }

        onScreenTaTouchDown();
    }

    else {
        //"Unhandled touch target.";

        throw LogicException(LOC);
    }

    return false;
}

void MainSidebarView::onAnimationFinished(
    IGenericDrawable* target)
{
    // Ignore show animation complete events.
    if(isActive()) {
        return ;
    }

    if(outCommandType <= 0) {
        //void
    }else {
        //"Unhandled command type.";

        throw LogicException(LOC);
    }

    outCommandType = COMMAND_NONE;
}

MainSidebarView::~MainSidebarView() {
    //void
}
