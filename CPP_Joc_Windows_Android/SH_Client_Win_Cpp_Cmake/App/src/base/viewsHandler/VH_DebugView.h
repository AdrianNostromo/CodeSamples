#pragma once

#include <base/gh.h>
#include "VH_ColorSelectorView.h"

namespace base {
    class DebugView;
};

namespace base {
class VH_DebugView : public VH_ColorSelectorView {priv typedef VH_ColorSelectorView super;pub dCtor(VH_DebugView);
    priv base::DebugView* debugView = nullptr;

    pub explicit VH_DebugView(IApp* app);

    pub void initDebugView();
    pub base::IDebugView* getDebugView() final;

    pub ~VH_DebugView() override;
};
};
