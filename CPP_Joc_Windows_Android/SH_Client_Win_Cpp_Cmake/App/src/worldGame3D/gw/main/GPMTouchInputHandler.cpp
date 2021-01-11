#include <base/input/util/TouchEventType.h>
#include <base/input/util/Touch.h>
#include <base/visual2D/VisualUtil2D.h>
#include <base/screen/ScreenUtil.h>
#include <base/log/GlobalAccessLogHandler.h>
#include "GPMTouchInputHandler.h"
#include <base/app/IAppAssets.h>

using namespace base;

GPMTouchInputHandler::GPMTouchInputHandler(base::IGameWorld* gw)
    : super(gw)
{
    screenListener.cb_onActiveScreenResolutionChange = std::bind(&GPMTouchInputHandler::onActiveScreenResolutionChange, this, std::placeholders::_1, std::placeholders::_2);

	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_localInput->type,
		std::bind(&GPMTouchInputHandler::onGWEvent_gameLoop_localInput, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPMTouchInputHandler::createMain() {
    super::createMain();

    base::ScreenUtil::screen->addListener(&screenListener, 3);
}

void GPMTouchInputHandler::onActiveScreenResolutionChange(int resolutionX, int resolutionY) {
    touchAreaBgMove->setSize(
        base::ScreenUtil::screen->getWindowResolutionX(),
        base::ScreenUtil::screen->getWindowResolutionY()
    );
    touchAreaBgButtons->setSize(
        base::ScreenUtil::screen->getWindowResolutionX(),
        base::ScreenUtil::screen->getWindowResolutionY()
    );
}

void GPMTouchInputHandler::setScreenTouch2DListener(IScreenTouch2DListener* screenTouch2DListener) {
    if(this->screenTouch2DListener != nullptr && screenTouch2DListener != nullptr) {
        throw LogicException(LOC);
    }
    if(this->screenTouch2DListener == nullptr && screenTouch2DListener == nullptr) {
        return ;
    }

    this->screenTouch2DListener = screenTouch2DListener;
    if(!this->screenTouch2DListener) {
        // Clear the buffer and disable events caching.

        touchListenerMove.setIsBufferingEnabled(false);
        touchListenerDrag.setIsBufferingEnabled(false);
        touchListenerButtons.setIsBufferingEnabled(false);

        touchListenerMove.eventsList.clear();
        touchListenerDrag.eventsList.clear();
        touchListenerButtons.eventsList.clear();
    }else {
        // Clear the events buffer end enable events caching because events were disabled until now.

        touchListenerMove.eventsList.clear();
        touchListenerDrag.eventsList.clear();
        touchListenerButtons.eventsList.clear();

        touchListenerMove.setIsBufferingEnabled(true);
        touchListenerDrag.setIsBufferingEnabled(true);
        touchListenerButtons.setIsBufferingEnabled(true);

        // This will force a move event for mouse cursors.
        if(touchAreaBgMove != nullptr) {
            touchAreaBgMove->onTouchBoxChanged();
        }
        if(touchAreaBgButtons != nullptr) {
            touchAreaBgButtons->onTouchBoxChanged();
        }
    }
}

void GPMTouchInputHandler::onGWEvent_gameLoop_localInput(IEventListener& eventListener, base::GWEvent& event) {
    if(screenTouch2DListener == nullptr) {
        return ;
    }

    // Touch events are ignored while the menu is animating.
    if(gw->getIsLevelEnded() || !gw->getIsLevelStarted() || !gw->getIsPlayEnabled()) {
        return ;
    }

    while(touchListenerMove.eventsList.size()) {
        BufferedTouchListener::Entry* touchEvent = touchListenerMove.eventsList.getPointer(0);
        
        if (touchEvent->type == TouchEventType::CURSOR_MOVE) {
            onCursorEvent(TouchEventType::CURSOR_MOVE, touchEvent->cursorIndex, touchEvent->buttonId, touchEvent->screenX, touchEvent->screenY);
        } else {
            throw LogicException(LOC);
        }

        touchListenerMove.eventsList.remove(0);
    }

    while(touchListenerDrag.eventsList.size()) {
        BufferedTouchListener::Entry* touchEvent = touchListenerDrag.eventsList.getPointer(0);

        if (touchEvent->type == TouchEventType::CURSOR_DRAG) {
            onCursorEvent(TouchEventType::CURSOR_DRAG, touchEvent->cursorIndex, touchEvent->buttonId, touchEvent->screenX, touchEvent->screenY);
        } else {
            throw LogicException(LOC);
        }

        touchListenerDrag.eventsList.remove(0);
    }

    while(touchListenerButtons.eventsList.size()) {
        BufferedTouchListener::Entry* touchEvent = touchListenerButtons.eventsList.getPointer(0);

        if (touchEvent->type == TouchEventType::CURSOR_DOWN) {
            if (touchEvent->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
                onCursorEvent(TouchEventType::CURSOR_DOWN, touchEvent->cursorIndex, touchEvent->buttonId, touchEvent->screenX, touchEvent->screenY);
            }
        } else if (touchEvent->type == TouchEventType::CURSOR_UP) {
            if (touchEvent->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
                onCursorEvent(TouchEventType::CURSOR_UP, touchEvent->cursorIndex, touchEvent->buttonId, touchEvent->screenX, touchEvent->screenY);
            }
        } else if (touchEvent->type == TouchEventType::CURSOR_UP_OUTSIDE) {
            if (touchEvent->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
                onCursorEvent(TouchEventType::CURSOR_UP_OUTSIDE, touchEvent->cursorIndex, touchEvent->buttonId, touchEvent->screenX, touchEvent->screenY);
            }
        } else {
            throw LogicException(LOC);
        }

        touchListenerButtons.eventsList.remove(0);
    }
}

void GPMTouchInputHandler::isPlayEnabled_changedEvent() {
    super::isPlayEnabled_changedEvent();

    if(gw->getIsPlayEnabled()) {
        touchListenerMove.eventsList.clear();
        touchListenerDrag.eventsList.clear();
        touchListenerButtons.eventsList.clear();
    }
}

void GPMTouchInputHandler::createContent() {
    super::createContent();

    getVisualsMap()->putDirectB("touchAreaBgButtons", touchAreaBgButtons = VisualUtil2D::newTouchArea(
        gw->getHandler()->getAppAssets(),
        0, 0, base::ScreenUtil::screen->getWindowResolutionX(), base::ScreenUtil::screen->getWindowResolutionY(),
        0.0f, 0.0f,
        nullptr, 1.0f, 0, nullptr, 1.0f,
        layerTouchAreaUnder, nullptr
    ));
    touchAreaBgButtons->addTouchListener(&touchListenerButtons);
    touchAreaBgButtons->addTouchListener(&touchListenerDrag);

    getVisualsMap()->putDirectB("touchAreaBgMove", touchAreaBgMove = VisualUtil2D::newTouchArea(
        gw->getHandler()->getAppAssets(),
        0, 0, base::ScreenUtil::screen->getWindowResolutionX(), base::ScreenUtil::screen->getWindowResolutionY(),
        0.0f, 0.0f,
        nullptr, 0.5f, 0, nullptr, 0.5f,
        layerTouchAreaOver, nullptr
    ));
	touchAreaBgMove->addTouchListener(&touchListenerMove);
    touchAreaBgMove->setIsTouchFallthroughEnabled(true);
}

bool GPMTouchInputHandler::onCursorEvent(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, float screenX, float screenY) {
    if(screenTouch2DListener != nullptr) {
        screenTouch2DListener->onScreenTouch2D(touchEventType, cursorIndex, buttonId, screenX, screenY);
    }

    return false;
}

GPMTouchInputHandler::~GPMTouchInputHandler() {
    if(screenListener.getIsListening()) {
        base::ScreenUtil::screen->removeListener(&screenListener, 3);
    }
}
