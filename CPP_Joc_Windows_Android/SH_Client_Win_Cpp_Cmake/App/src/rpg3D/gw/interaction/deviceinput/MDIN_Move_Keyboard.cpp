#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include "MDIN_Move_Keyboard.h"
#include <base/input/sensor/data/SensorDataKeyboard.h>

using namespace rpg3D;

MDIN_Move_Keyboard::MDIN_Move_Keyboard(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void MDIN_Move_Keyboard::createMain() {
	super::createMain();

	//void
}

void MDIN_Move_Keyboard::createBMain() {
	super::createBMain();

	for(int i=0;i<usedKeyCodesCount;i++) {
		int keyCode = usedKeyCodes[i];

		getBlockingInputLayer()->addKeyboardKeySensorListener(
			keyCode, base::Keyboard::ActionTypes::Press,
			std::bind(&MDIN_Move_Keyboard::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);
		getBlockingInputLayer()->addKeyboardKeySensorListener(
			keyCode, base::Keyboard::ActionTypes::Release,
			std::bind(&MDIN_Move_Keyboard::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);
	}
}

void MDIN_Move_Keyboard::onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent) {
	bool dirVectorChanged = false;

	if(sensorEvent->sensorType == base::Touch::SensorType::Keyboard && sensorEvent->eventType == base::SensorDataKeyboard::Type::Key) {
		base::SensorDataKeyboard* sensorData = sensorEvent->data->castToSubtype<base::SensorDataKeyboard*>();

		if(sensorData->keyCode == keyCode_left || sensorData->keyCode == keyCode_right) {
			bool isChange = false;

			if(sensorData->keyCode == keyCode_left) {
				if(sensorData->actionType == base::Keyboard::ActionTypes::Press) {
					if(!isKKeyPressed_moveLeft) {
						isKKeyPressed_moveLeft = true;
						lastPressDirection_leftRight = -1;

						isChange = true;
					}
				}else if(sensorData->actionType == base::Keyboard::ActionTypes::Release) {
					if(isKKeyPressed_moveLeft) {
						isKKeyPressed_moveLeft = false;

						isChange = true;
					}
				}else {
					throw LogicException(LOC);
				}
			}else if(sensorData->keyCode == keyCode_right) {
				if(sensorData->actionType == base::Keyboard::ActionTypes::Press) {
					if(!isKKeyPressed_moveRight) {
						isKKeyPressed_moveRight = true;
						lastPressDirection_leftRight = 1;

						isChange = true;
					}
				}else if(sensorData->actionType == base::Keyboard::ActionTypes::Release) {
					if(isKKeyPressed_moveRight) {
						isKKeyPressed_moveRight = false;

						isChange = true;
					}
				}else {
					throw LogicException(LOC);
				}
			}

			if(isChange) {
				// Dispatch.
				float v = 0.0f;

				if(isKKeyPressed_moveLeft && isKKeyPressed_moveRight) {
					v = lastPressDirection_leftRight;
				}else if(isKKeyPressed_moveLeft) {
					v = -1.0f;
				}else if(isKKeyPressed_moveRight) {
					v = 1.0f;
				}

				if(keyboardDirVector.x != v) {
					keyboardDirVector.x = v;

					dirVectorChanged = true;
				}
			}
		}else if(sensorData->keyCode == keyCode_back || sensorData->keyCode == keyCode_front) {
			bool isChange = false;

			if(sensorData->keyCode == keyCode_back) {
				if(sensorData->actionType == base::Keyboard::ActionTypes::Press) {
					if(!isKKeyPressed_moveBack) {
						isKKeyPressed_moveBack = true;
						lastPressDirection_backFront = -1;

						isChange = true;
					}
				}else if(sensorData->actionType == base::Keyboard::ActionTypes::Release) {
					if(isKKeyPressed_moveBack) {
						isKKeyPressed_moveBack = false;

						isChange = true;
					}
				}else {
					throw LogicException(LOC);
				}
			}else if(sensorData->keyCode == keyCode_front) {
				if(sensorData->actionType == base::Keyboard::ActionTypes::Press) {
					if(!isKKeyPressed_moveFront) {
						isKKeyPressed_moveFront = true;
						lastPressDirection_backFront = 1;

						isChange = true;
					}
				}else if(sensorData->actionType == base::Keyboard::ActionTypes::Release) {
					if(isKKeyPressed_moveFront) {
						isKKeyPressed_moveFront = false;

						isChange = true;
					}
				}else {
					throw LogicException(LOC);
				}
			}

			if(isChange) {
				// Dispatch.
				float v = 0.0f;

				if(isKKeyPressed_moveBack && isKKeyPressed_moveFront) {
					v = lastPressDirection_backFront;
				}else if(isKKeyPressed_moveBack) {
					v = -1.0f;
				}else if(isKKeyPressed_moveFront) {
					v = 1.0f;
				}

				if(keyboardDirVector.y != v) {
					keyboardDirVector.y = v;

					dirVectorChanged = true;
				}
			}
		}else {
			throw LogicException(LOC);
		}
	}else {
		throw LogicException(LOC);
	}

	if(dirVectorChanged) {
		onMoveInput();
	}
}

void MDIN_Move_Keyboard::clearInput() {
	super::clearInput();

	isKKeyPressed_moveLeft = false;
	isKKeyPressed_moveRight = false;

	isKKeyPressed_moveBack = false;
	isKKeyPressed_moveFront = false;
}

void MDIN_Move_Keyboard::disposePre() {
	localAutoListenersList.clear();

	super::disposePre();
}

MDIN_Move_Keyboard::~MDIN_Move_Keyboard() {
	//void
}
