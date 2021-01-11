#pragma once

#include <base/gh.h>
#include "VH_SplashScreenView.h"

namespace base {
class ViewsHandler : public VH_SplashScreenView {priv typedef VH_SplashScreenView super;pub dCtor(ViewsHandler);
    pub explicit ViewsHandler(IApp* app);

    pub ~ViewsHandler() override;
};
};
