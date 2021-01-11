#include "MDIN_Sensors.h"
#include <base/input/sensor/event/SensorEvent.h>
#include <base/input/sensor/data/SensorDataRotationsPitchRollOriented.h>

using namespace rpg3D;

MDIN_Sensors::MDIN_Sensors(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void MDIN_Sensors::createMain() {
    super::createMain();

    //void
}

void MDIN_Sensors::createBMain() {
    super::createBMain();

    getBlockingInputLayer()->addSensorListener(
        base::Touch::SensorType::RotationsPitchRoll_Oriented, base::SensorDataRotationsPitchRollOriented::Type::Data,
        std::bind(&MDIN_Sensors::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

void MDIN_Sensors::onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent) {
    if(sensorEvent->eventType == base::SensorDataRotationsPitchRollOriented::Type::Data && sensorEvent->sensorType == base::Touch::SensorType::RotationsPitchRoll_Oriented) {
        dispatch_onEntityControlInputs(
            sensorEvent
        );
    }else {
        throw LogicException(LOC);
    }
}

bool MDIN_Sensors::getIsSensorSupported(base::Touch::SensorType* sensorId) {
    return getBlockingInputLayer()->getIsSensorSupported(sensorId);
}

void MDIN_Sensors::setIsSensorEnabled(base::Touch::SensorType* sensorId, bool isEnabled) {
    getBlockingInputLayer()->setIsSensorActive(sensorId, isEnabled);
}

void MDIN_Sensors::disposePre() {
    localAutoListenersList.clear();

    super::disposePre();
}

MDIN_Sensors::~MDIN_Sensors() {
	//void
}
