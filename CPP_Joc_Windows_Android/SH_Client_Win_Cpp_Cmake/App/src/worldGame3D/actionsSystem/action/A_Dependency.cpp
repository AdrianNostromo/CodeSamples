#include "A_Dependency.h"

using namespace worldGame3D;

A_Dependency::A_Dependency(
	int type, std::string name,
	bool isSyncedToServer,
	int persistenceDurationMS,
	int groupsBitMap)
	: super(
		type, name,
		isSyncedToServer,
		persistenceDurationMS,
		groupsBitMap)
{
	//void
}

int A_Dependency::getDependenciesCount() {
	return dependencyActionsList.count();
}

void A_Dependency::addDependencyAction(std::shared_ptr<IAction> _this, std::shared_ptr<IAction> dependencyAction, bool triggerFailOnFinishSuccess, bool triggerFailOnFail) {
	// Create the dependency object.
	std::shared_ptr<Dependency> dependency = std::make_shared<Dependency>(
		_this, dependencyAction,
		triggerFailOnFinishSuccess, triggerFailOnFail
	);

	// Connect the dependency object to both actions.
	dependency->dependencyListEntry = dependencyActionsList.appendReference(dependency);
	dependencyAction->_appendDependencyDependant(dependency);
}

void A_Dependency::_appendDependencyDependant(std::shared_ptr<Dependency> dependant) {
	dependant->dependantListEntry = dependantActionsList.appendReference(dependant);
}

void A_Dependency::disconnectAllDependencyActions() {
	if (actionsListEntry == nullptr) {
		throw LogicException(LOC);
	}

	while (dependencyActionsList.count() > 0) {
		ListDL<std::shared_ptr<Dependency>>::Entry* dependencyActionListEntry = dependencyActionsList.getFirst();
		std::shared_ptr<Dependency> dependencyAction = dependencyActionListEntry->data;

		dependencyAction->disconnect();
	}
}

void A_Dependency::onRemoveFromActionsList_pre(int actionsStatus) {
	super::onRemoveFromActionsList_pre(actionsStatus);

	disconnectAllDependencyActions();

	while (dependantActionsList.count() > 0) {
		ListDL<std::shared_ptr<Dependency>>::Entry* dependantActionListEntry = dependantActionsList.getFirst();
		std::shared_ptr<Dependency> dependantAction = dependantActionListEntry->data;
		
		// Use this expanded check to also include unhandled cases error detection.
		if (actionsStatus == IAction::Status::Success) {
			if (dependantAction->triggerFailOnFinishSuccess) {
				pushDirectIndirectRemovedAction(dependantAction->dependantAction, actionsStatus);
			} else {
				dependantAction->disconnect();
			}
		}else if (actionsStatus == IAction::Status::Failed) {
			if (dependantAction->triggerFailOnFail) {
				pushDirectIndirectRemovedAction(dependantAction->dependantAction, actionsStatus);
			} else {
				dependantAction->disconnect();
			}
		} else {
			throw LogicException(LOC);
		}
	}
}

A_Dependency::~A_Dependency() {
	//void
}
