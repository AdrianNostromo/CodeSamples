#pragma once

#include <base/interaction/ITouchListener.h>
#include <base/list/ArrayList.h>

class WrappedTouchListener2D {
public:
    ITouchListener* listener;

private:
    //[cursorIndex]: (bool status).
    // 0: false;
    // 1: true;
    // NOTE : Use int instead of bool because std::vector optimises <bool> to occupy 1 bit per element. This special case doesn't work with the normal ArrayList code.
    ArrayList<int> isListenerHookingTouchDownList{};

public:
    WrappedTouchListener2D(WrappedTouchListener2D const&) = delete;
    WrappedTouchListener2D(WrappedTouchListener2D &&) = default;
    WrappedTouchListener2D& operator=(WrappedTouchListener2D const&) = delete;
    WrappedTouchListener2D& operator=(WrappedTouchListener2D &&) = default;

    explicit WrappedTouchListener2D(ITouchListener* listener);

    bool getIsListenerHookingTouchDown(int cursorIndex);
    void putIsListenerHookingTouchDown(int cursorIndex, bool isListenerHookingTouchDown);

    virtual ~WrappedTouchListener2D();
};
