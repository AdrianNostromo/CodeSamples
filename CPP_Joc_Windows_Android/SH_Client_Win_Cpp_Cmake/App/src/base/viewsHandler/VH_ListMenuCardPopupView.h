#pragma once

#include <base/gh.h>
#include "VH_DialogView.h"

namespace base {
class VH_ListMenuCardPopupView : public VH_DialogView {priv typedef VH_DialogView super;pub dCtor(VH_ListMenuCardPopupView);
    pub explicit VH_ListMenuCardPopupView(IApp* app);

    pub base::IListMenuCardPopupView* getNewListMenuCardPopupViewAutoDisposable(IContainer2D* parentHolderOverride) override;

    pub ~VH_ListMenuCardPopupView() override;
};
};
