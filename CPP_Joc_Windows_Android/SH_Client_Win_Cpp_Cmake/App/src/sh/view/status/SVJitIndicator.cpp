#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include "SVJitIndicator.h"
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_MovieClip.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <graphics/visual/BitmapFont.h>
#include <graphics/visual2d/drawable/util/AlignTypeH.h>
#include <graphics/visual2d/drawable/util/AlignTypeV.h>

using namespace sh;

MenuItemConfig* SVJitIndicator::new_viewItemConfig_indicator() {
	return new MenuItemConfig_ComposedVisual("indicator", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 96.0f, "_dpm.width")
			->put("height", 52.0f, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 0.0f)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_jitIndicator_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_jitIndicator_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("taIndicator", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f, "_dpm.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer",
			VCC_Sprite::New("visual2D/taGeneral.atlas", "sh/statusView/jitIndicator/bg"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f, "_dpm.height"
			)
			->put("alpha", 1.0f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("outline", "layer",
			VCC_Sprite::New("visual2D/taGeneral.atlas", "sh/statusView/jitIndicator/outline"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f, "_dpm.height"
			)
			->put("alpha", 1.0f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("iconBg", "layer",
			VCC_Sprite::New("visual2D/taGeneral.atlas", "sh/statusView/jitIndicator/screen/bg"), (new VisualPropertiesConfig())
			->put("width", 40.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f + 16.0f, "_dpm.height"
			)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("okIcon", "layer",
			VCC_Sprite::New("visual2D/taGeneral.atlas", "sh/statusView/jitIndicator/screen/okIcon"), (new VisualPropertiesConfig())
			->put("width", 40.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f + 16.0f, "_dpm.height"
			)
			->put("tintColor", Color("#000000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_status-ok_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_status-ok_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("connectToServerMc_inProgress", "layer",
			VCC_MovieClip::New(
				"visual2D/taGeneral.atlas", "sh/statusView/jitIndicator/screen/connectToServerMc_inProgress_####",
				1.0f / 2.0f, PlayMode::LOOP, true
			), (new VisualPropertiesConfig())
			->put("width", 40.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f + 16.0f, "_dpm.height"
			)
			->put("tintColor", Color("#000000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_status-connectToServer-inProgress_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_status-connectToServer-inProgress_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("connectToServerMc_failed", "layer",
			VCC_MovieClip::New(
				"visual2D/taGeneral.atlas", "sh/statusView/jitIndicator/screen/connectToServerMc_failed_####",
				1.0f / 2.0f, PlayMode::LOOP, true
			), (new VisualPropertiesConfig())
			->put("width", 40.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f + 16.0f, "_dpm.height"
			)
			->put("tintColor", Color("#DC0000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_status-connectToServer-failed_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_status-connectToServer-failed_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("checkInternetMc_inProgress", "layer",
			VCC_MovieClip::New(
				"visual2D/taGeneral.atlas", "sh/statusView/jitIndicator/screen/checkInternetMc_inProgress_####",
				1.0f / 2.0f, PlayMode::LOOP, true
			), (new VisualPropertiesConfig())
			->put("width", 40.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f + 16.0f, "_dpm.height"
			)
			->put("tintColor", Color("#000000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_status-checkInternet-inProgress_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_status-checkInternet-inProgress_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("checkInternetMc_failed", "layer",
			VCC_MovieClip::New(
				"visual2D/taGeneral.atlas", "sh/statusView/jitIndicator/screen/checkInternetMc_failed_####",
				1.0f / 2.0f, PlayMode::LOOP, true
			), (new VisualPropertiesConfig())
			->put("width", 40.0f, "_dpm.width")
			->put("height", 24.0f, "_dpm.height")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.0f)
			->put("x", 0.5f, "parent.props.width")
			->put("y",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-56.0f + 16.0f, "_dpm.height"
			)
			->put("tintColor", Color("#DC0000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_status-checkInternet-failed_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_status-checkInternet-failed_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
	);
};

SVJitIndicator::SVJitIndicator(
    IAppSquareHeads* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
    : super(app, viewItemConfigs, viewEaseDurationsSList)
{
    touchListener.cb_onCursorDown = std::bind(&SVJitIndicator::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool SVJitIndicator::v2d_onCursorDown(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
    // Touch events are ignored while the menu is animating.
    if(!isActive() || isAnimating()) {
        return false;
    }

    if(dynamic_cast<void*>(target) == dynamic_cast<void*>(taIndicator)) {
        toggleConsole();
    }else {
        throw LogicException(LOC);
    }

    return true;
}

void SVJitIndicator::createVisuals(Map1D<std::string, void*>* createConfigMap) {
    super::createVisuals(createConfigMap);

    (taIndicator = getItemOptionalAs<IInteractiveDrawable2D*>("indicator.taIndicator"))->addTouchListener(&touchListener);
}

std::string SVJitIndicator::computeStateFlags(std::string baseState) {
    std::string state = super::computeStateFlags(baseState);

    if(getActiveStatusType() != StatusType::None) {
        state += "_jitIndicator_";
    }

    if(getActiveStatusType() == StatusType::None) {
        state += "_status-none_";
    }else if(getActiveStatusType() == StatusType::General_ok) {
        state += "_status-ok_";
    }else if(getActiveStatusType() == StatusType::ConnectToServer_inProgress) {
        state += "_status-connectToServer-inProgress_";
    }else if(getActiveStatusType() == StatusType::ConnectToServer_error) {
        state += "_status-connectToServer-failed_";
    }else if(getActiveStatusType() == StatusType::CheckInternet_inProgress) {
        state += "_status-checkInternet-inProgress_";
    }else if(getActiveStatusType() == StatusType::CheckInternet_error) {
        state += "_status-checkInternet-failed_";
    }else {
        // Unhandled state.
        throw LogicException(LOC);
    }

    return state;
}

SVJitIndicator::~SVJitIndicator() {
    //void
}
