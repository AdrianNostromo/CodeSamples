#include "TouchListener.h"

TouchListener::TouchListener() {
    //void
}

bool TouchListener::onCursorDown(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos)
{
    return false;
}

void TouchListener::onCursorUp(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos)
{
    //void
}

void TouchListener::onCursorUpOutside(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos)
{
    //void
}

void TouchListener::onCursorTap(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos)
{
    //void
}

void TouchListener::onCursorDrag(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW)
{
    //void
}

void TouchListener::onCursorMove(
    IInteractiveDrawable2D* target,
    int cursorIndex,
    Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
    //void
}

void TouchListener::onCursorOver(
    IInteractiveDrawable2D* target,
    int cursorIndex,
    Vector2& screenPos, Vector2& localPos)
{
    //void
}

void TouchListener::onCursorOut(
    IInteractiveDrawable2D* target,
    int cursorIndex,
    Vector2& screenPos, Vector2& localPos)
{
    //void
}

TouchListener::~TouchListener() {
    //void
}
