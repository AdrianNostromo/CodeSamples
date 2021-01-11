#include "TouchArea2D.h"

TouchArea2D::TouchArea2D(
    base::audio::ISound* touchDownSound, float touchDownSound_volume, base::IHapticFeedback* touchDown_hapticFeedback,
    base::audio::ISound* touchUpSound, float touchUpSound_volume)
    : super(touchDownSound, touchDownSound_volume, touchDown_hapticFeedback,
        touchUpSound, touchUpSound_volume)
{
    setIsInputInteractive(true);
}

TouchArea2D::~TouchArea2D() {
    //void
}
