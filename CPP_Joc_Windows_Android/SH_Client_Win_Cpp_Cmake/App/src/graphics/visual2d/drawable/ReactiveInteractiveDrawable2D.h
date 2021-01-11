#pragma once

#include "InteractiveDrawable2D.h"
#include <base/interaction/CbTouchListener.h>
#include <base/audio/sound/ISound.h>

namespace base {
	class IHapticFeedback;
};

class ReactiveInteractiveDrawable2D : public InteractiveDrawable2D {
private: typedef InteractiveDrawable2D super;
private:
	// Unmanaged.
	base::audio::ISound* touchDownSound;
	float touchDownSound_volume;
	// Managed
	base::IHapticFeedback* touchDown_hapticFeedback;

	// Unmanaged.
	base::audio::ISound* touchUpSound;
	float touchUpSound_volume;

    bool isSoundDisabledUntilNextTouchDown = false;
    bool isSoundDisabledUntilNextTouchDownPost = false;

public:
    ReactiveInteractiveDrawable2D(ReactiveInteractiveDrawable2D const&) = delete;
    ReactiveInteractiveDrawable2D(ReactiveInteractiveDrawable2D &&) = default;
    ReactiveInteractiveDrawable2D& operator=(ReactiveInteractiveDrawable2D const&) = delete;
    ReactiveInteractiveDrawable2D& operator=(ReactiveInteractiveDrawable2D &&) = default;

    explicit ReactiveInteractiveDrawable2D(
    	base::audio::ISound* touchDownSound, float touchDownSound_volume, base::IHapticFeedback* touchDown_hapticFeedback,
    	base::audio::ISound* touchUpSound, float touchUpSound_volume);

	void disableSoundsUntilNextTouchDown();

    ~ReactiveInteractiveDrawable2D() override;
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
