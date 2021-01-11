#include <base/input/sensor/data/SensorDataCursor.h>
#include "MDIN_LockedCursor.h"
#include <rpg3D/gw/interaction/deviceinput/customSensor/SensorDataSensitizedLockedCursor.h>
#include <base/app/IAppMetrics.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

float MDIN_LockedCursor::mouseSensitivityMultiplier = 0.15f;

MDIN_LockedCursor::MDIN_LockedCursor(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void MDIN_LockedCursor::createMain() {
	super::createMain();

	//void
}

void MDIN_LockedCursor::createBMain() {
	super::createBMain();

	getBlockingInputLayer()->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorDown,
		std::bind(&MDIN_LockedCursor::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
	getBlockingInputLayer()->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorMoved,
		std::bind(&MDIN_LockedCursor::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
	getBlockingInputLayer()->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorDrag,
		std::bind(&MDIN_LockedCursor::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
	getBlockingInputLayer()->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorUp,
		std::bind(&MDIN_LockedCursor::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
	getBlockingInputLayer()->addSensorListener(
		base::Touch::SensorType::Cursor, base::SensorDataCursor::Type::LockedCursorDownCanceled,
		std::bind(&MDIN_LockedCursor::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
}

void MDIN_LockedCursor::onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent) {
	if(sensorEvent->sensorType == base::Touch::SensorType::Cursor) {
		base::SensorDataCursor* sensorData = sensorEvent->data->castToSubtype<base::SensorDataCursor*>();

		if(sensorEvent->eventType == base::SensorDataCursor::Type::LockedCursorDown) {
			onLockedCursorDown(
				sensorData->cursorIndex, sensorData->toolType, sensorData->buttonId,
				sensorData->screenPos, sensorData->lockedCursorSpace
			);
		}else if(sensorEvent->eventType == base::SensorDataCursor::Type::LockedCursorMoved) {
			onLockedCursorMoved(
				sensorData->cursorIndex, sensorData->toolType,
				sensorData->screenPos, sensorData->delta
			);
		}else if(sensorEvent->eventType == base::SensorDataCursor::Type::LockedCursorDrag) {
			onLockedCursorDrag(
				sensorData->cursorIndex, sensorData->toolType, sensorData->buttonId,
				sensorData->screenPos, sensorData->delta
			);
		}else if(sensorEvent->eventType == base::SensorDataCursor::Type::LockedCursorUp) {
			onLockedCursorUp(
				sensorData->cursorIndex, sensorData->toolType, sensorData->buttonId,
				sensorData->screenPos, sensorData->lockedCursorSpace
			);
		}else if(sensorEvent->eventType == base::SensorDataCursor::Type::LockedCursorDownCanceled) {
			onLockedCursorDownCanceled(
				sensorData->cursorIndex, sensorData->toolType, sensorData->buttonId,
				sensorData->screenPos, sensorData->lockedCursorSpace
			);
		}

		else {
			throw LogicException(LOC);
		}
	}else {
		throw LogicException(LOC);
	}
}

void MDIN_LockedCursor::onLockedCursorDown(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& lockedCursorSpace)
{
	if (buttonId == base::Touch::ButtonCode::LeftOrTouch || buttonId == base::Touch::ButtonCode::Right) {
		std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
			CustomSensorTypes::SensitizedLockedCursor, SensorDataSensitizedLockedCursor::Type::Data,
			SensorDataSensitizedLockedCursor::getNew_LockedCursorDown(
				cursorIndex, toolType, buttonId,
				screenPos
			)
		);
		dispatch_onEntityControlInputs(sensorEvent);
	}
}

void MDIN_LockedCursor::onLockedCursorMoved(
	int cursorIndex, base::Touch::ToolType* toolType,
	Vector2& screenPos, Vector2& delta)
{
	Vector2* screenDensityMultiplier = gw->getHandler()->getAppMetrics()->getScreen()->getScreenDensityMultiplier();
	Vector2 sensitisedDelta{
		(delta.x * mouseSensitivityMultiplier) / screenDensityMultiplier->x,
		(delta.y * mouseSensitivityMultiplier) / screenDensityMultiplier->y
	};
	std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
		CustomSensorTypes::SensitizedLockedCursor, SensorDataSensitizedLockedCursor::Type::Data,
		SensorDataSensitizedLockedCursor::getNew_LockedCursorMove(
			cursorIndex, toolType,
			screenPos, delta,
			sensitisedDelta
		)
	);
	dispatch_onEntityControlInputs(sensorEvent);
}

void MDIN_LockedCursor::onLockedCursorDrag(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& delta)
{
	Vector2 sensitisedDelta{delta.x * mouseSensitivityMultiplier, delta.y * mouseSensitivityMultiplier};
	std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
		CustomSensorTypes::SensitizedLockedCursor, SensorDataSensitizedLockedCursor::Type::Data,
		SensorDataSensitizedLockedCursor::getNew_LockedCursorDrag(
			cursorIndex, toolType, buttonId,
			screenPos, delta,
			sensitisedDelta
		)
	);
	dispatch_onEntityControlInputs(sensorEvent);
}

void MDIN_LockedCursor::onLockedCursorUp(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& lockedCursorSpace)
{
	if (buttonId == base::Touch::ButtonCode::LeftOrTouch || buttonId == base::Touch::ButtonCode::Right) {
		std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
			CustomSensorTypes::SensitizedLockedCursor, SensorDataSensitizedLockedCursor::Type::Data,
			SensorDataSensitizedLockedCursor::getNew_LockedCursorUp(
				cursorIndex, toolType, buttonId,
				screenPos
			)
		);
		dispatch_onEntityControlInputs(sensorEvent);
	}
}

void MDIN_LockedCursor::onLockedCursorDownCanceled(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& lockedCursorSpace)
{
	if (buttonId == base::Touch::ButtonCode::LeftOrTouch || buttonId == base::Touch::ButtonCode::Right) {
		std::shared_ptr<base::SensorEvent> sensorEvent = std::make_shared<base::SensorEvent>(
			CustomSensorTypes::SensitizedLockedCursor, SensorDataSensitizedLockedCursor::Type::Data,
			SensorDataSensitizedLockedCursor::getNew_LockedCursorUp(
				cursorIndex, toolType, buttonId,
				screenPos
			)
		);
		dispatch_onEntityControlInputs(sensorEvent);
	}
}

bool MDIN_LockedCursor::getIsCursorBlockedModeSupported() {
	return getBlockingInputLayer()->getIsCursorModeSupported(base::Touch::CursorMode::Locked);
}

void MDIN_LockedCursor::setIsCursorBlockedMode(bool isCursorBlockedMode) {
	if(!isCursorBlockedMode) {
		getBlockingInputLayer()->setCursorMode(base::Touch::CursorMode::Normal);
	}else {
		getBlockingInputLayer()->setCursorMode(base::Touch::CursorMode::Locked);
	}
}

void MDIN_LockedCursor::disposePre() {
	localAutoListenersList.clear();

	super::disposePre();
}

MDIN_LockedCursor::~MDIN_LockedCursor() {
	//void
}
