#include "ReactiveInteractiveDrawable2D.h"
#include <base/sensor/hapticFeedback/IHapticFeedback.h>

ReactiveInteractiveDrawable2D::ReactiveInteractiveDrawable2D(
	base::audio::ISound* touchDownSound, float touchDownSound_volume, base::IHapticFeedback* touchDown_hapticFeedback,
	base::audio::ISound* touchUpSound, float touchUpSound_volume)
    : super(),
      touchDownSound(touchDownSound), touchDownSound_volume(touchDownSound_volume), touchDown_hapticFeedback(touchDown_hapticFeedback),
      touchUpSound(touchUpSound), touchUpSound_volume(touchUpSound_volume)
{
	//void
}

void ReactiveInteractiveDrawable2D::disableSoundsUntilNextTouchDown() {
	isSoundDisabledUntilNextTouchDown = true;
	isSoundDisabledUntilNextTouchDownPost = true;
}

void ReactiveInteractiveDrawable2D::playTouchDownSound() {
	if (touchDownSound != nullptr) {
		touchDownSound->play(touchDownSound_volume);
	}
	if(touchDown_hapticFeedback != nullptr) {
		touchDown_hapticFeedback->play();
	}
}

void ReactiveInteractiveDrawable2D::playTouchUpSound() {
	if (touchUpSound != nullptr) {
		touchUpSound->play(touchUpSound_volume);
	}
}

bool ReactiveInteractiveDrawable2D::onCursorDown(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	bool b = super::onCursorDown(
		target,
		cursorIndex, buttonId,
		screenPos, localPos
	);

	b = true;

	if (isSoundDisabledUntilNextTouchDown) {
		isSoundDisabledUntilNextTouchDown = false;

		return b;
	}

	if (isSoundDisabledUntilNextTouchDownPost) {
		isSoundDisabledUntilNextTouchDownPost = false;
	}

	playTouchDownSound();

	return b;
}

void ReactiveInteractiveDrawable2D::onCursorUp(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	super::onCursorUp(
		target,
		cursorIndex, buttonId,
		screenPos, localPos
	);

	if (isSoundDisabledUntilNextTouchDownPost) {
		return;
	}

	playTouchUpSound();
}

void ReactiveInteractiveDrawable2D::onCursorUpOutside(
	IInteractiveDrawable2D* target,
	int cursorIndex, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& localPos)
{
	super::onCursorUpOutside(
		target,
		cursorIndex, buttonId,
		screenPos, localPos
	);

	if (isSoundDisabledUntilNextTouchDownPost) {
		return;
	}

	playTouchUpSound();
}

void ReactiveInteractiveDrawable2D::disposeMain() {
	if(touchDown_hapticFeedback != nullptr) {
		delete touchDown_hapticFeedback;
		touchDown_hapticFeedback = nullptr;
	}

	super::disposeMain();
}

ReactiveInteractiveDrawable2D::~ReactiveInteractiveDrawable2D() {
    //void
}
