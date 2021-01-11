#include <base/math/Math.h>
#include "MDIN_Move_Base.h"
#include <rpg3D/gw/interaction/deviceinput/customSensor/SensorDataDirectionalMovement.h>

using namespace rpg3D;

MDIN_Move_Base::MDIN_Move_Base(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void MDIN_Move_Base::onMoveInput() {
	bool isChange = false;

	float vX = keyboardDirVector.x + uiDPadDirVector.x;
	vX = Math::max(-1.0f, Math::min(1.0f, vX));
	if(vX != lastDispatchedMoveDirVector2D.x) {
		lastDispatchedMoveDirVector2D.x = vX;

		isChange = true;
	}

	float vY = keyboardDirVector.y + uiDPadDirVector.y;
	vY = Math::max(-1.0f, Math::min(1.0f, vY));
	if(vY != lastDispatchedMoveDirVector2D.y) {
		lastDispatchedMoveDirVector2D.y = vY;

		isChange = true;
	}

	if(isChange) {
		std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
			IManagedDeviceInput::CustomSensorTypes::DirectionalMovement, SensorDataDirectionalMovement::Type::Data,
			new SensorDataDirectionalMovement(
				lastDispatchedMoveDirVector2D
			)
		);
		dispatch_onEntityControlInputs(sensorEvent);
	}
}

MDIN_Move_Base::~MDIN_Move_Base() {
	//void
}
