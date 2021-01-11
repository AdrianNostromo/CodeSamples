#pragma once

#include <base/gh.h>
#include <base/viewsHandler/IViewsHandler.h>

namespace sh {
    class IMainSidebarView;
    class IStatusView;
};

namespace sh {
class IViewsHandler : public virtual base::IViewsHandler {priv typedef base::IViewsHandler super;
    pub static int TYPE;
    DEC_componentBasics();

    pub virtual IMainSidebarView* getOrCreateMainSidebarView() = 0;

    pub virtual IStatusView* getOrCreateStatusView() = 0;

    pub ~IViewsHandler() override = default;
};
};
