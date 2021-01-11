#include <base/input/util/Keyboard.h>
#include <base/math/util/BoolUtil.h>
#include "NI_Keyboard.h"
#include <iostream>
#include <base/input/sensor/data/SensorDataKeyboard.h>
#include <base/input/sensor/event/SensorEvent.h>

NI_Keyboard* NI_Keyboard::globalRef = nullptr;

NI_Keyboard::NI_Keyboard(GLFWwindow* window)
    : super(window)
{
    globalRef = this;
}

void NI_Keyboard::createMain() {
    super::createMain();

    glfwSetKeyCallback(window, global_onKeyEvent);
}

void NI_Keyboard::global_onKeyEvent(GLFWwindow* window, int keyCode, int deviceSpecificScancode, int actionType, int modifierBits) {
    globalRef->onKeyEvent(window, keyCode, deviceSpecificScancode, actionType, modifierBits);
}

void NI_Keyboard::onKeyEvent(GLFWwindow* window, int keyCode, int deviceSpecificScancode, int actionType, int modifierBits) {
    if(!rawInputProcessor) {
        return ;
    }

    int sanitised_keyCode = keyCode;
    int sanitised_actionType;
    int sanitised_modifierBits = 0;

    if(actionType == GLFW_PRESS) {
        sanitised_actionType = base::Keyboard::ActionTypes::Press;
    }else if(actionType == GLFW_RELEASE) {
        sanitised_actionType = base::Keyboard::ActionTypes::Release;
    }else {
        return;
    }

    if((modifierBits & GLFW_MOD_SHIFT) != 0) {
        sanitised_modifierBits |= base::Keyboard::ModifierBitsTypes::Shift;
    }
    if((modifierBits & GLFW_MOD_CONTROL) != 0) {
        sanitised_modifierBits |= base::Keyboard::ModifierBitsTypes::Control;
    }
    if((modifierBits & GLFW_MOD_ALT) != 0) {
        sanitised_modifierBits |= base::Keyboard::ModifierBitsTypes::Alt;
    }
    if((modifierBits & GLFW_MOD_SUPER) != 0) {
        sanitised_modifierBits |= base::Keyboard::ModifierBitsTypes::Super;
    }

	std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
		base::Touch::SensorType::Keyboard, base::SensorDataKeyboard::Type::Key,
		new base::SensorDataKeyboard(
			sanitised_keyCode,
			sanitised_actionType,
			sanitised_modifierBits
		)
	);
	rawInputProcessor->onSensorEvent(sensorEvent);
}

NI_Keyboard::~NI_Keyboard() {
    //void
}
