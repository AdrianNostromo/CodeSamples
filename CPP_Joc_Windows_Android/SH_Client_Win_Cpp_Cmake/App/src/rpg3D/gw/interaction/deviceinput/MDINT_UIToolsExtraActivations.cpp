#include "MDINT_UIToolsExtraActivations.h"
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <rpg3D/gw/interaction/deviceinput/customSensor/SensorDataToolExtraActivation.h>

using namespace rpg3D;

MDINT_UIToolsExtraActivations::MDINT_UIToolsExtraActivations(base::IGameWorld* gw)
	: super(gw)
{
	uiToolExtraActivationListener.cb_onToolExtraActivation_listControl_Down_MainBtn = std::bind(&MDINT_UIToolsExtraActivations::uiTool_onExtraActivation_listControl_Down_MainBtn, this, std::placeholders::_1, std::placeholders::_2);
	uiToolExtraActivationListener.cb_onToolExtraActivation_secondaryControl_Down_MainBtn = std::bind(&MDINT_UIToolsExtraActivations::uiTool_onExtraActivation_secondaryControl_Down_MainBtn, this, std::placeholders::_1, std::placeholders::_2);
}

void MDINT_UIToolsExtraActivations::createBMain() {
	super::createBMain();

	gw->getComponentAs<IGamePartUserInterface*>(true/*mustExist*/)->setMainUiToolExtraActivationListener(&uiToolExtraActivationListener);
}

void MDINT_UIToolsExtraActivations::uiTool_onExtraActivation_listControl_Down_MainBtn(int toolInstanceId, int sourceId) {
	std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
		IManagedDeviceInput::CustomSensorTypes::ToolExtraActivation, SensorDataToolExtraActivation::Type::Data_ListControl_Down_MainBtn,
		new SensorDataToolExtraActivation(
			toolInstanceId, sourceId
		)
	);
	dispatch_onEntityControlInputs(sensorEvent);
}

void MDINT_UIToolsExtraActivations::uiTool_onExtraActivation_secondaryControl_Down_MainBtn(int toolInstanceId, int sourceId) {
	std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
		IManagedDeviceInput::CustomSensorTypes::ToolExtraActivation, SensorDataToolExtraActivation::Type::Data_SecondaryControl_Down_MainBtn,
		new SensorDataToolExtraActivation(
			toolInstanceId, sourceId
		)
	);
	dispatch_onEntityControlInputs(sensorEvent);
}

void MDINT_UIToolsExtraActivations::disposePre() {
	gw->getComponentAs<IGamePartUserInterface*>(true/*mustExist*/)->setMainUiToolExtraActivationListener(nullptr);

	super::disposePre();
}

MDINT_UIToolsExtraActivations::~MDINT_UIToolsExtraActivations() {
	//void
}
