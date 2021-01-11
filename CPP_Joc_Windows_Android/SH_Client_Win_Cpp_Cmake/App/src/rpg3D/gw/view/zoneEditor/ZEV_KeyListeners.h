#pragma once

#include <base/gh.h>
#include "ZEV_GeneralInterface.h"
#include <base/input/util/Keyboard.h>
#include <memory>

class IEventListener;
namespace base {
	class SensorEvent;
};

namespace rpg3D {
class ZEV_KeyListeners : public ZEV_GeneralInterface {priv typedef ZEV_GeneralInterface super;pub dCtor(ZEV_KeyListeners);
    priv int keyCode_closeEditor = base::Keyboard::KeyCodes::KEY_ESCAPE;

	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	pub explicit ZEV_KeyListeners(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	prot void onSelectedStateChanged() override;

	priv void onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent);

    pub ~ZEV_KeyListeners() override;
};
};
