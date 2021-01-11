#include <base/input/util/TouchEventType.h>
#include "BufferedTouchListener.h"

BufferedTouchListener::BufferedTouchListener(
    bool isBuffer_CURSOR_DOWN,

    bool isBuffer_CURSOR_UP,
    bool isBuffer_CURSOR_UP_OUTSIDE,

    bool isBuffer_CURSOR_DRAG,

    bool isBuffer_CURSOR_MOVE,

    bool isBuffer_CURSOR_OVER,
    bool isBuffer_CURSOR_OUT
)
    : super(),
      isBuffer_CURSOR_DOWN(isBuffer_CURSOR_DOWN),

      isBuffer_CURSOR_UP(isBuffer_CURSOR_UP),
      isBuffer_CURSOR_UP_OUTSIDE(isBuffer_CURSOR_UP_OUTSIDE),

      isBuffer_CURSOR_DRAG(isBuffer_CURSOR_DRAG),

      isBuffer_CURSOR_MOVE(isBuffer_CURSOR_MOVE),

      isBuffer_CURSOR_OVER(isBuffer_CURSOR_OVER),
      isBuffer_CURSOR_OUT(isBuffer_CURSOR_OUT)
{
    //void
}

bool BufferedTouchListener::getIsBufferingEnabled() {
    return isBufferingEnabled;
}

void BufferedTouchListener::setIsBufferingEnabled(bool isBufferingEnabled) {
    this->isBufferingEnabled = isBufferingEnabled;
}

bool BufferedTouchListener::onCursorDown(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos)
{
    bool ret = super::onCursorDown(
        target,
        cursorIndex, buttonId,
        screenPos, localPos
    );

    if(isBufferingEnabled && isBuffer_CURSOR_DOWN) {
        TouchEventType* type = TouchEventType::CURSOR_DOWN;
        Entry* e = eventsList.append_emplace(type, target);

        e->cursorIndex = cursorIndex;
        e->buttonId = buttonId;

		e->screenX = screenPos.x;
		e->screenY = screenPos.y;
		e->localX = localPos.x;
		e->localY = localPos.y;

        // Make sure to always receive move and touch_up_outside for this touch down target.
        return true;
    }

    return ret;
}

void BufferedTouchListener::onCursorUp(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos)
{
    super::onCursorUp(
        target,
        cursorIndex, buttonId,
        screenPos, localPos
    );

    if(isBufferingEnabled && isBuffer_CURSOR_UP) {
        TouchEventType* type = TouchEventType::CURSOR_UP;
        Entry* e = eventsList.append_emplace(type, target);

        e->cursorIndex = cursorIndex;
        e->buttonId = buttonId;

        e->screenX = screenPos.x;
        e->screenY = screenPos.y;
        e->localX = localPos.x;
        e->localY = localPos.y;
    }
}

void BufferedTouchListener::onCursorUpOutside(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos)
{
    super::onCursorUpOutside(
        target,
        cursorIndex, buttonId,
        screenPos, localPos
    );

    if(isBufferingEnabled && isBuffer_CURSOR_UP_OUTSIDE) {
        TouchEventType* type = TouchEventType::CURSOR_UP_OUTSIDE;
        Entry* e = eventsList.append_emplace(type, target);

        e->cursorIndex = cursorIndex;
        e->buttonId = buttonId;

		e->screenX = screenPos.x;
		e->screenY = screenPos.y;
		e->localX = localPos.x;
		e->localY = localPos.y;
    }
}

void BufferedTouchListener::onCursorDrag(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
    super::onCursorDrag(
        target,
        cursorIndex, buttonId,
        screenPos, localPos,
		deltaPosW
    );

    if(isBufferingEnabled && isBuffer_CURSOR_DRAG) {
        TouchEventType* type = TouchEventType::CURSOR_DRAG;
        Entry* e = eventsList.append_emplace(type, target);

        e->cursorIndex = cursorIndex;
        e->buttonId = buttonId;

		e->screenX = screenPos.x;
		e->screenY = screenPos.y;
		e->localX = localPos.x;
		e->localY = localPos.y;
    }
}

void BufferedTouchListener::onCursorMove(
    IInteractiveDrawable2D* target,
    int cursorIndex,
    Vector2& screenPos, Vector2& localPos,
	Vector2& deltaPosW)
{
    super::onCursorMove(
        target,
        cursorIndex,
        screenPos, localPos,
		deltaPosW
    );

    if(isBufferingEnabled && isBuffer_CURSOR_MOVE) {
        TouchEventType* type = TouchEventType::CURSOR_MOVE;
        Entry* e = eventsList.append_emplace(type, target);

        e->cursorIndex = cursorIndex;

		e->screenX = screenPos.x;
		e->screenY = screenPos.y;
		e->localX = localPos.x;
		e->localY = localPos.y;
    }
}

void BufferedTouchListener::onCursorOver(
    IInteractiveDrawable2D* target,
    int cursorIndex,
    Vector2& screenPos, Vector2& localPos)
{
    super::onCursorOver(
        target,
        cursorIndex,
        screenPos, localPos
    );

    if(isBufferingEnabled && isBuffer_CURSOR_OVER) {
        TouchEventType* type = TouchEventType::CURSOR_OVER;
        Entry* e = eventsList.append_emplace(type, target);

        e->cursorIndex = cursorIndex;

		e->screenX = screenPos.x;
		e->screenY = screenPos.y;
		e->localX = localPos.x;
		e->localY = localPos.y;
    }
}

void BufferedTouchListener::onCursorOut(
    IInteractiveDrawable2D* target,
    int cursorIndex,
    Vector2& screenPos, Vector2& localPos)
{
    super::onCursorOut(
        target,
        cursorIndex,
        screenPos, localPos
    );

    if(isBufferingEnabled && isBuffer_CURSOR_OUT) {
        TouchEventType* type = TouchEventType::CURSOR_OUT;
        Entry* e = eventsList.append_emplace(type, target);

        e->cursorIndex = cursorIndex;

		e->screenX = screenPos.x;
		e->screenY = screenPos.y;
		e->localX = localPos.x;
		e->localY = localPos.y;
    }
}

BufferedTouchListener::~BufferedTouchListener() {
    //void
}

BufferedTouchListener::Entry::Entry(TouchEventType* type, IInteractiveDrawable2D* target)
    : type(type), target(target)
{
    //void
}

BufferedTouchListener::Entry::~Entry() {
    //void
}
