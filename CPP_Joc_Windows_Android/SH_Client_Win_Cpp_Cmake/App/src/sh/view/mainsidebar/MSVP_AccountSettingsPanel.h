#pragma once

#include "MSVPBody.h"

namespace sh {
class MSVP_AccountSettingsPanel : public MSVPBody {priv typedef MSVPBody super; pub dCtor(MSVP_AccountSettingsPanel);
public:
    explicit MSVP_AccountSettingsPanel(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    virtual ~MSVP_AccountSettingsPanel();
protected:
    virtual void onScreenTaTouchDown();
};
};
