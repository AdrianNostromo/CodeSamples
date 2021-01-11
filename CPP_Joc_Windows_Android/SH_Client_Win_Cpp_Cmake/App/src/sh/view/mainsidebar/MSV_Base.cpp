#include "MSV_Base.h"

using namespace sh;

int MSV_Base::COMMAND_NONE = 0;

MSV_Base::MSV_Base(
    IApp* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
    worldGame3D::IAppGame* appGame, IAppUtils* appUtils)
    : super(app, viewItemConfigs, viewEaseDurationsSList, nullptr/*handlerAutoDisposal*/),
      appGame(appGame), appUtils(appUtils)
{
    //void
}

std::string MSV_Base::computeStateFlags(std::string baseState) {
    std::string state = super::computeStateFlags(baseState);

    state += "screenDimmer_";

    return state;
}

MSV_Base::~MSV_Base() {
    //void
}
