#pragma once

#include <base/gh.h>
#include "VH_ViewsAutoDispose.h"

namespace base {
class VH_ColorSelectorView : public VH_ViewsAutoDispose {priv typedef VH_ViewsAutoDispose super;pub dCtor(VH_ColorSelectorView);
    pub explicit VH_ColorSelectorView(IApp* app);

    pub base::IColorSelectorView* getNewColorSelectorViewAutoDisposable() override;

    pub ~VH_ColorSelectorView() override;
};
};
