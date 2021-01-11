#include <base/log/ILogHandler.h>
#include "TouchStage2D.h"
#include <base/log/ILogHandler.h>
#include <base/exceptions/LogicException.h>
#include <base/input/util/TouchEventType.h>
#include <graphics/visual2d/util/WrappedTouchListener2D.h>
#include <base/input/sensor/event/SensorEvent.h>
#include <base/input/sensor/data/SensorDataCursor.h>
#include <base/input/IInputHandler.h>
#include <base/event/listener/IEventListener.h>
#include <base/input/sensor/data/SensorDataInputHandler.h>
#include <graphics/environment/environment2D/IEnvironment2D.h>

TouchStage2D::TouchStage2D(graphics::IEnvironment2D* environment, IInputHandler* inputHandler)
  :super(environment),
   inputHandler(inputHandler)
{
	inputHandler->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorDown,
		std::bind(&TouchStage2D::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
	inputHandler->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorDownCanceled,
		std::bind(&TouchStage2D::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	inputHandler->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorUp,
		std::bind(&TouchStage2D::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	inputHandler->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorMoved,
		std::bind(&TouchStage2D::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
	inputHandler->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorMoveCanceled,
		std::bind(&TouchStage2D::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	inputHandler->addSensorListener(
		base::Touch::SensorType::InputHandler, base::SensorDataInputHandler::Type::EventsCacheBufferDispatchFinished,
		std::bind(&TouchStage2D::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

bool TouchStage2D::onCursorDown(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	float screenX, float screenY)
{
	base::CursorHandlingData::ButtonHD* cursorButtonHD = getCursorButtonHandlingData(cursorIndex, buttonId);

	if(cursorButtonHD->isPressed) {
		// This must be filtered in the device native cursor handling code.
		throw LogicException(LOC);
	}

	if(cursorButtonHD->syncChecker_down) {
		throw LogicException(LOC);
	}
	cursorButtonHD->syncChecker_down = true;

	// Save the tool type on the cursor info.
	cursorButtonHD->cursorHD->toolType = toolType;

	cursorButtonHD->isPressed = true;

	if(!getIsVisible()) {
		cursorButtonHD->syncChecker_down = false;

		// The Stage may never be hidden.
		throw LogicException(LOC);
	}

	cursorButtonHD->downPosRawScreen.set(screenX, screenY);

	Vector3 newDownPosW{ screenX, screenY , 0.0f };
	environment->peekCamera()->unproject(newDownPosW);
	cursorButtonHD->downPosW.set(newDownPosW.x, newDownPosW.y);

	// Also save the positions in the drag variables because buttonDownCancel needs the last position values and it is better to take the values from the drag variables.
	cursorButtonHD->cursorHD->posRawScreen.set(cursorButtonHD->downPosRawScreen);
	cursorButtonHD->cursorHD->posW.set(cursorButtonHD->downPosW);

	bool ret = worldTouchEvent(
		TouchEventType::CURSOR_DOWN,
		cursorIndex, cursorButtonHD->buttonId,
		cursorButtonHD->downPosW, nullptr/*deltaPosW*/,
		cursorButtonHD->cursorHD, cursorButtonHD,
		++cursorEventIndexCounter
	);

	cursorButtonHD->syncChecker_down = false;

	return ret;
}

bool TouchStage2D::onCursorDownCanceled(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	float screenX, float screenY)
{
	base::CursorHandlingData::ButtonHD* cursorButtonHD = getCursorButtonHandlingData(cursorIndex, buttonId);
	if(!cursorButtonHD->isPressed) {
		throw LogicException(LOC);
	}

	if(cursorButtonHD->syncChecker_downCanceled) {
		throw LogicException(LOC);
	}
	cursorButtonHD->syncChecker_downCanceled = true;

	// Save the tool type on the cursor info.
	cursorButtonHD->cursorHD->toolType = toolType;

	cursorButtonHD->downPosRawScreen.set(screenX, screenY);

	Vector3 newDownPosW{ screenX, screenY , 0.0f };
	environment->peekCamera()->unproject(newDownPosW);
	cursorButtonHD->downPosW.set(newDownPosW.x, newDownPosW.y);

	onCursorUp(
		cursorIndex, toolType, buttonId,
		cursorButtonHD->cursorHD->posRawScreen.x, cursorButtonHD->cursorHD->posRawScreen.y
	);

	cursorButtonHD->syncChecker_downCanceled = false;

	return true;
}

bool TouchStage2D::onCursorUp(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	float screenX, float screenY)
{
	base::CursorHandlingData::ButtonHD* cursorButtonHD = getCursorButtonHandlingData(cursorIndex, buttonId);
	if(cursorButtonHD->cursorHD->cursorIndex < 0) {
        throw LogicException(LOC);
	}
	if(cursorButtonHD->cursorHD->cursorIndex != cursorIndex) {
        throw LogicException(LOC);
	}

	// Threat as an ignorable error. This may occur on right click menus above the game area that catch a left mouse down and dissapear without catching the left mouse up. Handle touch_up event without a previous touch_down event.
	if(!cursorButtonHD->isPressed) {
		return true;
	}

	if(cursorButtonHD->syncChecker_up) {
		throw LogicException(LOC);
	}
	cursorButtonHD->syncChecker_up = true;

	// Save the tool type on the cursor info.
	cursorButtonHD->cursorHD->toolType = toolType;

	cursorButtonHD->isPressed = false;

	bool ret = false;

	cursorButtonHD->upPosRawScreen.set(screenX, screenY);

	Vector3 newUpPosW{ screenX, screenY , 0.0f };
	environment->peekCamera()->unproject(newUpPosW);
	cursorButtonHD->upPosW.set(newUpPosW.x, newUpPosW.y);

	bool b = worldTouchEvent(
		TouchEventType::CURSOR_UP,
		cursorIndex, cursorButtonHD->buttonId,
		cursorButtonHD->upPosW, nullptr/*deltaPosW*/,
		cursorButtonHD->cursorHD, cursorButtonHD,
		++cursorEventIndexCounter
	);
	if(b) {
		ret = true;
	}

	for (int j = 0; j < cursorButtonHD->cursorDownTargetsList.size(); j++) {
		IInteractiveDrawable2D* touchDownTarget = *cursorButtonHD->cursorDownTargetsList.getPointer(j);

		Vector2 localPos{};
		touchDownTarget->worldPosToLocalPos(
			cursorButtonHD->upPosW, localPos
		);

		for (int i = 0; i < touchDownTarget->getWrappedTouchListenersList()->size(); i++) {
			WrappedTouchListener2D* wrappedTouchListener = *touchDownTarget->getWrappedTouchListenersList()->getPointer(i);
			
			wrappedTouchListener->listener->onCursorUpOutside(
				touchDownTarget,
				cursorIndex, buttonId,
				cursorButtonHD->upPosW, localPos
			);
		}
		touchDownTarget->onCursorUpOutside(
			touchDownTarget,
			cursorIndex, buttonId,
			cursorButtonHD->upPosW, localPos
		);

		cursorButtonHD->removeCursorDownTarget(touchDownTarget);
		j--;
	}

	cursorButtonHD->cursorDownHookingTargetsList.clear();

	cursorButtonHD->syncChecker_up = false;

	return ret;
}

bool TouchStage2D::onCursorMoved(
	int cursorIndex, base::Touch::ToolType* toolType,
	float screenX, float screenY)
{
	base::CursorHandlingData* cursorHD = getCursorHandlingData(cursorIndex);

	cursorHD->moveEventsLoopTickIndex = moveEventsLoopTickIndexCounter;

	if(cursorHD->syncChecker_dragged) {
		throw LogicException(LOC);
	}
	cursorHD->syncChecker_dragged = true;

	// Save the tool type on the cursor info.
	cursorHD->toolType = toolType;

	cursorHD->posRawScreen.set(screenX, screenY);

	Vector3 newPosW(screenX, screenY, 0);
	environment->peekCamera()->unproject(newPosW);

	cursorHD->deltaPosW.set(
		newPosW.x - cursorHD->posW.x,
		newPosW.y - cursorHD->posW.y
	);

	cursorHD->posW.set(
		newPosW.x, 
		newPosW.y
	);
	
	bool ret = false;

	bool b = handlePressedCursorButtonsMove(cursorHD);
	if(b) {
		ret = b;
	}

	if(cursorHD->toolType == base::Touch::ToolType::Mouse) {
		int currentCursorEventIndex = ++cursorEventIndexCounter;

		// Handle move, over, out.
		b = worldTouchEvent(
			TouchEventType::CURSOR_MOVE,
			cursorIndex, base::Touch::ButtonCode::None,
			cursorHD->posW, &cursorHD->deltaPosW,
			cursorHD, nullptr,
			currentCursorEventIndex
		);
		if (b) {
			ret = true;
		}

		// Enumerate all cursor over targets and remove the ones that have an old move event index.
		for (int i = cursorHD->cursorOverTargetsList.size() - 1; i >= 0; i--) {
			IInteractiveDrawable2D *target = cursorHD->cursorOverTargetsList.getDirect(i);

			if (target->getCursorMoveEventIndex() != currentCursorEventIndex) {
				Vector2 localPos{};
				target->worldPosToLocalPos(cursorHD->posRawScreen, localPos);

				// Dispatch a cursor_out event on the target.
				callTouchListener(
					cursorHD, nullptr,
					TouchEventType::CURSOR_OUT, target->getWrappedTouchListenersList(), target,
					cursorIndex, base::Touch::ButtonCode::None,
					cursorHD->posW, localPos,
					nullptr/*deltaPosW*/,
					currentCursorEventIndex
				);
			}
		}
	}

	cursorHD->syncChecker_dragged = false;

	return ret;
}

bool TouchStage2D::onCursorMoveCanceled(
	int cursorIndex,
	float screenX, float screenY)
{
	// Enumerate all cursorOverTargetsList and remove all mouse_over targets.
	base::CursorHandlingData* cursorHD = getCursorHandlingData(cursorIndex);

	if(cursorHD->syncChecker_moveCanceled) {
		throw LogicException(LOC);
	}
	cursorHD->syncChecker_moveCanceled = true;

	// Save the tool type on the cursor info.
	// Set the tool type to none in this case because this was probably a mouse that got unplugged from a android device.
	cursorHD->toolType = base::Touch::ToolType::None;

	cursorHD->removeAllCursorOverTargets();

	cursorHD->syncChecker_moveCanceled = false;

	return true;
}

bool TouchStage2D::handlePressedCursorButtonsMove(base::CursorHandlingData* cursorHD) {
	bool ret = false;

	// Handle drags.
	for(int buttonIndex =0; buttonIndex <cursorHD->buttonsHandlingDataList.size(); buttonIndex++) {
		std::shared_ptr<base::CursorHandlingData::ButtonHD> cursorButtonHD = cursorHD->buttonsHandlingDataList.getDirect(buttonIndex);

		// Allow for cursorButtonHD to be nullptr because that occurs when RMB or MMB drag occurs and the previous cursorButtonHD are nullptr.
		if (cursorButtonHD == nullptr || !cursorButtonHD->isPressed) {
			continue;
		}

		if (!getIsVisible()) {
			cursorHD->syncChecker_dragged = false;

			// The Stage may never be hidden.
			throw LogicException(LOC);
		}

		for (int j = 0; j < cursorButtonHD->cursorDownHookingTargetsList.size(); j++) {
			IInteractiveDrawable2D *touchDownHookingTarget = *cursorButtonHD->cursorDownHookingTargetsList.getPointer(j);

			if (!touchDownHookingTarget->getIsInputInteractive()) {
				// The entry should have already been removed instantly when set to not_enabled.
				throw LogicException(LOC);
			}

			Vector2 localPos{};
			touchDownHookingTarget->worldPosToLocalPos(
				cursorHD->posW, localPos
			);

			for (int i = 0; i < touchDownHookingTarget->getWrappedTouchListenersList()->size(); i++) {
				WrappedTouchListener2D *wrappedTouchListener = *touchDownHookingTarget->getWrappedTouchListenersList()->getPointer(i);

				if (wrappedTouchListener->getIsListenerHookingTouchDown(cursorHD->cursorIndex)) {
					wrappedTouchListener->listener->onCursorDrag(
						touchDownHookingTarget,
						cursorHD->cursorIndex, cursorButtonHD->buttonId,
						cursorHD->posW, localPos,
						cursorHD->deltaPosW
					);

					ret = true;
				}
			}

			touchDownHookingTarget->onCursorDrag(
				touchDownHookingTarget,
				cursorHD->cursorIndex, cursorButtonHD->buttonId,
				cursorHD->posW, localPos, 
				cursorHD->deltaPosW
			);
		}
	}

	return ret;
}

void TouchStage2D::onCursorCacheLoopTickDispatchFinished() {
	if(getIsTouchBoxChanged()) {
		clearIsTouchBoxChanged();

		// Note. This will dispatch a move_event for each mouse if there was no move_event
		// received on the current events cache. This will allow visuals to receive mouse_move, mouse_over
		// and mouse_out events when the visuals move or are created and the mouse didn't move.
		// Enumerate all mouse cursors.
		for(int cursorIndex=0;cursorIndex<cursorHandlingDatasList.size();cursorIndex++) {
			std::shared_ptr<base::CursorHandlingData> cursorHD = cursorHandlingDatasList.getDirect(cursorIndex, nullptr);

			// If there was no move_event on the latest loop tick, force dispatch a move event.
			if(cursorHD->toolType == base::Touch::ToolType::Mouse && cursorHD->moveEventsLoopTickIndex != moveEventsLoopTickIndexCounter) {
				onCursorMoved(
					cursorIndex, cursorHD->toolType,
					cursorHD->posRawScreen.x, cursorHD->posRawScreen.y
				);
			}
		}
	}

	moveEventsLoopTickIndexCounter++;
}

void TouchStage2D::onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent) {
	if(sensorEvent->sensorType == base::Touch::SensorType::Cursor) {
		base::SensorDataCursor *sensorData = sensorEvent->data->castToSubtype<base::SensorDataCursor *>();

		if (sensorEvent->eventType == base::SensorDataCursor::Type::CursorDown) {
			if (sensorData->buttonId->index >= USER_INTERFACE_INTERRACTIONS_BUTTONS_COUNT) {
				// Only The first buttons (LMB, RMB, MMB) are u.i. interracting.
				return;
			}

			onCursorDown(
				sensorData->cursorIndex, sensorData->toolType, sensorData->buttonId,
				sensorData->screenPos.x, sensorData->screenPos.y
			);
		} else if (sensorEvent->eventType == base::SensorDataCursor::Type::CursorDownCanceled) {
			if (sensorData->buttonId->index >= USER_INTERFACE_INTERRACTIONS_BUTTONS_COUNT) {
				// Only The first buttons (LMB, RMB, MMB) are u.i. interracting.
				return;
			}

			onCursorDownCanceled(
				sensorData->cursorIndex, sensorData->toolType, sensorData->buttonId,
				sensorData->screenPos.x, sensorData->screenPos.y
			);
		} else if (sensorEvent->eventType == base::SensorDataCursor::Type::CursorUp) {
			if (sensorData->buttonId->index >= USER_INTERFACE_INTERRACTIONS_BUTTONS_COUNT) {
				// Only The first buttons (LMB, RMB, MMB) are u.i. interracting.
				return;
			}

			onCursorUp(
				sensorData->cursorIndex, sensorData->toolType, sensorData->buttonId,
				sensorData->screenPos.x, sensorData->screenPos.y
			);
		} else if (sensorEvent->eventType == base::SensorDataCursor::Type::CursorMoved) {
			onCursorMoved(
				sensorData->cursorIndex, sensorData->toolType,
				sensorData->screenPos.x, sensorData->screenPos.y
			);
		} else if (sensorEvent->eventType == base::SensorDataCursor::Type::CursorMoveCanceled) {
			onCursorMoveCanceled(
				sensorData->cursorIndex,
				sensorData->screenPos.x, sensorData->screenPos.y
			);
		} else {
			throw LogicException(LOC);
		}
	}

	else if(sensorEvent->sensorType == base::Touch::SensorType::InputHandler) {
		if (sensorEvent->eventType == base::SensorDataInputHandler::Type::EventsCacheBufferDispatchFinished) {
			onCursorCacheLoopTickDispatchFinished();
		} else {
			throw LogicException(LOC);
		}
	}

	else {
		throw LogicException(LOC);
	}
}

base::CursorHandlingData* TouchStage2D::getCursorHandlingData(int cursorIndex) {
	while(cursorHandlingDatasList.size() <= cursorIndex) {
		std::shared_ptr<base::CursorHandlingData> cursorHD = std::make_shared<base::CursorHandlingData>(cursorHandlingDatasList.size());
		cursorHandlingDatasList.appendReference(cursorHD);
		if(cursorHD->cursorIndex < 0) {
		    // This occurred for some reason.
            throw LogicException(LOC);
		}
	}

	std::shared_ptr<base::CursorHandlingData> cursorHD = cursorHandlingDatasList.getDirect(cursorIndex, nullptr);

	return cursorHD.get();
}

base::CursorHandlingData::ButtonHD* TouchStage2D::getCursorButtonHandlingData(int cursorIndex, base::Touch::ButtonCode* buttonId) {
	base::CursorHandlingData* cursorHD = getCursorHandlingData(cursorIndex);

    return cursorHD->getButtonHandlingData(buttonId);
}

void TouchStage2D::clearExistingTouchDownTarget(IInteractiveDrawable2D* entry, int cursorIndex, base::Touch::ButtonCode* buttonId) {
	base::CursorHandlingData::ButtonHD* cursorButtonHD = getCursorButtonHandlingData(cursorIndex, buttonId);

    if(!cursorButtonHD->isPressed) {
        throw LogicException(LOC);
    }

	cursorButtonHD->removeCursorDownTarget(entry);

    int i = cursorButtonHD->cursorDownHookingTargetsList.indexOf(entry);
    if(i >= 0) {
	    cursorButtonHD->cursorDownHookingTargetsList.remove(i);
    }
}

void TouchStage2D::clearExistingCursorOverTarget(IInteractiveDrawable2D* entry, int cursorIndex) {
	base::CursorHandlingData* cursorHD = getCursorHandlingData(cursorIndex);

	cursorHD->removeCursorOverTarget(entry);
}

TouchStage2D::~TouchStage2D() {
    //void
}
