#pragma once

#include <base/input/util/Touch.h>
#include <base/input/util/TouchEventType.h>

namespace base {
class IScreenTouch2DListener {
public:
    virtual void onScreenTouch2D(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, float screenX, float screenY) = 0;

    virtual ~IScreenTouch2DListener() = default;
};
};
