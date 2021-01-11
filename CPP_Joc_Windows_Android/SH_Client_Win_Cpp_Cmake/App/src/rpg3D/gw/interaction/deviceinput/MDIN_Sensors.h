#pragma once

#include <base/gh.h>
#include "MDIN_LockedCursor.h"

namespace rpg3D {
class MDIN_Sensors : public MDIN_LockedCursor {priv typedef MDIN_LockedCursor super;pub dCtor(MDIN_Sensors);
    ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit MDIN_Sensors(base::IGameWorld* gw);
    prot void createMain() override;
    prot void createBMain() override;

    priv void onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent);

    pub bool getIsSensorSupported(base::Touch::SensorType* sensorId) override;
    pub void setIsSensorEnabled(base::Touch::SensorType* sensorId, bool isEnabled) override;

    prot void disposePre() override;
    pub ~MDIN_Sensors() override;
};
};
