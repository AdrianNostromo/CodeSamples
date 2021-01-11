#pragma once

#include <base/gh.h>
#include "VH_ListMenuCardPopupView.h"

namespace base {
class VH_SnackbarView : public VH_ListMenuCardPopupView {priv typedef VH_ListMenuCardPopupView super;pub dCtor(VH_SnackbarView);
    pub explicit VH_SnackbarView(IApp* app);

    pub sp<base::snackbar::WrappedSnackbarView> getNewSnackbarViewAutoDisposable() override;

    pub ~VH_SnackbarView() override;
};
};
