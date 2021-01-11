#pragma once

#include <base/view/UIView.h>
#include "IMainSidebarView.h"

class IAppUtils;
namespace worldGame3D {
	class IAppGame;
};

namespace sh {
class MSV_Base : public UIView, virtual public IMainSidebarView {priv typedef UIView super; pub dCtor(MSV_Base);
protected:
    static int COMMAND_NONE;

    worldGame3D::IAppGame* appGame;
    IAppUtils* appUtils;

    int outCommandType = COMMAND_NONE;

public:
    explicit MSV_Base(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

    std::string computeStateFlags(std::string baseState) override;

    ~MSV_Base() override;
};
};
