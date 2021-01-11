#include "ZEV_KeyListeners.h"
#include <base/input/sensor/event/SensorEvent.h>
#include <base/input/sensor/data/SensorDataKeyboard.h>
#include <rpg3D/gw/view/entityEditor/util/IListenerEntityEditorView.h>
#include <base/input/util/IBlockingInputLayer.h>
#include <functional>
#include <rpg3D/gw/view/zoneEditor/util/IListenerZoneEditorView.h>

using namespace rpg3D;

ZEV_KeyListeners::ZEV_KeyListeners(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewEaseDurationsSList,
		appUtils
	)
{
	//void
}

void ZEV_KeyListeners::onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent) {
	if (sensorEvent->sensorType == base::Touch::SensorType::Keyboard && sensorEvent->eventType == base::SensorDataKeyboard::Type::Key) {
		base::SensorDataKeyboard* sensorData = sensorEvent->data->castToSubtype<base::SensorDataKeyboard*>();

		if (sensorData->keyCode == keyCode_closeEditor) {
			if (sensorData->actionType == base::Keyboard::ActionTypes::Press) {
				if (listener != nullptr) {
					listener->onBtn(this, IListenerEntityEditorView::ButtonIds::CLOSE);
				}
			} else {
				throw LogicException(LOC);
			}
		} else {
			throw LogicException(LOC);
		}
	}
}

void ZEV_KeyListeners::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		// Release the keyboard listener.
		localAutoListenersList.clear();
	} else {
		// Hook the keyboard listener.
		getBlockingStreamLayer()->addKeyboardKeySensorListener(
			keyCode_closeEditor, base::Keyboard::ActionTypes::Press,
			std::bind(&ZEV_KeyListeners::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);
	}
}

ZEV_KeyListeners::~ZEV_KeyListeners() {
	//void
}
