#pragma once

#include <base/gh.h>
#include "VH_MainSidebarView.h"

namespace sh {
class ViewsHandler : public VH_MainSidebarView {priv typedef VH_MainSidebarView super;pub dCtor(ViewsHandler);
    pub explicit ViewsHandler(IAppSquareHeads* app);

    pub ~ViewsHandler() override;
};
};
