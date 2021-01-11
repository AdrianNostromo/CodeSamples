#pragma once

#include <graphics/visual2d/drawable/IRoot2D.h>

class IInteractiveDrawable2D;

class IRoot2DInteractive : virtual public IRoot2D {
public:
    virtual void clearExistingTouchDownTarget(IInteractiveDrawable2D* entry, int cursorIndex, base::Touch::ButtonCode* buttonId) = 0;
    virtual void clearExistingCursorOverTarget(IInteractiveDrawable2D* entry, int cursorIndex) = 0;
};
