#include "WrappedTouchListener2D.h"

WrappedTouchListener2D::WrappedTouchListener2D(ITouchListener* listener)
    :listener(listener)
{
        //void
}

bool WrappedTouchListener2D::getIsListenerHookingTouchDown(int cursorIndex) {
    while(isListenerHookingTouchDownList.size() <= cursorIndex) {
        isListenerHookingTouchDownList.appendDirect(false);
    }

    bool b = isListenerHookingTouchDownList.getPointer(cursorIndex) == 0 ? false : true;
    return b;
}

void WrappedTouchListener2D::putIsListenerHookingTouchDown(int cursorIndex, bool isListenerHookingTouchDown) {
    if(getIsListenerHookingTouchDown(cursorIndex) == isListenerHookingTouchDown) {
        return ;
    }

    int i = isListenerHookingTouchDown ? 1 : 0;
    isListenerHookingTouchDownList.setDirect(cursorIndex, i);
}

WrappedTouchListener2D::~WrappedTouchListener2D() {
    //void
}
