#include "EEV_KeyListeners.h"
#include <base/input/sensor/event/SensorEvent.h>
#include <base/input/sensor/data/SensorDataKeyboard.h>
#include <rpg3D/gw/view/entityEditor/util/IListenerEntityEditorView.h>

using namespace rpg3D;

EEV_KeyListeners::EEV_KeyListeners(
	IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
	IAppUtils* appUtils)
	: super(
		app, viewItemConfigs, viewEaseDurationsSList,
		appUtils
	)
{
	//void
}

void EEV_KeyListeners::onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent) {
	if(sensorEvent->sensorType == base::Touch::SensorType::Keyboard && sensorEvent->eventType == base::SensorDataKeyboard::Type::Key) {
		base::SensorDataKeyboard* sensorData = sensorEvent->data->castToSubtype<base::SensorDataKeyboard*>();

		if(sensorData->keyCode == keyCode_closeEditor) {
			if(sensorData->actionType == base::Keyboard::ActionTypes::Press) {
				if (listener != nullptr) {
					listener->onBtn(this, IListenerEntityEditorView::ButtonIds::CLOSE);
				}
			}else {
				throw LogicException(LOC);
			}
		}else {
			throw LogicException(LOC);
		}
	}
}

void EEV_KeyListeners::onSelectedStateChanged() {
	super::onSelectedStateChanged();

	if (!isActive()) {
		// Release the keyboard listener.
		localAutoListenersList.clear();
	} else {
		// Hook the keyboard listener.
		getBlockingStreamLayer()->addKeyboardKeySensorListener(
			keyCode_closeEditor, base::Keyboard::ActionTypes::Press,
			std::bind(&EEV_KeyListeners::onSensorEvent, this, std::placeholders::_1, std::placeholders::_2),
			localAutoListenersList
		);
	}
}

EEV_KeyListeners::~EEV_KeyListeners() {
	//void
}
