#include "ColorSelectorView.h"
#include <base/statics/StaticsInit.h>
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

using namespace base;

ArrayList<MenuItemConfig*>* ColorSelectorView::viewItemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	viewItemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("size", (new VisualPropertiesConfig())
			->put("width", 1.0f, "_appSize.width")
			->put("height", 1.0f, "_appSize.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			//void
		))
		
		->appendDirect_chain(new MenuItemConfig_Visual("screenDimmerFullScreen", "_local.layer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_local.size.width")
			->put("height", 1.0f, "_local.size.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("tintColor", Color("#000000FF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("in", AppStyles::SCREEN_DIMMER_ALPHA, EaseConfig::LINEAR)
				->putState("out", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaFullScreen", "_local.layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_local.size.width")
			->put("height", 1.0f, "_local.size.height")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new_menuItemConfigs_contentArea());
});

MenuItemConfig* ColorSelectorView::new_menuItemConfigs_contentArea() {
	return new MenuItemConfig_ComposedVisual("contentArea", (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("previewAreaSize", (new VisualPropertiesConfig())
			->put("width", 250.0f, "_dpm.width")
			->put("height", 250.0f, "_dpm.width")
		))
		->appendDirect_chain(new MenuItemConfig_Data("controlsAreaSize", (new VisualPropertiesConfig())
			->put("width", 250.0f, "_dpm.width")
			->putAnimating("dHeight", (new MenuVisualAnimationProperty())
				->putState("_isAlphaEditable_", 160.0f, EaseConfig::LINEAR)
				->putState("_", 130.0f, EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Data("historyAreaSize", (new VisualPropertiesConfig())
			->put("width", 38.0f, "_dpm.width")
			->putAnimating("dHeight", (new MenuVisualAnimationProperty())
				->putState("_", new VisualValueConfig(
					1.0f, "_local.previewAreaSize.height",
					VisualValueConfig::OPERATION_ADD,
					1.0f, "_local.controlsAreaSize.dHeight"
				), EaseConfig::LINEAR
				)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Data("composedSize", (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_historyActive_", new VisualValueConfig(
					1.0f, "_local.previewAreaSize.width",
					VisualValueConfig::OPERATION_ADD,
					1.0f, "_local.historyAreaSize.width"
				), EaseConfig::LINEAR
				)
				->putState("_", 1.0f, "_local.previewAreaSize.width", EaseConfig::LINEAR)
			)
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_",
					1.0f, "_local.previewAreaSize.height",
					VisualValueConfig::OPERATION_ADD,
					1.0f, "_local.controlsAreaSize.dHeight",
					EaseConfig::LINEAR
				)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_out_",
					-1.0f, "_local.composedSize.width",
					EaseConfig::LINEAR
				)
				->putState("_",
					0.5f, "_appSize.width",
					VisualValueConfig::OPERATION_SUBTRACT,
					0.5f, "_local.composedSize.width",
					EaseConfig::LINEAR
				)
			)
			->putAnimating("y", (new MenuVisualAnimationProperty())
				->putState("_",
					0.5f, "_appSize.height",
					VisualValueConfig::OPERATION_SUBTRACT,
					0.5f, "_local.composedSize.height",
					EaseConfig::LINEAR
				)
			)
		))

		->appendDirect_chain(new MenuItemConfig_ComposedVisual("shadowHolder", (new ArrayList<MenuItemConfig*>())
			->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
				->put("width", 1.0f, "_local.parent.composedSize.width")
				->put("height", 1.0f, "_local.parent.composedSize.height")
			))
			->appendDirect_chain(new MenuItemConfig_Data("shadowOffset", (new VisualPropertiesConfig())
				->put("x", 0.0f)
				->put("y", 0.0f)
			))
			->appendDirect_chain(new MenuItemConfig_Visual("layer", "_local.parent.layer", VCC_Group::New(), (new VisualPropertiesConfig())
				->put("x", 0.5f, "_local.parent.composedSize.width")
				->put("y", 0.5f, "_local.parent.composedSize.height")

			))
			->appendDirect_chain(AppStyles::new_ITEM_CONFIG_shadow_DIALOG())
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaContentBg", "_local.layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_", 1.0f, "_local.composedSize.width", EaseConfig::LINEAR)
			)
			->putAnimating("height", (new MenuVisualAnimationProperty())
				->putState("_", 1.0f, "_local.composedSize.height", EaseConfig::LINEAR)
			)
			->put("anchorX", 0.0f)
			->put("anchorY", 0.0f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new_menuItemConfigs_contentArea_historyArea())

		->appendDirect_chain(new_menuItemConfigs_contentArea_previewArea())
		->appendDirect_chain(new_menuItemConfigs_contentArea_controlsArea())
	);
}

ArrayList<StateChangeDurations*>* ColorSelectorView::viewAnimationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	viewAnimationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
	});

ColorSelectorView::ColorSelectorView(IApp* app, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewAnimationDurations, handlerAutoDisposal)
{
	touchListener.cb_onCursorDown = std::bind(&ColorSelectorView::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

bool ColorSelectorView::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	// Touch events are ignored while the menu is animating.
	if (!isActive()) {
		return false;
	}

	if (dynamic_cast<void*>(target) == dynamic_cast<void*>(touchAreaFullScreen)) {
		if (listener != nullptr) {
			listener->onCloseRequested(this);
		}
	} else if (dynamic_cast<void*>(target) == dynamic_cast<void*>(contentArea_touchAreaContentBg)) {
		//void
	} else {
		throw LogicException(LOC);
	}

	return false;
}

void ColorSelectorView::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		//void
	} else {
		//void
	}
}

void ColorSelectorView::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	(touchAreaFullScreen = getItemMustExistAs< IInteractiveDrawable2D*>("touchAreaFullScreen"))->addTouchListener(&touchListener);
	(contentArea_touchAreaContentBg = getItemMustExistAs< IInteractiveDrawable2D*>("contentArea.touchAreaContentBg"))->addTouchListener(&touchListener);
}

ColorSelectorView::~ColorSelectorView() {
	//void
}
