#pragma once

#include <base/gh.h>
#include "VH_StatusView.h"

namespace sh {
    class MainSidebarView;
};

namespace sh {
class VH_MainSidebarView : public VH_StatusView {priv typedef VH_StatusView super;pub dCtor(VH_MainSidebarView);
    priv MainSidebarView* mainSidebarView = nullptr;

    pub explicit VH_MainSidebarView(IAppSquareHeads* app);

    pub IMainSidebarView* getOrCreateMainSidebarView() override;

    pub ~VH_MainSidebarView() override;
};
};
