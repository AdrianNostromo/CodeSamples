#include "MDIN_ScreenTouch2D.h"
#include <rpg3D/gw/interaction/deviceinput/customSensor/SensorDataLocalisedScreenCursor.h>
#include <base/input/util/TouchEventType.h>
#include <worldGame3D/gw/main/IGamePartMain.h>

using namespace rpg3D;

MDIN_ScreenTouch2D::MDIN_ScreenTouch2D(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

void MDIN_ScreenTouch2D::createMain() {
	super::createMain();

	screenTouch2DListener.cb_onScreenTouch2D = std::bind(&MDIN_ScreenTouch2D::onScreenTouch2D, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
}

void MDIN_ScreenTouch2D::createBMain() {
	super::createBMain();

	gw->getMain()->setScreenTouch2DListener(&screenTouch2DListener);
}

void MDIN_ScreenTouch2D::onScreenTouch2D(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, float screenX, float screenY) {
	if (touchEventType == TouchEventType::CURSOR_MOVE || (buttonId == base::Touch::ButtonCode::LeftOrTouch || buttonId == base::Touch::ButtonCode::Right)) {
		Vector2 screenPos{screenX, screenY};

		std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
			IManagedDeviceInput::CustomSensorTypes::LocalisedScreenCursor, SensorDataLocalisedScreenCursor::Type::Data,
			new SensorDataLocalisedScreenCursor(
				touchEventType, cursorIndex, buttonId,
				screenPos
			)
		);
		dispatch_onEntityControlInputs(sensorEvent);
	}
}

void MDIN_ScreenTouch2D::clearInput() {
	super::clearInput();

	//void
}

void MDIN_ScreenTouch2D::disposePre() {
	gw->getMain()->setScreenTouch2DListener(nullptr);

	super::disposePre();
}

MDIN_ScreenTouch2D::~MDIN_ScreenTouch2D() {
    //void
}
