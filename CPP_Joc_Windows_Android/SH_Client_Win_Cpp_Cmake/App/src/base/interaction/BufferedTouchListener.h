#pragma once

#include <base/list/ArrayList.h>
#include <base/input/util/Touch.h>
#include "TouchListener.h"
#include <base/input/util/TouchEventType.h>

class BufferedTouchListener : public TouchListener {
private: typedef TouchListener super;
public:
    class Entry {
    public:
        TouchEventType* type;
        IInteractiveDrawable2D* target;

        int cursorIndex;
        base::Touch::ButtonCode* buttonId = base::Touch::ButtonCode::None;

        float screenX;
        float screenY;

        float localX;
        float localY;

    public:
        Entry(Entry const&) = delete;
        Entry(Entry &&) = default;
        Entry& operator=(Entry const&) = delete;
        Entry& operator=(Entry &&) = default;
        explicit Entry(TouchEventType* type, IInteractiveDrawable2D* target);
        ~Entry();
    };
public:
    bool isBuffer_CURSOR_DOWN;

    bool isBuffer_CURSOR_UP;
    bool isBuffer_CURSOR_UP_OUTSIDE;

    bool isBuffer_CURSOR_DRAG;

    bool isBuffer_CURSOR_MOVE;

    bool isBuffer_CURSOR_OVER;
    bool isBuffer_CURSOR_OUT;

    // Convert to shared_ptr;
    ArrayList<Entry> eventsList{};

    bool isBufferingEnabled = true;

public:
    BufferedTouchListener(BufferedTouchListener const&) = delete;
    BufferedTouchListener(BufferedTouchListener &&) = default;
    BufferedTouchListener& operator=(BufferedTouchListener const&) = delete;
    BufferedTouchListener& operator=(BufferedTouchListener &&) = default;

    explicit BufferedTouchListener(
        bool isBuffer_CURSOR_DOWN,

        bool isBuffer_CURSOR_UP,
        bool isBuffer_CURSOR_UP_OUTSIDE,

        bool isBuffer_CURSOR_DRAG,

        bool isBuffer_CURSOR_MOVE,

        bool isBuffer_CURSOR_OVER,
        bool isBuffer_CURSOR_OUT
    );

    bool getIsBufferingEnabled();
    void setIsBufferingEnabled(bool isBufferingEnabled);

    bool onCursorDown(
            IInteractiveDrawable2D* target,
            int cursorIndex, base::Touch::ButtonCode* buttonId,
            Vector2& screenPos, Vector2& localPos) override;

    void onCursorUp(
            IInteractiveDrawable2D* target,
            int cursorIndex, base::Touch::ButtonCode* buttonId,
            Vector2& screenPos, Vector2& localPos) override;

    void onCursorUpOutside(
            IInteractiveDrawable2D* target,
            int cursorIndex, base::Touch::ButtonCode* buttonId,
            Vector2& screenPos, Vector2& localPos) override;

    void onCursorDrag(
            IInteractiveDrawable2D* target,
            int cursorIndex, base::Touch::ButtonCode* buttonId,
            Vector2& screenPos, Vector2& localPos,
		Vector2& deltaPosW) override;

    void onCursorMove(
        IInteractiveDrawable2D* target,
        int cursorIndex,
        Vector2& screenPos, Vector2& localPo,
		Vector2& deltaPosWs) override;

    void onCursorOver(
        IInteractiveDrawable2D* target,
        int cursorIndex,
        Vector2& screenPos, Vector2& localPos) override;
    void onCursorOut(
        IInteractiveDrawable2D* target,
        int cursorIndex,
        Vector2& screenPos, Vector2& localPos) override;

    ~BufferedTouchListener() override;

};
