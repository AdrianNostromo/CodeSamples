#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <worldGame3D/gw/interaction/IGPInteraction.h>
#include "GPUEC_SensorDeviceRotationOriented.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <base/math/Math.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/input/sensor/data/SensorDataRotationsPitchRollOriented.h>

using namespace rpg3D;

GPUEC_SensorDeviceRotationOriented::GPUEC_SensorDeviceRotationOriented(base::IGameWorld* gw)
    : super(gw)
{
	//void
}

bool GPUEC_SensorDeviceRotationOriented::processCacheInput(std::shared_ptr<base::SensorEvent>& input) {
	bool ret = super::processCacheInput(input);

	if (input->sensorType == base::Touch::SensorType::RotationsPitchRoll_Oriented) {
		ret = true;

		base::SensorDataRotationsPitchRollOriented* cSensorData = input->data->castToSubtype<base::SensorDataRotationsPitchRollOriented*>();
		
		processInput_RotationPitchRollOriented(cSensorData->pitchDeg, cSensorData->rollDeg_delta);
	}

	return ret;
}

void GPUEC_SensorDeviceRotationOriented::processInput_RotationPitchRollOriented(float pitchDeg, float rollDeg_delta) {
    filterTriggerEventIndexCounter++;

    base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3DElevation_Absolute, false);
    if(dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
        const float pitchOffset = 45.0f;
        dispatchInteraction_NotFiltered(
            rpg3D::WorldOrEntityInteractionEvent::getNew_ContinuousFocusView3DElevation_Absolute(
                filterTriggerEventIndexCounter,
                -pitchDeg + pitchOffset
            ),
            dispatcher, IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3DElevation_Absolute
        );
    }

    dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3DYaw_Delta, false);
    if(dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
        dispatchInteraction_NotFiltered(
            rpg3D::WorldOrEntityInteractionEvent::getNew_ContinuousFocusView3DYaw_Delta(
                filterTriggerEventIndexCounter,
                rollDeg_delta
            ),
            dispatcher, IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3DYaw_Delta
        );
    }
}

GPUEC_SensorDeviceRotationOriented::~GPUEC_SensorDeviceRotationOriented() {
	//void
}
