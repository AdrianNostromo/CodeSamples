#pragma once

#include "MSV_Base.h"

namespace sh {
class MSV_PlayGamesStatus : public MSV_Base {priv typedef MSV_Base super; pub dCtor(MSV_PlayGamesStatus);
public:
    explicit MSV_PlayGamesStatus(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    virtual ~MSV_PlayGamesStatus();
};
};
