#pragma once

#include <base/gh.h>
#include "MDINT_Base.h"

namespace rpg3D {
class MDINT_UIToolsExtraActivations : public MDINT_Base {priv typedef MDINT_Base super;pub dCtor(MDINT_UIToolsExtraActivations);
    priv IMainInGameUIViewListeners::CbListenerToolExtraActivation uiToolExtraActivationListener{};

    pub explicit MDINT_UIToolsExtraActivations(base::IGameWorld* gw);
    prot void createBMain() override;

    priv void uiTool_onExtraActivation_listControl_Down_MainBtn(int toolInstanceId, int sourceId);
    priv void uiTool_onExtraActivation_secondaryControl_Down_MainBtn(int toolInstanceId, int sourceId);

    prot void disposePre() override;
    pub ~MDINT_UIToolsExtraActivations() override;
};
};
