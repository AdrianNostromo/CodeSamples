#pragma once

#include <base/gh.h>
#include "VH_Base.h"

namespace base {
    class LoopEvent;
};
namespace sh {
    class StatusView;
};

namespace sh {
class VH_StatusView : public VH_Base {priv typedef VH_Base super;pub dCtor(VH_StatusView);
    priv StatusView* statusView = nullptr;

    pub explicit VH_StatusView(IAppSquareHeads* app);
    prot void create() override;

    pub IStatusView* getOrCreateStatusView() override;

    priv void onLoopEvent_appLoop_General(IEventListener& eventListener, base::LoopEvent& event);

    pub ~VH_StatusView() override;
};
};
