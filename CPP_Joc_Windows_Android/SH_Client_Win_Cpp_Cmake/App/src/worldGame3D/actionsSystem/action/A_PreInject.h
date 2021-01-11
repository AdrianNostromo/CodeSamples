#pragma once

#include <base/gh.h>
#include "A_Override.h"
#include <base/list/ArrayList.h>
#include <memory>

namespace worldGame3D {
class A_PreInject : public A_Override {priv typedef A_Override super;pub dCtor(A_PreInject);
	// If this contains entries, pre-inject logic is used. The actions are pre-injected and they and this are re-called on the current tick.
	pub ArrayList<CfgPreInjectedAction> childDependencyActionsToPreInjectList{};

	pub explicit A_PreInject(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int priority, 
		int groupsBitMap,
		int preRequirementsGroupsBitMask,
		int replaceGroupsBitMask,
		int overridesGroupsBitMask);

	pub ArrayList<CfgPreInjectedAction>* getChildDependencyActionsToPreInjectList() final;

    pub ~A_PreInject() override;
};
};
