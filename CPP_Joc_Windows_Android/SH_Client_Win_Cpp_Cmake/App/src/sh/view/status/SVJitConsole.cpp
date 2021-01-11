#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include "SVJitConsole.h"
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/exceptions/LogicException.h>
#include <base/assets/util/AssetType.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>

using namespace sh;

MenuItemConfig* SVJitConsole::new_viewItemConfig_jitConsoleScreenTa() {
	return new MenuItemConfig_Visual("jitConsoleScreenTa", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
		->put("width", 1.0f, "props.width")
		->put("height", 1.0f, "props.height")
		->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
			->putState("_jitConsole_", 1.0f, EaseConfig::LINEAR)
			->putState("_", 0.0f, EaseConfig::LINEAR)
		)
	);
}

MenuItemConfig* SVJitConsole::new_viewItemConfig_jitConsoleScreenDimmer() {
	return new MenuItemConfig_Visual("jitConsoleScreenDimmer", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
		->put("width", 1.0f, "props.width")
		->put("height", 1.0f, "props.height")
		->put("anchorX", 0.0f)
		->put("anchorY", 0.0f)
		->put("x", 0.0f)
		->put("y", 0.0f)
		->put("alpha", 0.4f)
		->put("tintColor", Color("#000000FF"))
		->putAnimating("alpha", (new MenuVisualAnimationProperty())
			->putState("_jitConsole_", 0.4f, EaseConfig::LINEAR)
			->putState("_", 0.0f, EaseConfig::LINEAR)
		)
		->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
			->putState("_jitConsole_", 1.0f, EaseConfig::LINEAR)
			->putState("_", 0.0f, EaseConfig::LINEAR)
		)
	);
}

MenuItemConfig* SVJitConsole::new_viewItemConfig_console_header() {
	return new MenuItemConfig_ComposedVisual("header", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
			->put("height", 60.0f, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", -1.0f, "props.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("ta", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("alpha", 0.7f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblAsd0", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-32-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 32, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.0f)
			->put("anchorY", 0.5f)
			->put("x", 0.0f, "props.width")
			->put("y", 0.5f, "props.height")
			->put("alpha", 1.0f)
			->put("tintColor", Color("#FFFFFFFF"))
		))

		/*asdA9,
		lblAsd2,
		lblAsd3,
		lblAsd4*/
	);
}

MenuItemConfig* SVJitConsole::new_viewItemConfig_console_body() {
	return new MenuItemConfig_ComposedVisual("body", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("width", 1.0f, "parent.props.width")
			->put("height",
				1.0f, "parent.props.height",
				VisualValueConfig::OPERATION_ADD,
				-60.0f, "_dpm.height"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", -1.0f, "parent.props.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("ta", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "props.width")
			->put("height", 1.0f, "props.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("alpha", 0.4f)
			->put("tintColor", Color("#000000FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblAsd0", "layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-32-_dpFont_"), "Data: ###"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 32, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.5f, "props.width")
			->put("y", 0.5f, "props.height")
			->put("alpha", 1.0f)
			->put("tintColor", Color("#FFFFFFFF"))
		))
	);
}

MenuItemConfig* SVJitConsole::new_viewItemConfig_console() {
	return new MenuItemConfig_ComposedVisual("console", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("screenWidth", 1.0f, "_appSize.width")
			->put("screenHeight", 1.0f, "_appSize.height")

			->put("width",
				1.0f, "_appSize.width",
				VisualValueConfig::OPERATION_ADD,
				-8.0f * 2.0f, "_dpm.width"
			)
			->put("height", 0.5f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 8.0f, "_dpm.width")
			->put("y", 1.0f, "_appSize.height")
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_jitConsole_",
					1.0f, "_appSize.height",
					VisualValueConfig::OPERATION_ADD,
					0.0f, "_dpm.height",// NOTE : This is created as an example.
					EaseConfig::LINEAR)
				->putState("_",
					new VisualValueConfig(
						1.0f, "_appSize.height",
						VisualValueConfig::OPERATION_ADD,
						new VisualValueConfig(50.0f, "_dpm.height")
					),
					EaseConfig::LINEAR
				)
			)

			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_jitConsole_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_jitConsole_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new_viewItemConfig_console_header())
		->appendDirect_chain(new_viewItemConfig_console_body())
	);
}

SVJitConsole::SVJitConsole(
    IAppSquareHeads* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
    : super(app, viewItemConfigs, viewEaseDurationsSList)
{
    touchListener.cb_onCursorDown = std::bind(&SVJitConsole::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool SVJitConsole::v2d_onCursorDown(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
    // Touch events are ignored while the menu is animating.
    if(!isActive() || isAnimating()) {
        return false;
    }

    if(dynamic_cast<void*>(target) == dynamic_cast<void*>(jitConsoleScreenTa)) {
        setIsConsoleActive(false);
    }else if(dynamic_cast<void*>(target) == dynamic_cast<void*>(header_ta)) {
        //void
    }else if(dynamic_cast<void*>(target) == dynamic_cast<void*>(body_ta)) {
        //void
    }else {
        throw LogicException(LOC);
    }

    return true;
}

void SVJitConsole::createVisuals(Map1D<std::string, void*>* createConfigMap) {
    super::createVisuals(createConfigMap);

    (jitConsoleScreenTa = getItemOptionalAs<IInteractiveDrawable2D*>("jitConsoleScreenTa"))->addTouchListener(&touchListener);
    (header_ta = getItemOptionalAs<IInteractiveDrawable2D*>("console.header.ta"))->addTouchListener(&touchListener);
    (body_ta = getItemOptionalAs<IInteractiveDrawable2D*>("console.body.ta"))->addTouchListener(&touchListener);
}

std::string SVJitConsole::computeStateFlags(std::string baseState) {
    std::string state = super::computeStateFlags(baseState);

    if(getIsConsoleActive()) {
        state += "_jitConsole_";
    }

    return state;
}

SVJitConsole::~SVJitConsole() {
    //void
}
