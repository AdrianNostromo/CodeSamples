#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/GWExtendable.h>

namespace rpg3D {
class GWBase : public base::GWExtendable {priv typedef base::GWExtendable super;pub dCtor(GWBase);
    pub explicit GWBase(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

    pub ~GWBase() override;
};
};
