#include "GPUEC_ToolExraActivation.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <rpg3D/gw/entity/module/userControlled/util/IWorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <rpg3D/gw/interaction/deviceinput/customSensor/SensorDataToolExtraActivation.h>
#include <graphics/camera/Camera.h>
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

GPUEC_ToolExraActivation::GPUEC_ToolExraActivation(base::IGameWorld* gw)
	: super(gw)
{
    //void
}

bool GPUEC_ToolExraActivation::processCacheInput(std::shared_ptr<base::SensorEvent>& input) {
	bool isProcessed = super::processCacheInput(input);
	
	if (input->sensorType == IManagedDeviceInput::CustomSensorTypes::ToolExtraActivation) {
		if (isProcessed) {
			throw LogicException(LOC);
		}
		
		isProcessed = true;

		rpg3D::SensorDataToolExtraActivation* cSensorData = input->data->castToSubtype<rpg3D::SensorDataToolExtraActivation*>();

		if (input->eventType == SensorDataToolExtraActivation::Type::Data_ListControl_Down_MainBtn) {
			base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_ListControl_Down_MainBtn, false);
			if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
				dispatchInteraction_NotFiltered(
					rpg3D::WorldOrEntityInteractionEvent::getNew_ToolExtraActivation_ListControl_Down_MainBtn(filterTriggerEventIndexCounter, cSensorData->toolInstanceId, cSensorData->sourceId),
					dispatcher, IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_ListControl_Down_MainBtn
				);
			}
		}else if (input->eventType == SensorDataToolExtraActivation::Type::Data_SecondaryControl_Down_MainBtn) {
			base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_SecondaryControl_Down_MainBtn, false);
			if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
				dispatchInteraction_NotFiltered(
					rpg3D::WorldOrEntityInteractionEvent::getNew_ToolExtraActivation_SecondaryControl_Down_MainBtn(filterTriggerEventIndexCounter, cSensorData->toolInstanceId, cSensorData->sourceId),
					dispatcher, IWorldOrEntityInteractionFilter::Type::ToolExtraActivation_SecondaryControl_Down_MainBtn
				);
			}
		} else {
			throw LogicException(LOC);
		}
	}

	return isProcessed;
}

GPUEC_ToolExraActivation::~GPUEC_ToolExraActivation() {
	//void
}
