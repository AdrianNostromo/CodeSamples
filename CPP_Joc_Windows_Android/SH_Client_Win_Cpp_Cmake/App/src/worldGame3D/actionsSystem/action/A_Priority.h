#pragma once

#include <base/gh.h>
#include "A_DirectIndirectRemoves.h"

namespace worldGame3D {
class A_Priority : public A_DirectIndirectRemoves {priv typedef A_DirectIndirectRemoves super;pub dCtor(A_Priority);
	// This is used only to cancel this action from another if other->stoppedGroupsBitMask has any common bits with this->groupsBitMap.
	// This is probably used only for persistent actions.
	pub int priority; 

	pub explicit A_Priority(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int priority, 
		int groupsBitMap,
		int preRequirementsGroupsBitMask,
		int replaceGroupsBitMask);

	pub int getPriority() final;

    pub ~A_Priority() override;
};
};
