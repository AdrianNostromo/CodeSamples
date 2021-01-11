#pragma once

#include <base/gh.h>
#include "A_PreRequirements.h"

namespace worldGame3D {
class A_Replace : public A_PreRequirements {priv typedef A_PreRequirements super;pub dCtor(A_Replace);
	pub int replaceGroupsBitMask;

	// This is set to true when another action replaces this.
	// This must be checked by all onAction_...().
	// The onAction_...() will be called only once with this value set to true for logic to handle action removal correctly.
	pub bool isReplaced = false;

	pub explicit A_Replace(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int groupsBitMap,
		int preRequirementsGroupsBitMask,
		int replaceGroupsBitMask);

	pub int getReplaceGroupsBitMask() final;

	pub bool getIsReplaced() final;
	pub void setIsReplaced(bool isReplaced) final;

    pub ~A_Replace() override;
};
};
