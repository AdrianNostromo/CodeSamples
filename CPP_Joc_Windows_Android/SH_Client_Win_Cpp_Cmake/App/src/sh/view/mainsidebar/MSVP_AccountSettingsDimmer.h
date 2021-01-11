#pragma once

#include "MSVP_AccountSettingsPanel.h"

namespace sh {
class MSVP_AccountSettingsDimmer : public MSVP_AccountSettingsPanel {priv typedef MSVP_AccountSettingsPanel super; pub dCtor(MSVP_AccountSettingsDimmer);
public:
    explicit MSVP_AccountSettingsDimmer(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    virtual ~MSVP_AccountSettingsDimmer();
};
};
