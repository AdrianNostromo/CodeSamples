#pragma once

#include "MSVPH_Cloud.h"

namespace sh {
class MSVPH_Offline : public MSVPH_Cloud {priv typedef MSVPH_Cloud super; pub dCtor(MSVPH_Offline);
public:
    explicit MSVPH_Offline(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    virtual ~MSVPH_Offline();
};
};
