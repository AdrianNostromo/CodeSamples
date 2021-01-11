#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <base/list/IListEntry.h>
#include <base/list/ArrayList.h>
#include <base/list/ListDL.h>

namespace worldGame3D {
class IAction {
	pub class Status {
		pub static const int Active = 1;
		pub static const int Success = 2;
		// Replaced or something similar.
		pub static const int Failed = 3;
	};

	pub class CfgPreInjectedAction {
		pub std::shared_ptr<IAction> action;

		pub bool addAsDependency;
		pub bool addAsDependant;

		pub bool triggerDependencyFailOnFinishSuccess;
		pub bool triggerDependencyFailOnFail;

		pub CfgPreInjectedAction(
			std::shared_ptr<IAction> action,
			bool addAsDependency, bool addAsDependant,
			bool triggerDependencyFailOnFinishSuccess, bool triggerDependencyFailOnFail)
			 : action(action),
			addAsDependency(addAsDependency), addAsDependant(addAsDependant),
			triggerDependencyFailOnFinishSuccess(triggerDependencyFailOnFinishSuccess), triggerDependencyFailOnFail(triggerDependencyFailOnFail)
		{
			//void
		}
	};

	pub class Dependency {
		pub std::shared_ptr<IAction> dependantAction;
		pub IListEntry* dependantListEntry = nullptr;

		pub std::shared_ptr<IAction> dependencyAction;
		pub IListEntry* dependencyListEntry = nullptr;

		pub bool triggerFailOnFinishSuccess;
		pub bool triggerFailOnFail;

		pub Dependency(
			std::shared_ptr<IAction>& dependantAction, std::shared_ptr<IAction>& dependencyAction,
			bool triggerFailOnFinishSuccess, bool triggerFailOnFail)
			: dependantAction(dependantAction), dependencyAction(dependencyAction),
			triggerFailOnFinishSuccess(triggerFailOnFinishSuccess), triggerFailOnFail(triggerFailOnFail)
		{
			//void
		}

		pub void disconnect() {
			if (dependantListEntry == nullptr || dependencyListEntry == nullptr || dependantAction == nullptr || dependencyAction == nullptr || dependantAction.use_count() <= 0 || dependencyAction.use_count() <= 0) {
				throw LogicException(LOC);
			}

			dependantListEntry->remove();
			dependantListEntry = nullptr;

			dependantAction = nullptr;

			dependencyListEntry->remove();
			dependencyListEntry = nullptr;

			dependencyAction = nullptr;
		}
	};

	pub virtual int getType() = 0;
	pub virtual std::string getName() = 0;

	pub virtual bool getIsSyncedToServer() = 0;

	pub virtual bool getCanHaveNoListeners() = 0;

	pub virtual int getPersistenceDurationMS() = 0;
	pub virtual bool getIsFinished() = 0;

	pub virtual ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* getActionsListEntry() = 0;
	pub virtual void setActionsListEntry(ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* actionsListEntry) = 0;

	pub virtual float getTotalDeltaS() = 0;
	pub virtual int getTotalDeltaMS() = 0;
	pub virtual void setTotalDeltaTimes(int totalDeltaMS) = 0;

	pub virtual void incrementTicksCounter() = 0;

	pub virtual void setData(int deltaMS) = 0;
	pub virtual void clearData() = 0;

	pub virtual int getDependenciesCount() = 0;
	pub virtual void addDependencyAction(std::shared_ptr<IAction> _this, std::shared_ptr<IAction> dependencyAction, bool triggerFailOnFinishSuccess, bool triggerFailOnFail) = 0;
	pub virtual void _appendDependencyDependant(std::shared_ptr<Dependency> dependant) = 0;

	pub virtual int getGroupsBitMap() = 0;
	pub virtual int getReplaceGroupsBitMask() = 0;

	pub virtual int getOverridesGroupsBitMask() = 0;

	pub virtual int getPriority() = 0;

	pub virtual bool getIsReplaced() = 0;
	pub virtual void setIsReplaced(bool isReplaced) = 0;

	pub virtual ArrayList<std::shared_ptr<IAction>>* getDirectIndirectRemovedActionsList() = 0;
	pub virtual void pushDirectIndirectRemovedAction(std::shared_ptr<IAction> action, int actionsStatus) = 0;

	pub virtual void* getRawSubclassPointer(int validationType) = 0;

	pub virtual void removeFromActionsList(int actionsStatus) = 0;
	
	pub virtual ArrayList<CfgPreInjectedAction>* getChildDependencyActionsToPreInjectList() = 0;

	pub virtual bool getIsOverridden() = 0;

	// Only the first function should be called externally.
	pub virtual void addChildOverriddenAction(IAction* childOverriddenAction) = 0;
	// Dont call the next 3 functions externally. They should be protected but ... cpp.
	pub virtual void removeChildOverriddenAction(IAction* childOverriddenAction) = 0;
	pub virtual void addParentOverriderAction(IAction* parentOverriderAction) = 0;
	pub virtual void removeParentOverriderAction(IAction* parentOverriderAction) = 0;

	pub virtual int getPreRequirementsGroupsBitMask() = 0;

	pub virtual bool getIsPreRequirementsMet() = 0;
	pub virtual void connectPreRequirementAction(std::shared_ptr<IAction> preRequirementAction) = 0;
	pub virtual void disconnectPreRequirementAction(std::shared_ptr<IAction> preRequirementAction) = 0;
	pub virtual void disconnectAllPreRequirementActions() = 0;
	pub virtual void disconnectAllPreRequirerActions() = 0;
	pub virtual void _connectPreRequirerAction(std::shared_ptr<IAction> preRequirerAction) = 0;
	pub virtual void _disconnectPreRequirerAction(std::shared_ptr<IAction> preRequirerAction) = 0;

	pub virtual ~IAction() = default;
};
};
