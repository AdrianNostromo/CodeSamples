#pragma once

#include "ISlider2D.h"
#include <base/menu/AnimatedComposedStiledVisual.h>
#include <base/interaction/CbTouchListener.h>

class Sprite2D;

namespace base {
class Slider2D : public AnimatedComposedStiledVisual, virtual public ISlider2D {
private: typedef AnimatedComposedStiledVisual super;
private:
	static ArrayList<MenuItemConfig*>* itemConfigs;

	static ArrayList<StateChangeDurations*>* animationDurations;

protected:
	IListener* eventListener = nullptr;

	float barWidth = 0.0f;
	float anchorX = 0.0f;
	float movementSpeedMultiplierS = 10.0f;

	float knobDiameter_idle = 0.0f;
	float knobDiameter_active = 0.0f;

	float activeTickMark_width = 0.0f;
	float activeTickMark_height = 0.0f;
	Color activeTickMark_tintColor{ "#000000FF" };
	float activeTickMark_alpha = 1.0f;

	bool isGrayZeroKnob = false;

	bool isShowActiveTickMarks = false;

	float activeStateChangeAnimationDurationS = 0.1f;

	IContainer2D* tickMarksLayer = nullptr;
	IInteractiveDrawable2D* bg_bar_base = nullptr;
	IInteractiveDrawable2D* bg_bar_fill = nullptr;
	IInteractiveDrawable2D* knob = nullptr;
	IInteractiveDrawable2D* touchAreaBack = nullptr;

	float visualUpdateValue = 0.0f;

private:
	CbTouchListener touchListener{};

	float rangeMin = 0.0f;
	float rangeMax = 1.0f;
	// 0 - Not used.
	float fixedIncrementDelta = 0.0f;
	bool roundValue = false;

	// [0f, 1f]
	float value = 0.0f;
	// [rangeMin, rangeMax]
	float rangeValue = 0.0f;

	int touchDownIndex = -1;
	bool isTouchSlideActive = false;
	// Animates from iddle(0f) to touch_down(1f) so the knob grows in rendering3DSize when touch slide is in progress.
	float slideFocusStatePercent = 0.0f;

	bool isDirty_anchor = false;
	bool isDirty_barSize = false;
	bool isDirty_knobSize = false;
	bool isDirty_valueIndicators = false;
	bool isDirty_tickMarksStyle = false;
	bool isDirty_tickMarksCount = false;

	ArrayList<Sprite2D*> tickMarksList{};

	Color tCol{};

public:
    Slider2D(Slider2D const&) = delete;
    Slider2D(Slider2D &&) = default;
    Slider2D& operator=(Slider2D const&) = delete;
    Slider2D& operator=(Slider2D &&) = default;

    explicit Slider2D(IAppMetrics* appMetrics, IAppAssets* appAssets);

	void setEventListener(IListener* eventListener) override;

	void setIsGrayZeroKnob(bool isGrayZeroKnob);

	void setIsShowActiveTickMarks(bool showActiveTickMarks);

	void visualLoop(float deltaS, int deltaMS) override;

	std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
	void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

	float getRangeValue() override;
	void setRangeValue(float rangeValue, bool doAnimation, bool dispatchEvent) override;
	void setRange(float rangeMin, float rangeMax, float fixedIncrementDelta, bool roundValue);

    ~Slider2D() override;
protected:

	void disposeMain() override;
private:
	bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	void v2d_onCursorDrag(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW);
	void v2d_onCursorUp(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);
	void v2d_onCursorUpOutside(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	void setSlideFocusStatePercent(float slideFocusStatePercent);
	void setVisualUpdateValue(float visualUpdateValue);

	static float rangeValueToValue(
		float rangeValue,
		float rangeMin, float rangeMax);

	static float valueToRangeValue(
		float value,
		float rangeMin, float rangeMax,
		float fixedIncrementDelta, bool roundValue);

	float computeSliderValueAtTouchWorldPos(float localX, float localY);
};
};
