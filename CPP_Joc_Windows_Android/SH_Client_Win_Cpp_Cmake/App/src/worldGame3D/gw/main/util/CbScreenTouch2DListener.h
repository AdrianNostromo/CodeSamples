#pragma once

#include "IScreenTouch2DListener.h"
#include <functional>
#include <base/input/util/TouchEventType.h>

namespace base {
class CbScreenTouch2DListener : virtual public IScreenTouch2DListener {
public:
    std::function<void(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, float screenX, float screenY)> cb_onScreenTouch2D;

public:
    CbScreenTouch2DListener(CbScreenTouch2DListener const&) = delete;
    CbScreenTouch2DListener(CbScreenTouch2DListener &&) = default;
    CbScreenTouch2DListener& operator=(CbScreenTouch2DListener const&) = delete;
    CbScreenTouch2DListener& operator=(CbScreenTouch2DListener &&) = default;

    explicit CbScreenTouch2DListener();

    void onScreenTouch2D(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, float screenX, float screenY) override;

    ~CbScreenTouch2DListener() override;
};
};
