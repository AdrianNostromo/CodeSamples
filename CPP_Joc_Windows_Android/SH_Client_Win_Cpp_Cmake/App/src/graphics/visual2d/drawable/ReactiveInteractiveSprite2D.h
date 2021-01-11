#pragma once

#include "Sprite2D.h"
#include <base/interaction/CbTouchListener.h>
#include <base/audio/sound/ISound.h>

class ReactiveInteractiveSprite2D : public Sprite2D {
private: typedef Sprite2D super;
private:
	base::audio::ISound* touchDownSound;
	float touchDownSound_volume;

	base::audio::ISound* touchUpSound;
	float touchUpSound_volume;

    bool isSoundDisabledUntilNextTouchDown = false;
    bool isSoundDisabledUntilNextTouchDownPost = false;

public:
    ReactiveInteractiveSprite2D(ReactiveInteractiveSprite2D const&) = delete;
    ReactiveInteractiveSprite2D(ReactiveInteractiveSprite2D &&) = default;
    ReactiveInteractiveSprite2D& operator=(ReactiveInteractiveSprite2D const&) = delete;
    ReactiveInteractiveSprite2D& operator=(ReactiveInteractiveSprite2D &&) = default;

    explicit ReactiveInteractiveSprite2D(
    	base::audio::ISound* touchDownSound, float touchDownSound_volume,
    	base::audio::ISound* touchUpSound, float touchUpSound_volume);

	void disableSoundsUntilNextTouchDown();

    ~ReactiveInteractiveSprite2D() override;
protected:
	virtual void playTouchDownSound();
	virtual void playTouchUpSound();

	void disposeMain() override;
private:
	bool onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;
	void onCursorUp(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;
	void onCursorUpOutside(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) override;

};
