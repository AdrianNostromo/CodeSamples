#include "MainInGameUIView.h"
#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/exceptions/LogicException.h>
#include <functional>
#include <base/statics/StaticsInit.h>
#include <base/sensor/hapticFeedback/util/HapticFeedbackType.h>
#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>
#include <rpg3D/config/sounds/SoundConfigsList.h>
#include <base/visual2D/util/VCC_SpriteButton.h>

using namespace rpg3D;

ArrayList<MenuItemConfig*>* MainInGameUIView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_in_", 1.0f, EaseConfig::ACCELERATION)
				->putState("_out_", 0.0f, EaseConfig::ACCELERATION)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layerTopBar", "layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_fullMode_", 0.0f, "_dpm.height", EaseConfig::ACCELERATION)
				->putState("_editorMode_", -56.0f, "_dpm.height", EaseConfig::DECELERATION)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("pixBg", "layerTopBar", VCC_Sprite::New(
			"visual2D/taUtilLinear.atlas",
			"bg/simple_white_FFFFFF"
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			//->put("x", 24.0f + 4.0f, "_dpm.width")
			->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("y",
				1.0f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 24.0f, "_dpm.height"
			)
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("pixIcon", "layerTopBar", VCC_SpriteButton::New(
			"visual2D/taGeneral.atlas", "solarOne/gw/mainUiView/iconPix",
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f,
			nullptr, 0.0f,
			nullptr, 0.0f
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("touchWidthOverride", std::make_shared<float>(48.0f), "_dpm.width")
			->put("touchHeightOverride", std::make_shared<float>(48.0f), "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			//->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("x", 4.0f + 24.0f, "_dpm.width")
			->put("y",
				1.0f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 24.0f, "_dpm.height"
			)
			->put("alpha", 0.6f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bgControlModeToggle", "layerTopBar", VCC_SpriteButton::New(
			"visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF",
			&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f,
			nullptr, 0.0f,
			nullptr, 0.0f
		), (new VisualPropertiesConfig())
			->put("width", 48.0f, "_dpm.width")
			->put("height", 48.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "size.width",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 24.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 24.0f, "_dpm.height"
			)
			->put("alpha", 0.2f)
			->put("tintColor", Color("#000000FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("iconControlMode_firstPerson", "layerTopBar", VCC_Sprite::New(
			"visual2D/taGeneral.atlas", "solarOne/gw/mainUiView/iconCameraMode_firstPerson"
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "size.width",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 24.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 24.0f, "_dpm.height"
			)
			->put("tintColor", Color("#FFFFFFFF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_firstPersonCameraMode_", 0.6f, EaseConfig::ACCELERATION)
				->putState("_", 0.0f, EaseConfig::ACCELERATION)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_firstPersonCameraMode_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("iconControlMode_thirdPerson", "layerTopBar", VCC_Sprite::New(
			"visual2D/taGeneral.atlas", "solarOne/gw/mainUiView/iconCameraMode_thirdPerson"
		), (new VisualPropertiesConfig())
			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "size.width",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 24.0f, "_dpm.width"
			)
			->put("y",
				1.0f, "size.height",
				VisualValueConfig::OPERATION_ADD,
				-4.0f - 24.0f, "_dpm.height"
			)
			->put("tintColor", Color("#FFFFFFFF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_thirdPersonCameraMode_", 0.6f, EaseConfig::ACCELERATION)
				->putState("_", 0.0f, EaseConfig::ACCELERATION)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_thirdPersonCameraMode_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new_viewItemConfig_activeTool())

		->appendDirect_chain(new_viewItemConfig_dPad())
		
		->appendDirect_chain(new MenuItemConfig_Visual("crosshair", "layer", VCC_Sprite::New(
			"visual2D/taGeneral.atlas", "rpg3D/gw/mainUi/crosshairA_v1"
		), (new VisualPropertiesConfig())
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isCrosshair_", 0.8f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isCrosshair_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("rotationD", (new MenuVisualAnimationProperty())
				->putState("_isCrosshair_", 0.0f, EaseConfig::LINEAR)
				->putState("_", 90.0f, EaseConfig::LINEAR)
			)

			->put("width", 24.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "size.width")
			->put("y", 0.5f, "size.height")
			->put("tintColor", Color("#FFFFFFFF"))
		));
});

ArrayList<StateChangeDurations*>* MainInGameUIView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
});

MainInGameUIView::MainInGameUIView(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
    : super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
    touchListener.cb_onCursorDown = std::bind(&MainInGameUIView::onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

MainInGameUIView::MainInGameUIView(IApp* app, base::IGameWorld* gw)
    : MainInGameUIView(app, viewItemConfigs, viewAnimationDurations, gw)
{
    //void
}

void MainInGameUIView::createContent() {
    super::createContent();

    (pixIcon = getItemOptionalAs<IInteractiveDrawable2D*>("pixIcon"))->addTouchListener(&touchListener);
    (bgControlModeToggle = getItemOptionalAs<IInteractiveDrawable2D*>("bgControlModeToggle"))->addTouchListener(&touchListener);
}

bool MainInGameUIView::onCursorDown(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
    if(!isActive()) {
        return false;
    }

    if(dynamic_cast<void*>(target) == dynamic_cast<void*>(pixIcon)) {
        // Show the side panel.
        handler->onRequestShowMainSidebarView();
    }else if(dynamic_cast<void*>(target) == dynamic_cast<void*>(bgControlModeToggle)) {
		handler->onRequestToggleCameraMode();
    }

    else {
        //"Unhandled touch target.";

        throw LogicException(LOC);
    }

    return false;
}

MainInGameUIView::~MainInGameUIView() {
    //void
}
