#pragma once

#include "ReactiveInteractiveDrawable2D.h"

class TouchArea2D : public ReactiveInteractiveDrawable2D {
private: typedef ReactiveInteractiveDrawable2D super;
public:
    TouchArea2D(TouchArea2D const&) = delete;
    TouchArea2D(TouchArea2D &&) = default;
    TouchArea2D& operator=(TouchArea2D const&) = delete;
    TouchArea2D& operator=(TouchArea2D &&) = default;

    explicit TouchArea2D(
        base::audio::ISound* touchDownSound, float touchDownSound_volume, base::IHapticFeedback* touchDown_hapticFeedback,
        base::audio::ISound* touchUpSound, float touchUpSound_volume);

    virtual ~TouchArea2D();
};
