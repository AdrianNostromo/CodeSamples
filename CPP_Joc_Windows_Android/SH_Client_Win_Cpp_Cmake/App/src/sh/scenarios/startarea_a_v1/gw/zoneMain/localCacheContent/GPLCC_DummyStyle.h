#pragma once

#include <base/gh.h>
#include "GPLCC_PersistentEntitiesLoader.h"

namespace base {
    class GWEvent;
};

namespace startarea_a_v1 {
class GPLCC_DummyStyle : public GPLCC_PersistentEntitiesLoader {priv typedef GPLCC_PersistentEntitiesLoader super;pub dCtor(GPLCC_DummyStyle);
    pub explicit GPLCC_DummyStyle(base::IGameWorld* gw);

    prot void onPersistentEntitiesLoadFinished() override;

    priv void onGWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded(IEventListener& eventListener, base::GWEvent& event);

    pub ~GPLCC_DummyStyle() override;
};
};
