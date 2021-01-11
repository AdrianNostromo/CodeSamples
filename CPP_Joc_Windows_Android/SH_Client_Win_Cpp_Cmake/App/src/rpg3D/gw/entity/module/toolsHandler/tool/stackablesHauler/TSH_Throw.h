#pragma once

#include <base/gh.h>
#include "TSH_Pickup.h"

namespace rpg3D {
class TSH_Throw : public TSH_Pickup {priv typedef TSH_Pickup super;pub dCtor(TSH_Throw);
	prot bool doThrowCarriedOnCarriedEntityTouchDown = true;

    pub explicit TSH_Throw(ToolConfigStackablesHauler* config);

	pub void initParam_doThrowCarriedOnCarriedEntityTouchDown(bool doThrowCarriedOnCarriedEntityTouchDown);

    pub ~TSH_Throw() override;
};
};
