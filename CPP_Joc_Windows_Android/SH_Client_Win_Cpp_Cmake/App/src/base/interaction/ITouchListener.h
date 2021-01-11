#pragma once

#include <base/math/Vector2.h>
#include <base/input/util/Touch.h>

class IInteractiveDrawable2D;

class ITouchListener {
public:
    // return true : it will receive the next touch up event anywhere it occurs.
    virtual bool onCursorDown(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) = 0;

    virtual void onCursorUp(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) = 0;

    virtual void onCursorUpOutside(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) = 0;

    virtual void onCursorTap(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos) = 0;

    virtual void onCursorDrag(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW) = 0;

    virtual void onCursorMove(
        IInteractiveDrawable2D* target,
        int cursorIndex,
		Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW) = 0;

    virtual void onCursorOver(
        IInteractiveDrawable2D* target,
        int cursorIndex,
		Vector2& screenPos, Vector2& localPos) = 0;
    virtual void onCursorOut(
        IInteractiveDrawable2D* target,
        int cursorIndex,
		Vector2& screenPos, Vector2& localPos) = 0;

    virtual ~ITouchListener() = default;
};
