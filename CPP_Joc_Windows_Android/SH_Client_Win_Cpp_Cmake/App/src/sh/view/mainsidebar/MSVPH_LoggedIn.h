#pragma once

#include "MSVPH_Connecting.h"

namespace sh {
class MSVPH_LoggedIn : public MSVPH_Connecting {priv typedef MSVPH_Connecting super; pub dCtor(MSVPH_LoggedIn);
public:
    explicit MSVPH_LoggedIn(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    virtual ~MSVPH_LoggedIn();
};
};
