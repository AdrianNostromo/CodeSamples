#pragma once

#include <base/gh.h>
#include "A_Dependency.h"

namespace worldGame3D {
class A_PreRequirements : public A_Dependency {priv typedef A_Dependency super;pub dCtor(A_PreRequirements);
	// For this to pass if not 0, there must ne any entry on the actions_list on the left side from this that has any of the required groups.
	// This is similar to a dependency but all the list left actions are checked for groups.
	priv int preRequirementsGroupsBitMask;

	priv ArrayList<std::shared_ptr<IAction>> preRequirementsActionsList{};
	priv ArrayList<std::shared_ptr<IAction>> preRequirersActionsList{};

    pub explicit A_PreRequirements(
		int type, std::string name,
		bool isSyncedToServer,
		int persistenceDurationMS,
		int groupsBitMap,
		int preRequirementsGroupsBitMask);

	pub int getPreRequirementsGroupsBitMask() final;

	pub bool getIsPreRequirementsMet() final;
	pub void connectPreRequirementAction(std::shared_ptr<IAction> preRequirementAction) final;
	pub void disconnectPreRequirementAction(std::shared_ptr<IAction> preRequirementAction) final;
	pub void disconnectAllPreRequirementActions() final;
	pub void disconnectAllPreRequirerActions() final;
	pub void _connectPreRequirerAction(std::shared_ptr<IAction> preRequirerAction) final;
	pub void _disconnectPreRequirerAction(std::shared_ptr<IAction> preRequirerAction) final;

	prot void onRemoveFromActionsList_pre(int actionsStatus) override;

    pub ~A_PreRequirements() override;
};
};
