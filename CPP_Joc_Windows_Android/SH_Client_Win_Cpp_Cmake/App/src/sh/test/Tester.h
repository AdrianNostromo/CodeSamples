#pragma once

#include <base/gh.h>
#include "TBase.h"

class IEventListener;
namespace base {
    class LoopEvent;
};

namespace sh {
class Tester : public TBase {priv typedef TBase super;pub dCtor(Tester);
    //asdA1; priv float timeS = 0.0f;

    pub explicit Tester(IAppSquareHeads* app);

    prot void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) override;

    pub ~Tester() override;
};
};
