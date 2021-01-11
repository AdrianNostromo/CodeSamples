#pragma once

#include <base/gh.h>
#include "VH_Base.h"
#include <base/view/IUIView.h>
#include <base/list/ListDL.h>

namespace base {
class VH_ViewsAutoDispose : public VH_Base, virtual public IUIView::IHandlerAutoDisposal {priv typedef VH_Base super;pub dCtor(VH_ViewsAutoDispose);
    pub explicit VH_ViewsAutoDispose(IApp* app);

    pub void onRequestDisposeView(IComponent* target) final;

    pub ~VH_ViewsAutoDispose() override;
};
};
