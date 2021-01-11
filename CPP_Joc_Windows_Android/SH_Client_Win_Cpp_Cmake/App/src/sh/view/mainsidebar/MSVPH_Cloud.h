#pragma once

#include "MSV_PlayGamesStatus.h"

namespace sh {
class MSVPH_Cloud : public MSV_PlayGamesStatus {priv typedef MSV_PlayGamesStatus super; pub dCtor(MSVPH_Cloud);
public:
    explicit MSVPH_Cloud(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    virtual ~MSVPH_Cloud();
};
};
