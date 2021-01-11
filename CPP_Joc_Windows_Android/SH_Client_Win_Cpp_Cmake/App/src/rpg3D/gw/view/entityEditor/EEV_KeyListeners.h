#pragma once

#include <base/gh.h>
#include "EEV_GeneralInterface.h"
#include <base/input/util/Keyboard.h>
#include <memory>

class IEventListener;

namespace rpg3D {
class EEV_KeyListeners : public EEV_GeneralInterface {priv typedef EEV_GeneralInterface super;pub dCtor(EEV_KeyListeners);
	priv int keyCode_closeEditor = base::Keyboard::KeyCodes::KEY_ESCAPE;

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	pub explicit EEV_KeyListeners(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	prot void onSelectedStateChanged() override;

	priv void onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent);

	pub ~EEV_KeyListeners() override;
};
};
