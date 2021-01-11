#pragma once

#include <base/gh.h>
#include "A_Base.h"
#include <memory>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>
#include <base/list/IListEntry.h>
#include <base/MM.h>

namespace worldGame3D {
class A_Dependency : public A_Base {priv typedef A_Base super;pub dCtor(A_Dependency);
	// These are pre-injected actions that are used as dependencies.
	// This list is only used to release the dependencies if this is replaced.
	priv ListDL<std::shared_ptr<Dependency>> dependencyActionsList{LOC};
	// This is used by children dependency actions to also cancel the creator action when this getts replace_cancelled.
	// This can be a pointer because this is made to nullptr when the creator action clears it's dependencies when it finishes or gets replace_cancelled.
	priv ListDL<std::shared_ptr<Dependency>> dependantActionsList{LOC};

	pub explicit A_Dependency(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int groupsBitMap);

	pub int getDependenciesCount() final;

	pub void addDependencyAction(std::shared_ptr<IAction> _this, std::shared_ptr<IAction> dependencyAction, bool triggerFailOnFinishSuccess, bool triggerFailOnFail) final;
	pub void _appendDependencyDependant(std::shared_ptr<Dependency> dependant) final;
	priv void disconnectAllDependencyActions();

	prot void onRemoveFromActionsList_pre(int actionsStatus) override;

    pub ~A_Dependency() override;
};
};
