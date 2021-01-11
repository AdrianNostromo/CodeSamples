#include "CbTouchListener.h"

CbTouchListener::CbTouchListener() {
    //void
}

bool CbTouchListener::onCursorDown(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
    if(cb_onCursorDown) {
        return cb_onCursorDown(
            target,
            cursorIndex, buttonId,
            screenPos, localPos);
    }

    return false;
}

void CbTouchListener::onCursorUp(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
    if(cb_onCursorUp) {
        cb_onCursorUp(
            target,
            cursorIndex, buttonId,
            screenPos, localPos);
    }
}

void CbTouchListener::onCursorUpOutside(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
    if(cb_onCursorUpOutside) {
        cb_onCursorUpOutside(
            target,
            cursorIndex, buttonId,
            screenPos, localPos);
    }
}

void CbTouchListener::onCursorTap(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos)
{
    if(cb_onCursorTap) {
        cb_onCursorTap(
            target,
            cursorIndex, buttonId,
            screenPos, localPos);
    }
}

void CbTouchListener::onCursorDrag(
    IInteractiveDrawable2D* target,
    int cursorIndex, base::Touch::ButtonCode* buttonId,
    Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
    if(cb_onCursorDrag) {
        cb_onCursorDrag(
            target,
            cursorIndex, buttonId,
            screenPos, localPos,
			deltaPosW
		);
    }
}

void CbTouchListener::onCursorMove(
    IInteractiveDrawable2D* target,
    int cursorIndex,
	Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
    if(cb_onCursorMove) {
        cb_onCursorMove(
            target,
            cursorIndex,
            screenPos, localPos,
			deltaPosW
		);
    }
}

void CbTouchListener::onCursorOver(
    IInteractiveDrawable2D* target,
    int cursorIndex,
    Vector2& screenPos, Vector2& localPos)
{
    if(cb_onCursorOver) {
        cb_onCursorOver(
            target,
            cursorIndex,
			screenPos, localPos);
    }
}

void CbTouchListener::onCursorOut(
    IInteractiveDrawable2D* target,
    int cursorIndex,
    Vector2& screenPos, Vector2& localPos)
{
    if(cb_onCursorOut) {
        cb_onCursorOut(
            target,
            cursorIndex,
            screenPos, localPos);
    }
}

CbTouchListener::~CbTouchListener() {
    //void
}
