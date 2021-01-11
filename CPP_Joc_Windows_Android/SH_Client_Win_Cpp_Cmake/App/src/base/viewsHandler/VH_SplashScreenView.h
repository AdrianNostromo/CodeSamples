#pragma once

#include <base/gh.h>
#include "VH_SnackbarView.h"

namespace base {
class VH_SplashScreenView : public VH_SnackbarView {priv typedef VH_SnackbarView super;pub dCtor(VH_SplashScreenView);
    priv base::SplashScreenView* splashScreenView = nullptr;

    pub explicit VH_SplashScreenView(IApp* app);

    pub base::SplashScreenView* getSplashScreenView();
    pub void hideAndAutoDisposeSplashScreen();

    pub ~VH_SplashScreenView() override;
};
};
