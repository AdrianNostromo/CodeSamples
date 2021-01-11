#pragma once

#include <base/gh.h>
#include "A_PreInject.h"

namespace worldGame3D {
class Action : public A_PreInject {priv typedef A_PreInject super;pub dCtor(Action);
    pub explicit Action(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int priority, 
		int groupsBitMap,
		int preRequirementsGroupsBitMask,
		int replaceGroupsBitMask,
		int overridesGroupsBitMask);

    pub ~Action() override;
};
};
