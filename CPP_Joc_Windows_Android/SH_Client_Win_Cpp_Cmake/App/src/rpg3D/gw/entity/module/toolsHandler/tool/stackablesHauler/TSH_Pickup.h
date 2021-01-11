#pragma once

#include <base/gh.h>
#include "TSH_Base.h"

namespace rpg3D {
class TSH_Pickup : public TSH_Base {
private: typedef TSH_Base super;
public:
   pub dCtor(TSH_Pickup);
    pub explicit TSH_Pickup(ToolConfigStackablesHauler* config);

    pub ~TSH_Pickup() override;
};
};
