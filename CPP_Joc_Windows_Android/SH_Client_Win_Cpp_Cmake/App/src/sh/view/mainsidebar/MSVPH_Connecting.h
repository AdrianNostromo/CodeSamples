#pragma once

#include "MSVPH_Offline.h"

namespace sh {
class MSVPH_Connecting : public MSVPH_Offline {priv typedef MSVPH_Offline super; pub dCtor(MSVPH_Connecting);
public:
    explicit MSVPH_Connecting(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    virtual ~MSVPH_Connecting();
};
};
