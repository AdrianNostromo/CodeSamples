#include "MIGUIV_Listener.h"
#include <base/input/sensor/event/SensorEvent.h>
#include <base/input/sensor/data/SensorDataKeyboard.h>

using namespace rpg3D;

MIGUIV_Listener::MIGUIV_Listener(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
    : super(app, viewItemConfigs, viewEaseDurationsSList, gw)
{
    //void
}

void MIGUIV_Listener::onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent) {
    if(sensorEvent->sensorType == base::Touch::SensorType::Keyboard && sensorEvent->eventType == base::SensorDataKeyboard::Type::Key) {
        base::SensorDataKeyboard* sensorData = sensorEvent->data->castToSubtype<base::SensorDataKeyboard*>();

        if(sensorData->keyCode == keyCode_toggleCameraMode) {
            if(sensorData->actionType == base::Keyboard::ActionTypes::Press) {
                handler->onRequestToggleCameraMode();
            }else {
                throw LogicException(LOC);
            }
        }else {
            throw LogicException(LOC);
        }
    }
}

void MIGUIV_Listener::onSelectedStateChanged() {
    super::onSelectedStateChanged();

    if (!isActive()) {
        // Release the keyboard listener.
        localAutoListenersList.clear();
    } else {
        // Hook the keyboard listener.
        getBlockingStreamLayer()->addKeyboardKeySensorListener(
            keyCode_toggleCameraMode, base::Keyboard::ActionTypes::Press,
            std::bind(&MIGUIV_Listener::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
            localAutoListenersList
        );
    }
}

MIGUIV_Listener::~MIGUIV_Listener() {
    //void
}
