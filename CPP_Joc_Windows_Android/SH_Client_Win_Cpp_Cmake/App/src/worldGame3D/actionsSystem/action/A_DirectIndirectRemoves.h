#pragma once

#include <base/gh.h>
#include "A_Replace.h"

namespace worldGame3D {
class A_DirectIndirectRemoves : public A_Replace {priv typedef A_Replace super;pub dCtor(A_DirectIndirectRemoves);
	// When this replaces an action, the remote action has isReplaced set to true and is moved from the actionsList to this list.
	pub ArrayList<std::shared_ptr<IAction>> directIndirectRemovedActionsList{};

    pub explicit A_DirectIndirectRemoves(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int groupsBitMap,
		int preRequirementsGroupsBitMask,
		int replaceGroupsBitMask);

	pub ArrayList<std::shared_ptr<IAction>>* getDirectIndirectRemovedActionsList() final;
	pub void pushDirectIndirectRemovedAction(std::shared_ptr<IAction> action, int actionsStatus) final;

    pub ~A_DirectIndirectRemoves() override;
};
};
