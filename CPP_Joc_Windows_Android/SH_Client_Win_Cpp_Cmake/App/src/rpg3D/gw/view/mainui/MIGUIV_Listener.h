#pragma once

#include <base/input/util/Keyboard.h>
#include "MIGUIVActiveTool.h"

namespace rpg3D {
class MIGUIV_Listener : public MIGUIVActiveTool {priv typedef MIGUIVActiveTool super;pub dCtor(MIGUIV_Listener);
    priv int keyCode_toggleCameraMode = base::Keyboard::KeyCodes::KEY_C;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit MIGUIV_Listener(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);
    
    prot void onSelectedStateChanged() override;

    priv void onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent);

    pub ~MIGUIV_Listener() override;
};
};
