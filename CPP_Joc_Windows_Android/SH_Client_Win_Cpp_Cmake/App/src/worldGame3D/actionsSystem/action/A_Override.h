#pragma once

#include <base/gh.h>
#include "A_Priority.h"

namespace worldGame3D {
class A_Override : public A_Priority {priv typedef A_Priority super;pub dCtor(A_Override);
	priv int overridesGroupsBitMask;

	// These 2 list are connected from one action to another in a method similar to dependencies.
	priv ArrayList<IAction*> childOverriddenActionsList{};
	priv ArrayList<IAction*> parentOverridersActionsList{};

    pub explicit A_Override(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int priority, 
		int groupsBitMap,
		int preRequirementsGroupsBitMask,
		int replaceGroupsBitMask,
		int overridesGroupsBitMask);

	pub int getOverridesGroupsBitMask() final;

	pub bool getIsOverridden() final;

	pub void addChildOverriddenAction(IAction* childOverriddenAction) final;
	prot void removeChildOverriddenAction(IAction* childOverriddenAction) final;
	priv void removeAllChildOverriddenActions();

	prot void addParentOverriderAction(IAction* parentOverriderAction) final;
	prot void removeParentOverriderAction(IAction* parentOverriderAction) final;
	priv void removeAllParentOverriderActions();

	prot void onRemoveFromActionsList_pre(int actionsStatus) override;

    pub ~A_Override() override;
};
};
