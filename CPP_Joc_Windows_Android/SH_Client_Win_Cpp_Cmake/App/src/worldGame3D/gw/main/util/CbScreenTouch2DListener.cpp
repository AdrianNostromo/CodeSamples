#include "CbScreenTouch2DListener.h"

using namespace base;

CbScreenTouch2DListener::CbScreenTouch2DListener() {
    //void
}

void CbScreenTouch2DListener::onScreenTouch2D(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, float screenX, float screenY) {
    if(cb_onScreenTouch2D) {
        cb_onScreenTouch2D(touchEventType, cursorIndex, buttonId, screenX, screenY);
    }
}

CbScreenTouch2DListener::~CbScreenTouch2DListener() {
    //void
}
