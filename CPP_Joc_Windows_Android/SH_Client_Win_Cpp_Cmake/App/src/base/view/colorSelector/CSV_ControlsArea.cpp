#include "CSV_ControlsArea.h"
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_ComposedVisual.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/menu/util/VisualPropertiesConfig.h>
#include <base/visual2D/util/VisualCreateConfig.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/util/AppStyles.h>
#include <base/config/sounds/SoundConfigsList.h>
#include <base/math/Math.h>
#include <base/menu/util/MenuItemConfig_SmartItem.h>
#include <graphics/visual2d/drawable/Slider2D.h>
#include <base/sensor/hapticFeedback/util/HapticFeedbackType.h>

using namespace base;

MenuItemConfig* CSV_ControlsArea::new_menuItemConfigs_contentArea_controlsArea() {
	return new MenuItemConfig_ComposedVisual("controlsArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_", 1.0f, "_local.parent.controlsAreaSize.width", EaseConfig::LINEAR)
			)
			->putAnimating("dHeight", (new MenuVisualAnimationProperty())
				->putState("_", 1.0f, "_local.parent.controlsAreaSize.dHeight", EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Data("data", (new VisualPropertiesConfig())
			->put("rightPadding", 8.0f, "_dpm.width")
			->put("labelTouchMargin", 8.0f, "_dpm.width")
			->put("interItemsHorizontalSpacing", 8.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->put("y", 0.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "_local.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_local.size.width")
			->put("height", 1.0f, "_local.size.dHeight")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		//            new MenuItemConfig_Visual("hDivider", "_local.layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/divider_horizontal_black"), (new VisualPropertiesConfig())
		//                    ->put("width", 1.0f, "_local.size.width")
		//                    ->put("height", AppStyles::DIVIDER_DARK_HEIGHT_DPM, "_dpm.width")
		//                    ->put("anchorX", 0.5f)
		//                    ->put("anchorY", 0.5f)
		//                    ->put("x", 0.5f, "_local.size.width")
		//                    ->put("y", 1.0f, "_local.size.height")
		//                    ->put("alpha", AppStyles::DIVIDER_DARK_ALPHA)
		//            ),

		->appendDirect_chain(new MenuItemConfig_Visual("layerRGBControls", "_local.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_isAlphaEditable_", 0.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", -50.0f, "_dpm.width", EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layerAControls", "_local.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f, "_dpm.width")
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_isAlphaEditable_", 0.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", -25.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_isAlphaEditable_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_isAlphaEditable_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("flblSliderNameR", "_local.layerRGBControls", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "R"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 22.0f, "_dpm.width")
			->put("y", 221.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("lblSliderValueR", "_local.layerRGBControls", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "000"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "_local.size.width",
				VisualValueConfig::OPERATION_SUBTRACT,
				28.0f, "_dpm.width"
			)
			->put("y", 221.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_ComposedVisual("sliderHolderColorR", (new ArrayList<MenuItemConfig*>())
			->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layerRGBControls", VCC_Group::New(), (new VisualPropertiesConfig())
				->put("x", 49.0f, "_dpm.width")
				->put("y", 221.0f, "_dpm.width")
			))

			->appendDirect_chain(new MenuItemConfig_SmartItem("slider", "_local.layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual * { return new base::Slider2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
				->put("x", 0.0f, "_dpm.width")
				->put("y", 0.0f, "_dpm.width")

				->put("anchorX", 0.0f)
				
				->put("barWidth",
					1.0f, "_local.parent.size.width",
					VisualValueConfig::OPERATION_SUBTRACT,
					49.0f * 2.0f, "_dpm.width"
				)
				->put("movementSpeedMultiplierS", AppStyles::SLIDER_MOVEMENT_SPEED_MULTIPLIER_S)
				->put("knobDiameter_idle", 12.0f, "_dpm.width")
				->put("knobDiameter_active", 18.0f, "_dpm.width")
			))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("flblSliderNameG", "_local.layerRGBControls", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "G"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 22.0f, "_dpm.width")
			->put("y", 173.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("lblSliderValueG", "_local.layerRGBControls", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "000"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "_local.size.width",
				VisualValueConfig::OPERATION_SUBTRACT,
				28.0f, "_dpm.width"
			)
			->put("y", 173.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_ComposedVisual("sliderHolderColorG", (new ArrayList<MenuItemConfig*>())
			->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layerRGBControls", VCC_Group::New(), (new VisualPropertiesConfig())
				->put("x", 49.0f, "_dpm.width")
				->put("y", 173.0f, "_dpm.width")
			))

			->appendDirect_chain(new MenuItemConfig_SmartItem("slider", "_local.layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual* { return new base::Slider2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
				->put("x", 0.0f, "_dpm.width")
				->put("y", 0.0f, "_dpm.width")

				->put("anchorX", 0.0f)

				->put("barWidth",
					1.0f, "_local.parent.size.width",
					VisualValueConfig::OPERATION_SUBTRACT,
					49.0f * 2.0f, "_dpm.width"
				)
				->put("movementSpeedMultiplierS", AppStyles::SLIDER_MOVEMENT_SPEED_MULTIPLIER_S)
				->put("knobDiameter_idle", 12.0f, "_dpm.width")
				->put("knobDiameter_active", 18.0f, "_dpm.width")
			))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblSliderNameB", "_local.layerRGBControls", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "B"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 22.0f, "_dpm.width")
			->put("y", 125.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("lblSliderValueB", "_local.layerRGBControls", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "000"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "_local.size.width",
				VisualValueConfig::OPERATION_SUBTRACT,
				28.0f, "_dpm.width"
			)
			->put("y", 125.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_ComposedVisual("sliderHolderColorB", (new ArrayList<MenuItemConfig*>())
			->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layerRGBControls", VCC_Group::New(), (new VisualPropertiesConfig())
				->put("x", 49.0f, "_dpm.width")
				->put("y", 125.0f, "_dpm.width")
			))

			->appendDirect_chain(new MenuItemConfig_SmartItem("slider", "_local.layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual* { return new base::Slider2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
				->put("x", 0.0f, "_dpm.width")
				->put("y", 0.0f, "_dpm.width")

				->put("anchorX", 0.0f)

				->put("barWidth",
					1.0f, "_local.parent.size.width",
					VisualValueConfig::OPERATION_SUBTRACT,
					49.0f * 2.0f, "_dpm.width"
				)
				->put("movementSpeedMultiplierS", AppStyles::SLIDER_MOVEMENT_SPEED_MULTIPLIER_S)
				->put("knobDiameter_idle", 12.0f, "_dpm.width")
				->put("knobDiameter_active", 18.0f, "_dpm.width")
			))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("flblSliderNameA", "_local.layerAControls", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "A"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 22.0f, "_dpm.width")
			->put("y", 77.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("lblSliderValueA", "_local.layerAControls", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "000"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "_local.size.width",
				VisualValueConfig::OPERATION_SUBTRACT,
				28.0f, "_dpm.width"
			)
			->put("y", 77.0f, "_dpm.width")
			->put("tintColor", Color("#000000FF"))
			->put("alpha", AppStyles::DARK_PRIMARY_TEXT_ALPHA)
		))
		->appendDirect_chain(new MenuItemConfig_ComposedVisual("sliderHolderColorA", (new ArrayList<MenuItemConfig*>())
			->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layerAControls", VCC_Group::New(), (new VisualPropertiesConfig())
				->put("x", 49.0f, "_dpm.width")
				->put("y", 77.0f, "_dpm.width")
			))

			->appendDirect_chain(new MenuItemConfig_SmartItem("slider", "_local.layer", "_in_", [](IAppMetrics* appMetrics, IAppAssets* appAssets) -> ComposedStiledVisual* { return new base::Slider2D(appMetrics, appAssets); }, (new VisualPropertiesConfig())
				->put("x", 0.0f, "_dpm.width")
				->put("y", 0.0f, "_dpm.width")

				->put("anchorX", 0.0f)

				->put("barWidth",
					1.0f, "_local.parent.size.width",
					VisualValueConfig::OPERATION_SUBTRACT,
					49.0f * 2.0f, "_dpm.width"
				)
				->put("movementSpeedMultiplierS", AppStyles::SLIDER_MOVEMENT_SPEED_MULTIPLIER_S)
				->put("knobDiameter_idle", 12.0f, "_dpm.width")
				->put("knobDiameter_active", 18.0f, "_dpm.width")
			))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblBtnA", "_local.layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "UPDATE"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
            /*->put("x",
                    1.0f, "_local.size.width",
                    VisualValueConfig::OPERATION_SUBTRACT,
                    51f, "_dpm.width"
            )*/
			->put("y", 33.0f, "_dpm.width")
			->put("tintColor", Color("#FFFFFFFF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("taBtnA", "_local.layer", VCC_TouchArea::New(&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f), (new VisualPropertiesConfig())
			// x is dynamicly updated.
			// ->put("width", 74.0f, "_dpm.width")
			->put("height", 30.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
            /*->put("x",
                    1.0f, "_local.size.width",
                    VisualValueConfig::OPERATION_SUBTRACT,
                    51.0f, "_dpm.width"
            )*/
			->put("y", 33.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblBtnB", "_local.layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "CANCEL"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
            /*->put("x",
                    1.0f, "_local.size.width",
                    VisualValueConfig::OPERATION_SUBTRACT,
                    126f, "_dpm.width"
            )*/
			->put("y", 33.0f, "_dpm.width")
			->put("tintColor", Color("#FFFFFFFF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("taBtnB", "_local.layer", VCC_TouchArea::New(&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f), (new VisualPropertiesConfig())
			// x is dynamicly updated.
			// ->put("width", 74.0f, "_dpm.width")
			->put("height", 30.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
            /*->put("x",
                    1.0f, "_local.size.width",
                    VisualValueConfig::OPERATION_SUBTRACT,
                    126.0f, "_dpm.width"
            )*/
			->put("y", 33.0f, "_dpm.width")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("lblBtnC", "_local.layer", VCC_BitmapLabel::New(
			ManagedAssetPath::stringPathToManagedFontPath("fonts/Roboto-Medium-AaNSS-14-_dpFont_"), "DELETE"
		), (new VisualPropertiesConfig())
			->put("fontName", "fonts/Roboto")
			->put("fontWeight", VCC_BitmapLabel::FontWeight::Medium)
			->put("fontSize", 14, "_dpm_font")
			->put("charactersRange", "AaNSS")

			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
            /*->put("x",
                    1.0f, "_local.size.width",
                    VisualValueConfig::OPERATION_SUBTRACT,
                    200.0f, "_dpm.width"
            )*/
			->put("y", 33.0f, "_dpm.width")
			->put("tintColor", Color("#FFFFFFFF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("taBtnC", "_local.layer", VCC_TouchArea::New(&SoundConfigsList::UserInterface_Btn_Down->managedPath->path, 0.5f, base::HapticFeedbackType::NONE, nullptr, 0.5f), (new VisualPropertiesConfig())
			// x is dynamicly updated.
			// ->put("width", 74.0f, "_dpm.width")
			->put("height", 30.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			// x is dynamicly updated.
            /*->put("x",
                    1.0f, "_local.size.width",
                    VisualValueConfig::OPERATION_SUBTRACT,
                    200.0f, "_dpm.width"
            )*/
			->put("y", 33.0f, "_dpm.width")
		))
	);
}

CSV_ControlsArea::CSV_ControlsArea(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
	touchListener.cb_onCursorDown = std::bind(&CSV_ControlsArea::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	sliderEventListener.cb_onValueChanged = std::bind(&CSV_ControlsArea::slider_onValueChanged, this, std::placeholders::_1);
}

bool CSV_ControlsArea::v2d_onCursorDown(
	IInteractiveDrawable2D * target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(taBtnA)
		|| dynamic_cast<void*>(target) == dynamic_cast<void*>(taBtnB)
		|| dynamic_cast<void*>(target) == dynamic_cast<void*>(taBtnC))
	{
		DynamicTextButtonConfig* dynamicButtonConfig = target->getUserDataMap()->getDirectB("dynamicButtonConfig")->getDirectAs_Custom< DynamicTextButtonConfig*>(-1);
		handleTouchRequest_dynamicButtonActivated(dynamicButtonConfig, *getActiveColor());
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void CSV_ControlsArea::slider_onValueChanged(ISlider2D* target) {
	if (target == colorSliderR || target == colorSliderG || target == colorSliderB || target == colorSliderA) {
		float r = colorSliderR->getRangeValue();
		float g = colorSliderG->getRangeValue();
		float b = colorSliderB->getRangeValue();
		float a;

		if (getIsAlphaEditable()) {
			a = colorSliderA->getRangeValue();
		} else {
			a = 1.0f;
		}

		Color col(r, g, b, a);
		setActiveColor(col, false, false);
	} else {
		throw LogicException(LOC);
	}
}

void CSV_ControlsArea::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	taBtnA = getItemMustExistAs< IInteractiveDrawable2D*>("_root.contentArea.controlsArea.taBtnA");
	taBtnA->addTouchListener(&touchListener);

	taBtnB = getItemMustExistAs< IInteractiveDrawable2D*>("_root.contentArea.controlsArea.taBtnB");
	taBtnB->addTouchListener(&touchListener);

	taBtnC = getItemMustExistAs< IInteractiveDrawable2D*>("_root.contentArea.controlsArea.taBtnC");
	taBtnC->addTouchListener(&touchListener);

	lblBtnA = getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.controlsArea.lblBtnA");
	lblBtnB = getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.controlsArea.lblBtnB");
	lblBtnC = getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.controlsArea.lblBtnC");

	colorSliderR = getItemMustExistAs<base::ISlider2D*>("_root.contentArea.controlsArea.sliderHolderColorR.slider");
	colorSliderR->setEventListener(&sliderEventListener);

	colorSliderG = getItemMustExistAs<base::ISlider2D*>("_root.contentArea.controlsArea.sliderHolderColorG.slider");
	colorSliderG->setEventListener(&sliderEventListener);

	colorSliderB = getItemMustExistAs<base::ISlider2D*>("_root.contentArea.controlsArea.sliderHolderColorB.slider");
	colorSliderB->setEventListener(&sliderEventListener);

	colorSliderA = getItemMustExistAs<base::ISlider2D*>("_root.contentArea.controlsArea.sliderHolderColorA.slider");
	colorSliderA->setEventListener(&sliderEventListener);

	lblSliderValueR = getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.controlsArea.lblSliderValueR");
	lblSliderValueG = getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.controlsArea.lblSliderValueG");
	lblSliderValueB = getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.controlsArea.lblSliderValueB");
	lblSliderValueA = getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.controlsArea.lblSliderValueA");
}

void CSV_ControlsArea::setActiveColor(Color& activeColor, bool updateSliders, bool animateSliders) {
	super::setActiveColor(activeColor, updateSliders, animateSliders);

	if (updateSliders) {
		// Update the sliders.
		colorSliderR->setRangeValue(activeColor.r, animateSliders, false);
		colorSliderG->setRangeValue(activeColor.g, animateSliders, false);
		colorSliderB->setRangeValue(activeColor.b, animateSliders, false);
		colorSliderA->setRangeValue(activeColor.a, animateSliders, false);
	}

	// Update the slider labels.
	int r = Math::round(0xff * activeColor.r);
	int g = Math::round(0xff * activeColor.g);
	int b = Math::round(0xff * activeColor.b);
	int a = Math::round(0xff * activeColor.a);

	lblSliderValueR->setTextDirect(std::to_string(r));
	lblSliderValueG->setTextDirect(std::to_string(g));
	lblSliderValueB->setTextDirect(std::to_string(b));
	lblSliderValueA->setTextDirect(std::to_string(a));
}

void CSV_ControlsArea::setIsAlphaEditable(bool isAlphaEditable, int alphaOverrideHexValue, bool updateShowState) {
	super::setIsAlphaEditable(isAlphaEditable, alphaOverrideHexValue, updateShowState);

	if (!isAlphaEditable) {
		Color col{ *getActiveColor() };

		if (col.a != 1.0f) {
			col.a = 1.0f;

			setActiveColor(col, true, false);
		}
	}
}

void CSV_ControlsArea::syncVisuals(bool syncAbsoluteValues, bool syncAnimatingValues, std::string startStateID, std::string endStateID, float stateCompletionPercent) {
	super::syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

	syncDynamicContent();
}

void CSV_ControlsArea::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		//void
	} else {
		syncDynamicContent();
	}
}

void CSV_ControlsArea::syncDynamicContent() {
	float rightPadding = getItemPropertyValue("contentArea.controlsArea.data.rightPadding", nullptr)->getDirectAs_float();
	float interItemsHorizontalSpacing = getItemPropertyValue("contentArea.controlsArea.data.interItemsHorizontalSpacing", nullptr)->getDirectAs_float();

	float controlsAreaWidth = getItemPropertyValue("contentArea.controlsAreaSize.width", nullptr)->getDirectAs_float();
	float rightWallPosX = controlsAreaWidth - rightPadding;

	const int cuntrolButtonLblsCount = 3;
	base::BitmapLabel* cuntrolButtonLbls[cuntrolButtonLblsCount] {
		lblBtnA,
		lblBtnB,
		lblBtnC
	};
	IInteractiveDrawable2D* cuntrolButtonTouchAreas[] {
		taBtnA,
		taBtnB,
		taBtnC
	};
	int btnIndex = 0;
	if (dynamicButtonConfigs != nullptr) {
		for (int i = 0; i < dynamicButtonConfigs->count(); i++) {
			DynamicTextButtonConfig* dBtnConfig = dynamicButtonConfigs->getPointer(i);

			base::BitmapLabel* btnLbl = cuntrolButtonLbls[btnIndex];
			IInteractiveDrawable2D* btnTA = cuntrolButtonTouchAreas[btnIndex];

			if (btnIndex > 0) {
				rightWallPosX -= interItemsHorizontalSpacing;
			}
			rightWallPosX = syncDynamicBtn(dBtnConfig, btnTA, btnLbl, rightWallPosX);

			btnIndex++;
		}
	}
	for (int i = btnIndex; i < cuntrolButtonLblsCount; i++) {
		base::BitmapLabel* btnLbl = cuntrolButtonLbls[i];
		IInteractiveDrawable2D* btnTA = cuntrolButtonTouchAreas[i];

		btnLbl->setIsVisible(false);
		btnTA->setIsVisible(false);
	}
}

float CSV_ControlsArea::syncDynamicBtn(DynamicTextButtonConfig* dynamicButtonConfig, IInteractiveDrawable2D* taBtn, base::BitmapLabel* lblBtn, float rightWallPosX) {
	if (dynamicButtonConfig == nullptr) {
		throw LogicException(LOC);
	}

	taBtn->getUserDataMap()->putDirectB("dynamicButtonConfig", base::IWrappedValue::new_Custom(dynamicButtonConfig, -1));

	taBtn->setIsVisible(true);
	lblBtn->setIsVisible(true);

	if (lblBtn->getText() != dynamicButtonConfig->text) {
		lblBtn->setTextDirect(dynamicButtonConfig->text);
		// This is reuired for the below access of the width property.
		lblBtn->validateGlyphsAndMetricsIfNeeded();
	}

	float labelTouchMargin = getItemPropertyValue("contentArea.controlsArea.data.labelTouchMargin", nullptr)->getDirectAs_float();

	float touchAreaWidth = lblBtn->getWidth() + labelTouchMargin * 2.0f;
	float posX = rightWallPosX - touchAreaWidth / 2.0f;

	taBtn->setWidth(touchAreaWidth);
	taBtn->setX(posX);

	lblBtn->setTintColor(dynamicButtonConfig->tintColor);
	lblBtn->setAlpha(dynamicButtonConfig->alpha);
	lblBtn->setX(posX);

	return posX - touchAreaWidth / 2.0f;
}

void CSV_ControlsArea::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	if (isDirty_controls) {
		isDirty_controls = false;

		syncDynamicContent();
	}
}

void CSV_ControlsArea::updateControlLblText(int controlIndex, std::string newText) {
	DynamicTextButtonConfig* dBtnConfig = dynamicButtonConfigs->getPointer(controlIndex);
	dBtnConfig->text = newText;
	
	// Invalidate the controls list.
	isDirty_controls = true;
}

base::BitmapLabel* CSV_ControlsArea::getControlLbl(int controlIndex) {
	static std::string controlsList[] {
		"lblBtnA",
		"lblBtnB",
		"lblBtnC"
	};

	base::BitmapLabel* lbl = getItemMustExistAs< base::BitmapLabel*>("_root.contentArea.controlsArea." + controlsList[controlIndex] );

	return lbl;
}

void CSV_ControlsArea::handleTouchRequest_dynamicButtonActivated(DynamicTextButtonConfig* dynamicButtonConfig, Color& activeColor) {
	if (listener != nullptr) {
		listener->onDynamicButtonActivated(this, dynamicButtonConfig, activeColor);
	}
}

CSV_ControlsArea::~CSV_ControlsArea() {
	//void
}
