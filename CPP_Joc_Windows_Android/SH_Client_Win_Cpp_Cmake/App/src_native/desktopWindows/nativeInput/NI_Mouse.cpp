#include <base/input/util/Touch.h>
#include "NI_Mouse.h"
#include <cmath>
#include <base/exceptions/LogicException.h>
#include <base/input/sensor/data/SensorDataCursor.h>
#include <base/input/sensor/event/SensorEvent.h>

const int NI_Mouse::MOUSE_CURSOR_INDEX = 0;

NI_Mouse::CursorHandlingData::CursorHandlingData() {
    //void
}

NI_Mouse::CursorHandlingData::ButtonHD* NI_Mouse::CursorHandlingData::getButtonHandlingData(base::Touch::ButtonCode* buttonId) {
    while(buttonsHandlingDataList.size() <= buttonId->index) {
        buttonsHandlingDataList.appendDirect(nullptr);
    }

	std::shared_ptr<ButtonHD> buttonHD = buttonsHandlingDataList.getDirect(buttonId->index, nullptr);
	if (buttonHD == nullptr) {
		buttonHD = *buttonsHandlingDataList.setDirect(buttonId->index, std::make_shared<ButtonHD>(this, buttonId));
	}

    return buttonHD.get();
}

NI_Mouse::CursorHandlingData::~CursorHandlingData() {
    //void
}

NI_Mouse::CursorHandlingData::ButtonHD::ButtonHD(CursorHandlingData* cursorHandlingData, base::Touch::ButtonCode* buttonId)
    : cursorHandlingData(cursorHandlingData), buttonId(buttonId)
{
    //void
}

NI_Mouse::CursorHandlingData::ButtonHD::~ButtonHD() {
    //void
}

NI_Mouse* NI_Mouse::globalRef = nullptr;

NI_Mouse::NI_Mouse(GLFWwindow* window)
    : super(window)
{
    globalRef = this;
}

void NI_Mouse::createMain() {
    super::createMain();

    // Do some tests:
    if(base::Touch::ButtonCode::LeftOrTouch->index != GLFW_MOUSE_BUTTON_LEFT || base::Touch::ButtonCode::Right->index != GLFW_MOUSE_BUTTON_RIGHT) {
        throw LogicException(LOC);
    }

    glfwSetCursorPosCallback(window, global_onMouseMoveEvent);
    glfwSetMouseButtonCallback(window, global_onMouseButtonEvent);
}

base::Touch::CursorMode* NI_Mouse::getCursorMode() {
    return cursorMode;
}

bool NI_Mouse::getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) {
	if (cursorMode == base::Touch::CursorMode::Normal || cursorMode == base::Touch::CursorMode::Locked) {
		return true;
	}

	return false;
}

void NI_Mouse::setCursorMode(base::Touch::CursorMode* _cursorMode) {
    if(this->cursorMode == _cursorMode) {
        return ;
    }

	if (!getIsCursorModeSupported(cursorMode)) {
		throw LogicException(LOC);
	}

	CursorHandlingData* cursorHD = getCursorHandlingData(MOUSE_CURSOR_INDEX);
	// Enumerate all buttons and dispatch the event.
	for (int buttonIndex = 0; buttonIndex < cursorHD->buttonsHandlingDataList.count(); buttonIndex++) {
		std::shared_ptr<CursorHandlingData::ButtonHD> cursorButtonHD = cursorHD->buttonsHandlingDataList.getDirect(buttonIndex);
		if (cursorButtonHD != nullptr && cursorButtonHD->isPressed) {
			// Do a button_press_cancel.
			cursorButtonHD->isPressed = false;

			if (this->cursorMode == base::Touch::CursorMode::Normal) {
				std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
					base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorDownCanceled,
					base::SensorDataCursor::getNew_CursorDownCanceled(
						MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, cursorButtonHD->buttonId,
						cursorHD->pos
					)
				);
				rawInputProcessor->onSensorEvent(sensorEvent);
			} else if (this->cursorMode == base::Touch::CursorMode::Locked) {
				Vector2 screenPos{screenSize.x / 2.0f, screenSize.y / 2.0f};
				std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
					base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorDownCanceled,
					base::SensorDataCursor::getNew_LockedCursorDownCanceled(
						MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, cursorButtonHD->buttonId,
						screenPos, cursorHD->pos
					)
				);
				rawInputProcessor->onSensorEvent(sensorEvent);
			} else {
				throw LogicException(LOC);
			}
		}
	}

    this->cursorMode = _cursorMode;

    // Change the mode.
    if(this->cursorMode == base::Touch::CursorMode::Normal) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }else if(this->cursorMode == base::Touch::CursorMode::Locked) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }else {
        throw LogicException(LOC);
    }

    // Update the mouse positions.
    double dX;
    double dY;
    glfwGetCursorPos(window, &dX, &dY);

    cursorHD->pos.set((float)dX, (float)dY);
}

void NI_Mouse::global_onMouseMoveEvent(GLFWwindow* window, double xpos, double ypos) {
    globalRef->onMouseMoveEvent(window, xpos, ypos);
}

void NI_Mouse::global_onMouseButtonEvent(GLFWwindow* window, int nativeButtonId, int action, int mods) {
	base::Touch::ButtonCode* buttonId = base::Touch::ButtonCode::IndexedButtons[nativeButtonId];

    globalRef->onMouseButtonEvent(window, buttonId, action, mods);
}

void NI_Mouse::onMouseMoveEvent(GLFWwindow* window, double xpos, double ypos) {
    if(!rawInputProcessor) {
        return ;
    }

    CursorHandlingData* cursorHD = getCursorHandlingData(MOUSE_CURSOR_INDEX);

    float oldX = cursorHD->pos.x;
    float oldY = cursorHD->pos.y;

    cursorHD->pos.set((float)xpos, (float)ypos);

    if(cursorMode == base::Touch::CursorMode::Normal) {
		std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
			base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorMoved,
			base::SensorDataCursor::getNew_CursorMoved(
				MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse,
				cursorHD->pos
			)
		);
		rawInputProcessor->onSensorEvent(sensorEvent);

		for (int buttonId=0; buttonId < cursorHD->buttonsHandlingDataList.count(); buttonId++) {
			std::shared_ptr<CursorHandlingData::ButtonHD> cursorButtonHD = cursorHD->buttonsHandlingDataList.getDirect(buttonId, nullptr);

			if (cursorButtonHD != nullptr && cursorButtonHD->isPressed) {
				std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
					base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorDrag,
					base::SensorDataCursor::getNew_CursorDrag(
						MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, cursorButtonHD->buttonId,
						cursorHD->pos
					)
				);
				rawInputProcessor->onSensorEvent(sensorEvent);
			}
		}
    }else if(cursorMode == base::Touch::CursorMode::Locked) {
		Vector2 screenPos{ screenSize.x / 2.0f, screenSize.y / 2.0f };
		Vector2 delta{ cursorHD->pos.x - oldX, -(cursorHD->pos.y - oldY) };
		std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
			base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorMoved,
			base::SensorDataCursor::getNew_LockedCursorMoved(
				MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse,
				screenPos, delta
			)
		);
		rawInputProcessor->onSensorEvent(sensorEvent);
		
		for (int buttonId = 0; buttonId < cursorHD->buttonsHandlingDataList.count(); buttonId++) {
			std::shared_ptr<CursorHandlingData::ButtonHD> cursorButtonHD = cursorHD->buttonsHandlingDataList.getDirect(buttonId, nullptr);

			if (cursorButtonHD != nullptr && cursorButtonHD->isPressed) {
				std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
					base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorDrag,
					base::SensorDataCursor::getNew_LockedCursorDrag(
						MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, cursorButtonHD->buttonId,
						screenPos,
						delta
					)
				);
				rawInputProcessor->onSensorEvent(sensorEvent);
			}
		}
    }else {
        throw LogicException(LOC);
    }
}

void NI_Mouse::onMouseButtonEvent(GLFWwindow* window, base::Touch::ButtonCode* _buttonId, int action, int mods) {
    if(!rawInputProcessor) {
        return ;
    }

	base::Touch::ButtonCode* buttonId = _buttonId;

    CursorHandlingData* cursorHD = getCursorHandlingData(MOUSE_CURSOR_INDEX);
    CursorHandlingData::ButtonHD* cursorButtonHD = cursorHD->getButtonHandlingData(buttonId);

    if (action == GLFW_PRESS) {
        if(cursorButtonHD->isPressed == true) {
            // Button is already in this state, cancel it.
            cursorButtonHD->isPressed = false;

			if (cursorMode == base::Touch::CursorMode::Normal) {
				std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
					base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorDownCanceled,
					base::SensorDataCursor::getNew_CursorDownCanceled(
						MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, buttonId,
						cursorHD->pos
					)
				);
				rawInputProcessor->onSensorEvent(sensorEvent);
			} else if (cursorMode == base::Touch::CursorMode::Locked) {
				Vector2 screenPos{ screenSize.x / 2.0f, screenSize.y / 2.0f };
				std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
					base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorDownCanceled,
					base::SensorDataCursor::getNew_LockedCursorDownCanceled(
						MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, buttonId,
						screenPos, cursorHD->pos
					)
				);
				rawInputProcessor->onSensorEvent(sensorEvent);
			} else {
				throw LogicException(LOC);
			}
        }

        cursorButtonHD->isPressed = true;

		if (cursorMode == base::Touch::CursorMode::Normal) {
			std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
				base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorDown,
				base::SensorDataCursor::getNew_CursorDown(
					MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, buttonId,
					cursorHD->pos
				)
			);
			rawInputProcessor->onSensorEvent(sensorEvent);
		} else if (cursorMode == base::Touch::CursorMode::Locked) {
			Vector2 screenPos{ screenSize.x / 2.0f, screenSize.y / 2.0f };
			std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
				base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorDown,
				base::SensorDataCursor::getNew_LockedCursorDown(
					MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, buttonId,
					screenPos, cursorHD->pos
				)
			);
			rawInputProcessor->onSensorEvent(sensorEvent);
		} else {
			throw LogicException(LOC);
		}
    } else if (action == GLFW_RELEASE) {
    	// Ignore button up events if the button is already in a up state.
	    if(cursorButtonHD->isPressed == true) {
            cursorButtonHD->isPressed = false;

			if (cursorMode == base::Touch::CursorMode::Normal) {
				std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
					base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::CursorUp,
					base::SensorDataCursor::getNew_CursorUp(
						MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, buttonId,
						cursorHD->pos
					)
				);
				rawInputProcessor->onSensorEvent(sensorEvent);
			} else if (cursorMode == base::Touch::CursorMode::Locked) {
				Vector2 screenPos{ screenSize.x / 2.0f, screenSize.y / 2.0f };
				std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
					base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorUp,
					base::SensorDataCursor::getNew_LockedCursorUp(
						MOUSE_CURSOR_INDEX, base::Touch::ToolType::Mouse, buttonId,
						screenPos, cursorHD->pos
					)
				);
				rawInputProcessor->onSensorEvent(sensorEvent);
			} else {
				throw LogicException(LOC);
			}
	    }
    }
}

NI_Mouse::CursorHandlingData* NI_Mouse::getCursorHandlingData(int cursorIndex) {
    while(cursorHandlingDataList.size() <= cursorIndex) {
        cursorHandlingDataList.appendDirect(nullptr);
    }

	std::shared_ptr<CursorHandlingData> cursorHD = cursorHandlingDataList.getDirect(cursorIndex, nullptr);
	if (cursorHD == nullptr) {
		cursorHD = *cursorHandlingDataList.setDirect(cursorIndex, std::make_shared<CursorHandlingData>());
	}
    
    return cursorHD.get();
}

NI_Mouse::~NI_Mouse() {
    //void
}
