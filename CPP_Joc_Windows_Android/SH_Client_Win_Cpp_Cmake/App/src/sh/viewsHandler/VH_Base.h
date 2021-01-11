#pragma once

#include <base/gh.h>
#include <base/viewsHandler/ViewsHandler.h>
#include "IViewsHandler.h"

namespace sh {
    class IAppSquareHeads;
};

namespace sh {
class VH_Base : public base::ViewsHandler, public virtual IViewsHandler {priv typedef base::ViewsHandler super;pub dCtor(VH_Base);
    prot IAppSquareHeads* app;

    pub explicit VH_Base(IAppSquareHeads* app);

    pub ~VH_Base() override;
};
};
