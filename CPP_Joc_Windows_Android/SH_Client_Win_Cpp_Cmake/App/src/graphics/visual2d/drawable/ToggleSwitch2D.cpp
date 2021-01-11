#include "ToggleSwitch2D.h"
#include <base/statics/StaticsInit.h>
#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>

using namespace base;

ArrayList<MenuItemConfig*>* ToggleSwitch2D::itemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	itemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("knobPosX_off", -8.0f, "_dpm.width")
			->put("knobPosX_on", 8.0f, "_dpm.width")

			// This is used for anchor position offsets.
			->put("computedComponent_width", 36.0f, "_dpm.width")
			->put("computedComponent_height", 20.0f, "_dpm.height")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x",
				0.5f, "props.computedComponent_width",
				VisualValueConfig::OPERATION_ADD,
				new VisualValueConfig(
					-1.0f, "props.computedComponent_width", 
					VisualValueConfig::OPERATION_MULTIPLY,
					1.0f, "_root.anchorX"
				)
			)
			->put("y",
				0.5f, "props.computedComponent_height",
				VisualValueConfig::OPERATION_ADD,
				new VisualValueConfig(
					-1.0f, "props.computedComponent_height",
					VisualValueConfig::OPERATION_MULTIPLY,
					1.0f, "_root.anchorY"
				)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaBack", "layer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width", 50.0f, "_dpm.width")
			->put("height", 32.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/toggleSwitch/bg"), (new VisualPropertiesConfig())
			->put("width", 36.0f, "_dpm.width")
			->put("height", 14.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->putAnimating("tintColor", (new MenuVisualAnimationProperty())
					->putState("_disabled_", Color("#D5D5D5FF"), EaseConfig::LINEAR)
					->putState("_", Color("#D5D5D5FF"), EaseConfig::LINEAR)//_enabled_
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("coloredBg", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/toggleSwitch/bg"), (new VisualPropertiesConfig())
			->putAnimating("width", (new MenuVisualAnimationProperty())
				->putState("_on_", 36.0f, "_dpm.width", EaseConfig::LINEAR)
				->putState("_", 0.0f, "_dpm.width", EaseConfig::LINEAR)
			)
			->put("height", 14.0f, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.5f)
			->put("x", -0.5f * 36.0f, "_dpm.width")
			->put("y", 0.0f)
			->put("tintColor", Color("#2196F3FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("knobShadowDisabled", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/toggleSwitch/knobShadowDisabled"), (new VisualPropertiesConfig())
			->put("width", 28.0f, "_dpm.width")
			->put("height", 28.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_on_", 1.0f, "props.knobPosX_on", EaseConfig::LINEAR)
				->putState("_off_", 1.0f, "props.knobPosX_off", EaseConfig::LINEAR)
			)
			->put("y", 0.0f, "_dpm.width")
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_disabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_disabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("knobShadow_enabled", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/toggleSwitch/knobShadow_enabled"), (new VisualPropertiesConfig())
			->put("width", 28.0f, "_dpm.width")
			->put("height", 28.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_on_", 1.0f, "props.knobPosX_on", EaseConfig::LINEAR)
				->putState("_off_", 1.0f, "props.knobPosX_off", EaseConfig::LINEAR)
			)
			->put("y", 0.0f, "_dpm.width")
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_enabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_enabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("knob_disabled", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/toggleSwitch/knob"), (new VisualPropertiesConfig())
			->put("width", 28.0f, "_dpm.width")
			->put("height", 28.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_on_", 1.0f, "props.knobPosX_on", EaseConfig::LINEAR)
				->putState("_off_", 1.0f, "props.knobPosX_off", EaseConfig::LINEAR)
			)
			->put("y", 0.0f, "_dpm.width")
			->put("tintColor", Color("#BDBDBDFF"))
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_disabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_disabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		))
		->appendDirect_chain(new MenuItemConfig_Visual("knob_enabled", "layer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/toggleSwitch/knob"), (new VisualPropertiesConfig())
			->put("width", 28.0f, "_dpm.width")
			->put("height", 28.0f, "_dpm.width")
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->putAnimating("x", (new MenuVisualAnimationProperty())
				->putState("_on_", 1.0f, "props.knobPosX_on", EaseConfig::LINEAR)
				->putState("_off_", 1.0f, "props.knobPosX_off", EaseConfig::LINEAR)
			)
			->put("y", 0.0f, "_dpm.width")
			->putAnimating("alpha", (new MenuVisualAnimationProperty())
				->putState("_enabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
			->putAnimating("_visibleFloat", (new MenuVisualAnimationProperty())
				->putState("_enabled_", 1.0f, EaseConfig::LINEAR)
				->putState("_", 0.0f, EaseConfig::LINEAR)
			)
		));
});

ArrayList<StateChangeDurations*>* ToggleSwitch2D::animationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	animationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
	});

ToggleSwitch2D::ToggleSwitch2D(IAppMetrics* appMetrics, IAppAssets* appAssets)
	: super(appMetrics, appAssets, itemConfigs, animationDurations)
{
	//void
}

void ToggleSwitch2D::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	bool doSyncVisuals = false;

	if (isDirty_anchor) {
		isDirty_anchor = false;

		doSyncVisuals = true;
	}

	if (doSyncVisuals) {
		syncVisuals(false, true, startStateID, endStateID, stateCompletionPercent);
	}
}

float ToggleSwitch2D::getAnchorX() {
	return anchorX;
}

float ToggleSwitch2D::getAnchorY() {
	return anchorY;
}

void ToggleSwitch2D::setAnchor(float anchorX, float anchorY) {
	// This function is called only when the toggle switch is manually created by code.
	// In that case, need to let the template logic know the values where initialised.
	// Note. make sure to call before reservedCreate is called. Don't call here because they need to be set before the reservedCreate.
	// putStaticAnimatingPropertyStatusDirect("anchorX", true);
	// putStaticAnimatingPropertyStatusDirect("anchorY", true);

	if (this->anchorX == anchorX && this->anchorY == anchorY) {
		return;
	}

	this->anchorX = anchorX;
	this->anchorY = anchorY;

	isDirty_anchor = true;
}

std::shared_ptr<base::IWrappedValue> ToggleSwitch2D::getPropertyValue(std::string property) {
	if (property == "anchorX") {
		return base::IWrappedValue::new_float(anchorX);
	}else if (property == "anchorY") {
		return base::IWrappedValue::new_float(anchorY);
	}

	return super::getPropertyValue(property);
}

void ToggleSwitch2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
	if (property == "anchorX") {
		anchorX = value->getDirectAs_float();
		isDirty_anchor = true;
	}else if (property == "anchorY") {
		anchorY = value->getDirectAs_float();
		isDirty_anchor = true;
	}

	else {
		super::setPropertyValue(property, value);
	}
}

ToggleSwitch2D::~ToggleSwitch2D() {
	//void
}
