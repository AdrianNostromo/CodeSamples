#pragma once

#include <base/gh.h>
#include "VH_DebugView.h"

namespace base {
class VH_DialogView : public VH_DebugView {priv typedef VH_DebugView super;pub dCtor(VH_DialogView);
    pub explicit VH_DialogView(IApp* app);

    pub base::IDialogView* getNewDialogViewAutoDisposable() override;

    pub ~VH_DialogView() override;
};
};
