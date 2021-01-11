#pragma once

#include <base/gh.h>
#include "AMBase.h"
#include <base/list/ArrayList.h>
#include <base/MM.h>

namespace worldGame3D {
class AMActionsList : public AMBase {priv typedef AMBase super;pub dCtor(AMActionsList);
	priv static const bool IS_DEBUG_ACTIONS;

	prot ListDL<std::shared_ptr<worldGame3D::IAction>> actionsList{LOC};
	//asd_x;// use this when server sync required.
	prot ListDL<std::shared_ptr<worldGame3D::IAction>> syncActionsList{LOC};

    pub explicit AMActionsList(
	    IWorldEntity* entity,
	    ActionsTemplate* t);

	// postEntry != nullptr; Insert before the post entry. It must be a existing list entry.
	// postEntry == nullptr; Append to the list.
	pub ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* addAction(
		std::shared_ptr<worldGame3D::IAction> newAction,
		ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* preinjectCreatorActionListEntry=nullptr,
		bool addAsDependencyToCreator=false, bool addAsDependantToCreator=false,
		bool triggerFailOnFinishSuccess=false, bool triggerFailOnFail=false) override;

	priv static ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* checkEntriesForReplaceCancelTargets(
		ListDL<std::shared_ptr<worldGame3D::IAction>>& actionsList,
		std::shared_ptr<worldGame3D::IAction>& newAction, ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* newListEntry);

	pub ~AMActionsList() override;

};
};
