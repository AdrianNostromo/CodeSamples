#include "Slider2D.h"
#include <base/statics/StaticsInit.h>
#include <base/util/AppStyles.h>
#include <base/menu/util/MenuItemConfig_Data.h>
#include <base/menu/util/MenuItemConfig_Visual.h>
#include <base/visual2D/util/VCC_Group.h>
#include <base/visual2D/util/VCC_Sprite.h>
#include <base/visual2D/util/VCC_TouchArea.h>
#include <base/math/Math.h>
#include <graphics/visual2d/drawable/Sprite2D.h>
#include <base/visual2D/VisualUtil2D.h>
#include <base/app/IAppAssets.h>

using namespace base;

ArrayList<MenuItemConfig*>* Slider2D::itemConfigs = base::StaticsInit::AddCbGeneral<ArrayList<MenuItemConfig*>*>(1, []() {
	itemConfigs = (new ArrayList<MenuItemConfig*>())
		->appendDirect_chain(new MenuItemConfig_Data("props", (new VisualPropertiesConfig())
			->put("visualUpdateValue", 1.0f, "_dataMap.visualUpdateValue")
			->put("slideFocusStatePercent", 1.0f, "_dataMap.slideFocusStatePercent")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("layer", "_root.visualsHolder", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x", 0.0f)
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("anchorLayer", "layer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("x",
				-1.0f, "_root.barWidth",
				VisualValueConfig::OPERATION_MULTIPLY,
				1.0f, "_root.anchorX"
			)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("touchAreaBack", "anchorLayer", VCC_TouchArea::New(), (new VisualPropertiesConfig())
			->put("width",
				// This slider has a "barWidth" manual property that is on the yMax level.
				1.0f, "_root.barWidth",
				VisualValueConfig::OPERATION_ADD,
				10.0f, "_dpm.width"
			)
			->put("height", 46.0f, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.5f)
			->put("x", -5.0f, "_dpm.width")
			->put("y", 0.0f)
		))

		->appendDirect_chain(new MenuItemConfig_Visual("bg_bar_base", "anchorLayer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			->put("width", 1.0f, "_root.barWidth")
			->put("height", 2.0f, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.5f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("tintColor", Color("#B9B9B9FF"))
		))
		->appendDirect_chain(new MenuItemConfig_Visual("bg_bar_fill", "anchorLayer", VCC_Sprite::New("visual2D/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), (new VisualPropertiesConfig())
			// width is dynamicly handled.
			->put("width",
				1.0f, "_root.barWidth",
				VisualValueConfig::OPERATION_MULTIPLY,
				1.0f, "props.visualUpdateValue"
			)
			->put("height", 2.0f, "_dpm.width")
			->put("anchorX", 0.0f)
			->put("anchorY", 0.5f)
			->put("x", 0.0f)
			->put("y", 0.0f)
			->put("tintColor", Color("#2196F3FF"))
		))

		->appendDirect_chain(new MenuItemConfig_Visual("tickMarksLayer", "anchorLayer", VCC_Group::New(), (new VisualPropertiesConfig())
			->put("alpha", 1.0f, "props.slideFocusStatePercent")
			->put("_visibleFloat", 1.0f, "props.slideFocusStatePercent")
		))

		->appendDirect_chain(new MenuItemConfig_Visual("knob", "anchorLayer", VCC_Sprite::New("visual2D/taGeneral.atlas", "general/slider/knob"), (new VisualPropertiesConfig())
			//knobDiameter_idle + (knobDiameter_active - knobDiameter_idle) * slideFocusStatePercent;
			->put("width",
				1.0f, "_root.knobDiameter_idle",
				VisualValueConfig::OPERATION_ADD,
				new VisualValueConfig(
					1.0f, "props.slideFocusStatePercent",
					VisualValueConfig::OPERATION_MULTIPLY,
					new VisualValueConfig(
						1.0f, "_root.knobDiameter_active",
						VisualValueConfig::OPERATION_SUBTRACT,
						1.0f, "_root.knobDiameter_idle"
					)
				)
			)
			->put("height",
				1.0f, "_root.knobDiameter_idle",
				VisualValueConfig::OPERATION_ADD,
				new VisualValueConfig(
					1.0f, "props.slideFocusStatePercent",
					VisualValueConfig::OPERATION_MULTIPLY,
					new VisualValueConfig(
						1.0f, "_root.knobDiameter_active",
						VisualValueConfig::OPERATION_SUBTRACT,
						1.0f, "_root.knobDiameter_idle"
					)
				)
			)
			->put("anchorX", 0.5f)
			->put("anchorY", 0.5f)
			->put("x",
				1.0f, "_root.barWidth",
				VisualValueConfig::OPERATION_MULTIPLY,
				1.0f, "props.visualUpdateValue"
			)
			->put("y", 0.0f)
			->put("tintColor", nullptr, "_dataMap.knobColor")
		));
});

ArrayList<StateChangeDurations*>* Slider2D::animationDurations = base::StaticsInit::AddCbGeneral<ArrayList<StateChangeDurations*>*>(1, []() {
	animationDurations = (new ArrayList<StateChangeDurations*>())
		->appendDirect_chain(new StateChangeDurations("_in_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_out_", "_in_", AppStyles::ANIMATION_DURATION_S_DEFAULT))
		->appendDirect_chain(new StateChangeDurations("_in_", "_out_", AppStyles::ANIMATION_DURATION_S_DEFAULT));
	});

Slider2D::Slider2D(IAppMetrics* appMetrics, IAppAssets* appAssets)
	: super(appMetrics, appAssets, itemConfigs, animationDurations)
{
	touchListener.cb_onCursorDown = std::bind(&Slider2D::v2d_onCursorDown, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	touchListener.cb_onCursorDrag = std::bind(&Slider2D::v2d_onCursorDrag, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
	touchListener.cb_onCursorUp = std::bind(&Slider2D::v2d_onCursorUp, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
	touchListener.cb_onCursorUpOutside = std::bind(&Slider2D::v2d_onCursorUpOutside, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

	putDataMapValueB("visualUpdateValue", base::IWrappedValue::new_float(visualUpdateValue), false);
	putDataMapValueB("slideFocusStatePercent", base::IWrappedValue::new_float(slideFocusStatePercent), false);
	Color col("#2196F3FF");
	putDataMapValueB("knobColor", base::IWrappedValue::new_Color(col), false);
}

bool Slider2D::v2d_onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (target == touchAreaBack) {
		touchDownIndex = cursorIndex;
		isTouchSlideActive = true;

		// Compute and set baseValue.
		float newValue = computeSliderValueAtTouchWorldPos(localPos.x, localPos.y);
		float newRangeValue = valueToRangeValue(newValue, rangeMin, rangeMax, fixedIncrementDelta, roundValue);

		if (newRangeValue != rangeValue) {
			setRangeValue(newRangeValue, false, true);
		}

		return true;
	} else {
		throw LogicException(LOC);
	}
}

void Slider2D::v2d_onCursorDrag(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
	if (target == touchAreaBack && isTouchSlideActive && cursorIndex == touchDownIndex) {
		// Compute and set baseValue.
		float newValue = computeSliderValueAtTouchWorldPos(localPos.x, localPos.y);
		float newRangeValue = valueToRangeValue(newValue, rangeMin, rangeMax, fixedIncrementDelta, roundValue);

		if (newRangeValue != rangeValue) {
			setRangeValue(newRangeValue, false, true);
		}
	}
}

void Slider2D::v2d_onCursorUp(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (target == touchAreaBack && isTouchSlideActive && cursorIndex == touchDownIndex) {
		isTouchSlideActive = false;
	}
}

void Slider2D::v2d_onCursorUpOutside(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	if (target == touchAreaBack && isTouchSlideActive && cursorIndex == touchDownIndex) {
		isTouchSlideActive = false;
	}
}

void Slider2D::setEventListener(IListener* eventListener) {
	this->eventListener = eventListener;
}

void Slider2D::createVisuals(Map1D<std::string, void*>* createConfigMap) {
	super::createVisuals(createConfigMap);

	tickMarksLayer = getItemMustExistAs< IContainer2D * >("tickMarksLayer");
	bg_bar_base = getItemMustExistAs< IInteractiveDrawable2D * >("bg_bar_base");
	bg_bar_fill = getItemMustExistAs< IInteractiveDrawable2D* >("bg_bar_fill");
	knob = getItemMustExistAs< IInteractiveDrawable2D* >("knob");
	touchAreaBack = getItemMustExistAs< IInteractiveDrawable2D* >("touchAreaBack");
	touchAreaBack->addTouchListener(&touchListener);
}

void Slider2D::setIsGrayZeroKnob(bool isGrayZeroKnob) {
	if (this->isGrayZeroKnob == isGrayZeroKnob) {
		return;
	}

	this->isGrayZeroKnob = isGrayZeroKnob;

	isDirty_valueIndicators = true;
}

void Slider2D::setIsShowActiveTickMarks(bool showActiveTickMarks) {
	if (this->isShowActiveTickMarks == showActiveTickMarks) {
		return;
	}

	this->isShowActiveTickMarks = showActiveTickMarks;

	isDirty_tickMarksCount = true;
}

void Slider2D::setSlideFocusStatePercent(float slideFocusStatePercent) {
	if (this->slideFocusStatePercent == slideFocusStatePercent) {
		return;
	}

	this->slideFocusStatePercent = slideFocusStatePercent;

	putDataMapValueB("slideFocusStatePercent", base::IWrappedValue::new_float(this->slideFocusStatePercent), false);

	isDirty_valueIndicators = true;
}

void Slider2D::setVisualUpdateValue(float visualUpdateValue) {
	if (this->visualUpdateValue == visualUpdateValue) {
		return;
	}

	this->visualUpdateValue = visualUpdateValue;

	putDataMapValueB("visualUpdateValue", base::IWrappedValue::new_float(this->visualUpdateValue), false);

	isDirty_valueIndicators = true;
}

void Slider2D::visualLoop(float deltaS, int deltaMS) {
	super::visualLoop(deltaS, deltaMS);

	bool doSyncVisuals = false;

	if ((!isTouchSlideActive && slideFocusStatePercent > 0.0f)
		|| (isTouchSlideActive && slideFocusStatePercent < 1.0f))
	{
		if (!isTouchSlideActive) {
			setSlideFocusStatePercent(Math::max(0.0f, slideFocusStatePercent - deltaS / activeStateChangeAnimationDurationS));
		} else {
			setSlideFocusStatePercent(Math::min(1.0f, slideFocusStatePercent + deltaS / activeStateChangeAnimationDurationS));
		}

		doSyncVisuals = true;
	}

	if (visualUpdateValue != value) {
		visualUpdateValue += (value - visualUpdateValue) * (movementSpeedMultiplierS * deltaS);

		doSyncVisuals = true;
	}

	if (isDirty_anchor) {
		isDirty_anchor = false;

		doSyncVisuals = true;
	}
	
	if (isDirty_barSize) {
		isDirty_barSize = false;

		doSyncVisuals = true;
	}

	if (isDirty_knobSize) {
		isDirty_knobSize = false;

		doSyncVisuals = true;
	}

	if (isDirty_valueIndicators) {
		isDirty_valueIndicators = false;

		if (visualUpdateValue <= 0.0f && isGrayZeroKnob) {
			putDataMapValueB("knobAlpha", base::IWrappedValue::new_float(1.0f - (1.0f - 0.4f) * slideFocusStatePercent), false);
			float rgbOff = 1.0f - AppStyles::ICON_BLACK_ALPHA;
			float rgbOn = 0.7f;
			tCol.set(
				rgbOff + (rgbOn - rgbOff) * slideFocusStatePercent,
				rgbOff + (rgbOn - rgbOff) * slideFocusStatePercent,
				rgbOff + (rgbOn - rgbOff) * slideFocusStatePercent,
				1.0f
			);
			putDataMapValueB("knobColor", base::IWrappedValue::new_Color(tCol), false);
		} else {
			Color col("#2196F3FF");
			putDataMapValueB("knobColor", base::IWrappedValue::new_Color(col), false);
		}

		doSyncVisuals = true;
	}

	if (isDirty_tickMarksCount) {
		isDirty_tickMarksCount = false;

		isDirty_tickMarksStyle = true;

		int tickMarksCount = (int)Math::floor((rangeMax - rangeMin) / fixedIncrementDelta) + 1;
		while (tickMarksList.size() < tickMarksCount) {
			std::string atlasID = "visual2D/taUtilLinear.atlas";
			std::string regionID = "bg/simple_white_FFFFFF";
			Sprite2D* tickMark = VisualUtil2D::newSprite(
				appAssets->getEmbeddedAssetsManager(),
				&atlasID, &regionID,
				tickMarksLayer
			);
			tickMark->setAnchor(
				0.5f,
				0.5f
			);

			tickMarksList.appendDirect(tickMark);
		}
		while (tickMarksList.size() > tickMarksCount) {
			// Remove the last tick mark.
			int i = tickMarksList.size() - 1;
			Sprite2D* tickMark = tickMarksList.removeAndGetDirect(i);
			if (tickMark->getParent() != nullptr) {
				tickMark->getParent()->removeChild(tickMark);
			}

			delete tickMark;
		}
	}

	if (isDirty_tickMarksStyle) {
		isDirty_tickMarksStyle = false;

		float unroundedTickSegmentsCount = (rangeMax - rangeMin) / fixedIncrementDelta;
		float tickSegmentsWidth = (1.0f / unroundedTickSegmentsCount) * barWidth;

		// Update style and pos.
		for (int i = 0; i < tickMarksList.size(); i++) {
			Sprite2D* tickMark = tickMarksList.getDirect(i);

			float posX = tickSegmentsWidth * i;

			tickMark->setPosition(
				posX,
				0.0f
			);
			tickMark->setSize(
				activeTickMark_width,
				activeTickMark_height
			);
			tickMark->setTintColor(activeTickMark_tintColor);
			tickMark->setAlpha(activeTickMark_alpha);
		}
	}

	if (doSyncVisuals) {
		syncVisuals(false, true, startStateID, endStateID, stateCompletionPercent);
	}
}

std::shared_ptr<base::IWrappedValue> Slider2D::getPropertyValue(std::string property) {
	if (property == "x") {
		return base::IWrappedValue::new_float(visualsHolder->getX());
	} else if (property == "y") {
		return base::IWrappedValue::new_float(visualsHolder->getY());
	}

	else if (property == "anchorX") {
		return base::IWrappedValue::new_float(anchorX);
	}

	else if (property == "barWidth") {
		return base::IWrappedValue::new_float(barWidth);
	} else if (property == "movementSpeedMultiplierS") {
		return base::IWrappedValue::new_float(movementSpeedMultiplierS);
	}

	else if (property == "knobDiameter_idle") {
		return base::IWrappedValue::new_float(knobDiameter_idle);
	} else if (property == "knobDiameter_active") {
		return base::IWrappedValue::new_float(knobDiameter_active);
	}

	else if (property == "activeTickMark_width") {
		return base::IWrappedValue::new_float(activeTickMark_width);
	} else if (property == "activeTickMark_height") {
		return base::IWrappedValue::new_float(activeTickMark_height);
	} else if (property == "activeTickMark_tintColor") {
		return base::IWrappedValue::new_Color(activeTickMark_tintColor);
	} else if (property == "activeTickMark_alpha") {
		return base::IWrappedValue::new_float(activeTickMark_alpha);
	}

	throw LogicException(LOC);
}

void Slider2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
	if (property == "x") {
		visualsHolder->setX(value->getDirectAs_float());
	} else if (property == "y") {
		visualsHolder->setY(value->getDirectAs_float());
	}

	else if (property == "anchorX") {
		anchorX = value->getDirectAs_float();
		isDirty_anchor = true;
	}

	else if (property == "barWidth") {
		barWidth = value->getDirectAs_float();
		isDirty_barSize = true;
	} else if (property == "movementSpeedMultiplierS") {
		movementSpeedMultiplierS = value->getDirectAs_float();
	}

	else if (property == "knobDiameter_idle") {
		knobDiameter_idle = value->getDirectAs_float();
		isDirty_knobSize = true;
	} else if (property == "knobDiameter_active") {
		knobDiameter_active = value->getDirectAs_float();
		isDirty_knobSize = true;
	}

	else if (property == "activeTickMark_width") {
		activeTickMark_width = value->getDirectAs_float();
		isDirty_tickMarksStyle = true;
	} else if (property == "activeTickMark_height") {
		activeTickMark_height = value->getDirectAs_float();
		isDirty_tickMarksStyle = true;
	} else if (property == "activeTickMark_tintColor") {
		activeTickMark_tintColor.set(value->getReferenceAs_Color());
		isDirty_tickMarksStyle = true;
	} else if (property == "activeTickMark_alpha") {
		activeTickMark_alpha = value->getDirectAs_float();
		isDirty_tickMarksStyle = true;
	}

	else {
		throw LogicException(LOC);
	}
}

float Slider2D::getRangeValue() {
	return rangeValue;
}

void Slider2D::setRangeValue(float rangeValue, bool doAnimation, bool dispatchEvent) {
	// Update even if the same baseValue because the extra parameters need to be processed.
	/*if(this->baseValue == baseValue) {
		return ;
	}*/

	this->rangeValue = Math::max(rangeMin, Math::min(rangeMax, rangeValue));
	this->value = rangeValueToValue(
		this->rangeValue,
		rangeMin, rangeMax
	);

	if (!doAnimation) {
		setVisualUpdateValue(this->value);
	}

	isDirty_valueIndicators = true;

	if (dispatchEvent) {
		if (eventListener != nullptr) {
			eventListener->onValueChanged(this);
		}
	}
}

float Slider2D::rangeValueToValue(
	float rangeValue,
	float rangeMin, float rangeMax)
{
	float value = (rangeValue - rangeMin) / (rangeMax - rangeMin);

	return value;
}

float Slider2D::valueToRangeValue(
	float value,
	float rangeMin, float rangeMax,
	float fixedIncrementDelta, bool roundValue)
{
	float rangeValue = rangeMin + (rangeMax - rangeMin) * value;
	rangeValue = Math::max(rangeMin, Math::min(rangeMax, rangeValue));

	if (fixedIncrementDelta != 0.0f) {
		rangeValue = rangeMin + Math::round((rangeValue - rangeMin) / fixedIncrementDelta) * fixedIncrementDelta;
		float f = (float)Math::floor((rangeMax - rangeMin) / fixedIncrementDelta);
		float highestStep = rangeMin + f * fixedIncrementDelta;
		rangeValue = Math::min(rangeValue, highestStep);
	}

	if (roundValue) {
		rangeValue = Math::round(rangeValue);
	}

	return rangeValue;
}

void Slider2D::setRange(float rangeMin, float rangeMax, float fixedIncrementDelta, bool roundValue) {
	if (this->rangeMin == rangeMin && this->rangeMax == rangeMax && this->fixedIncrementDelta == fixedIncrementDelta && this->roundValue == roundValue) {
		return;
	}

	this->rangeMin = rangeMin;
	this->rangeMax = rangeMax;
	this->fixedIncrementDelta = fixedIncrementDelta;
	this->roundValue = roundValue;

	isDirty_tickMarksCount = true;

	setRangeValue(this->rangeMin, false, false);
}

float Slider2D::computeSliderValueAtTouchWorldPos(float localX, float localY) {
	float val = localX / barWidth;
	val = Math::max(0.0f, Math::min(1.0f, val));

	return val;
}

void Slider2D::disposeMain() {
	while (tickMarksList.count() > 0) {
		Sprite2D* entry = tickMarksList.removeAndGetDirect(tickMarksList.count() - 1);
		if (entry->getParent() != nullptr) {
			entry->getParent()->removeChild(entry);
		}

		delete entry;
	}

	super::disposeMain();
}

Slider2D::~Slider2D() {
	//void
}
