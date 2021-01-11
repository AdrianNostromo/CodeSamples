#include "ReactiveInteractiveSprite2D.h"

ReactiveInteractiveSprite2D::ReactiveInteractiveSprite2D(
	base::audio::ISound* touchDownSound, float touchDownSound_volume,
	base::audio::ISound* touchUpSound, float touchUpSound_volume)
    : super(),
      touchDownSound(touchDownSound), touchDownSound_volume(touchDownSound_volume),
      touchUpSound(touchUpSound), touchUpSound_volume(touchUpSound_volume)
{
	//void
}

void ReactiveInteractiveSprite2D::disableSoundsUntilNextTouchDown() {
	isSoundDisabledUntilNextTouchDown = true;
	isSoundDisabledUntilNextTouchDownPost = true;
}

void ReactiveInteractiveSprite2D::playTouchDownSound() {
	if (touchDownSound != nullptr) {
		touchDownSound->play(touchDownSound_volume);
	}
}

void ReactiveInteractiveSprite2D::playTouchUpSound() {
	if (touchUpSound != nullptr) {
		touchUpSound->play(touchUpSound_volume);
	}
}

bool ReactiveInteractiveSprite2D::onCursorDown(
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

void ReactiveInteractiveSprite2D::onCursorUp(
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

void ReactiveInteractiveSprite2D::onCursorUpOutside(
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

void ReactiveInteractiveSprite2D::disposeMain() {
	//void

	super::disposeMain();
}

ReactiveInteractiveSprite2D::~ReactiveInteractiveSprite2D() {
    //void
}
