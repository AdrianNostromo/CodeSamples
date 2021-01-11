#include <base/math/util/BoolUtil.h>
#include "AMActionsList.h"
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>

using namespace worldGame3D;

const bool AMActionsList::IS_DEBUG_ACTIONS = false;

AMActionsList::AMActionsList(
	IWorldEntity* entity,
	ActionsTemplate* t)
	: super(entity, t)
{
	//void
}

ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* AMActionsList::addAction(
	std::shared_ptr<worldGame3D::IAction> newAction,
	ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* preinjectCreatorActionListEntry,
	bool addAsDependencyToCreator, bool addAsDependantToCreator,
	bool triggerFailOnFinishSuccess, bool triggerFailOnFail)
{
	if (AppConfig::IS_DEBUG && IS_DEBUG_ACTIONS) {
		GlobalAccessLogHandler::devLogHandler->post_debug("Persistent action+: %s", newAction->getName().c_str());
	}

	// Allow for pre-injecting logic to add the action before another one (that already exists in the list).
	ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* newListEntry;
	if (preinjectCreatorActionListEntry != nullptr) {
		newListEntry = actionsList.insertBeforeReference(preinjectCreatorActionListEntry, newAction);
		newAction->setActionsListEntry(newListEntry);
	} else {
		newListEntry = actionsList.appendReference(newAction);
		newAction->setActionsListEntry(newListEntry);
	}

	if (preinjectCreatorActionListEntry != nullptr) {
		// Set newAction as a dependency for creatorAction.
		if (addAsDependencyToCreator) {
			preinjectCreatorActionListEntry->data->addDependencyAction(preinjectCreatorActionListEntry->data, newAction, triggerFailOnFinishSuccess, triggerFailOnFail);
		}
		if (addAsDependantToCreator) {
			newAction->addDependencyAction(newAction, preinjectCreatorActionListEntry->data, triggerFailOnFinishSuccess, triggerFailOnFail);
		}
	}

	newListEntry = checkEntriesForReplaceCancelTargets(actionsList, newAction, newListEntry);

	return newListEntry;
}

ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* AMActionsList::checkEntriesForReplaceCancelTargets(
	ListDL<std::shared_ptr<worldGame3D::IAction>>& actionsList,
	std::shared_ptr<worldGame3D::IAction>& newAction, ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* newListEntry)
{
	// Check all previous actions, starting from the first one and cancel them and added them as cancelled_actions if the groups_bitMask check passes.
	ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* listEntry;
	actionsList.startManualEnumerationB();
	actionsList.nextEnumeratingEntryB = actionsList.getFirst();
	bool isNewActionListEntryPassed = false;
	while ((listEntry = actionsList.nextEnumeratingEntryB) != nullptr) {
		actionsList.nextEnumeratingEntryB = actionsList.nextEnumeratingEntryB->next;

		if (listEntry == newListEntry) {
			// Don'e check self entry.

			isNewActionListEntryPassed = true;

			continue;
		}

		std::shared_ptr<worldGame3D::IAction> listAction = listEntry->data;

		// Check pre_reqirements in both directions.
		if (newAction->getPreRequirementsGroupsBitMask() != 0 && newAction->getPreRequirementsGroupsBitMask() & listAction->getGroupsBitMap()) {
			newAction->connectPreRequirementAction(listAction);
		}
		if (listAction->getPreRequirementsGroupsBitMask() != 0 && listAction->getPreRequirementsGroupsBitMask() & newAction->getGroupsBitMap()) {
			listAction->connectPreRequirementAction(newAction);
		}

		if (newAction->getPriority() > listAction->getPriority()) {
			// New action has higher priority.

			if (newAction->getOverridesGroupsBitMask() & listAction->getGroupsBitMap()) {
				// Override list action.

				//GlobalAccessLogHandler::devLogHandler->post_debug("Overriding list action: %s by: %s", listAction->getName().c_str(), newAction->getName().c_str());

				newAction->addChildOverriddenAction(listAction.get());
			}

			if (newAction->getReplaceGroupsBitMask() & listAction->getGroupsBitMap()) {
				// Replace list action.

				//GlobalAccessLogHandler::devLogHandler->post_debug("Replacing list action: %s by: %s", listAction->getName().c_str(), newAction->getName().c_str());

				// The action from the list needs to be replace_cancelled.
				// Need to replace_cancel up until all parents are replace_canceled.
				// Also need to increment nextEnumeratingEntryB if that is the removed entry.
				newAction->pushDirectIndirectRemovedAction(listAction, IAction::Status::Failed);

				if (newAction->getIsReplaced()) {
					// Action got replace cancelled, skip the following checks.
					actionsList.endManualEnumerationB();
					return nullptr;
				}
			}
		} else if (newAction->getPriority() < listAction->getPriority()) {
			// List action has higher priority.
			// New action has lower priority.

			if (listAction->getOverridesGroupsBitMask() & newAction->getGroupsBitMap()) {
				// Override new action.

				//GlobalAccessLogHandler::devLogHandler->post_debug("Overriding new action: %s by: %s", newAction->getName().c_str(), listAction->getName().c_str());

				listAction->addChildOverriddenAction(newAction.get());
			}

			if (listAction->getReplaceGroupsBitMask() & newAction->getGroupsBitMap()) {
				// Replace new action.

				//GlobalAccessLogHandler::devLogHandler->post_debug("Replacing new action: %s by: %s", newAction->getName().c_str(), listAction->getName().c_str());

				listAction->pushDirectIndirectRemovedAction(newAction, IAction::Status::Failed);

				actionsList.endManualEnumerationB();
				return nullptr;
			}
		} else {
			// Equal priorities.

			if (!isNewActionListEntryPassed) {
				// On the left side of the new action.

				if (newAction->getOverridesGroupsBitMask() & listAction->getGroupsBitMap()) {
					// Override list action.

					//GlobalAccessLogHandler::devLogHandler->post_debug("Overriding list action: %s by: %s", listAction->getName().c_str(), newAction->getName().c_str());
					
					newAction->addChildOverriddenAction(listAction.get());

				} else if (listAction->getOverridesGroupsBitMask() & newAction->getGroupsBitMap()) {
					// Override new action.

					//GlobalAccessLogHandler::devLogHandler->post_debug("Overriding new action: %s by: %s", newAction->getName().c_str(), listAction->getName().c_str());

					listAction->addChildOverriddenAction(newAction.get());
				}

				if (newAction->getReplaceGroupsBitMask() & listAction->getGroupsBitMap()) {
					// Replace list action.

					//GlobalAccessLogHandler::devLogHandler->post_debug("Replacing list action: %s by: %s", listAction->getName().c_str(), newAction->getName().c_str());
					
					// The action from the list needs to be replace_cancelled.
					// Need to replace_cancel up until all parents are replace_canceled.
					// Also need to increment nextEnumeratingEntryB if that is the removed entry.
					newAction->pushDirectIndirectRemovedAction(listAction, IAction::Status::Failed);

					if (newAction->getIsReplaced()) {
						// Action got replace cancelled, skip the following checks.
						actionsList.endManualEnumerationB();
						return nullptr;
					}
				} else if (listAction->getReplaceGroupsBitMask() & newAction->getGroupsBitMap()) {
					// Replace new action.

					//GlobalAccessLogHandler::devLogHandler->post_debug("Replacing new action: %s by: %s", newAction->getName().c_str(), listAction->getName().c_str());

					listAction->pushDirectIndirectRemovedAction(newAction, IAction::Status::Failed);

					actionsList.endManualEnumerationB();
					return nullptr;
				}
			}else {
				// On te right side of the new action.

				if (listAction->getOverridesGroupsBitMask() & newAction->getGroupsBitMap()) {
					// Override new action.

					//GlobalAccessLogHandler::devLogHandler->post_debug("Overriding new action: %s by: %s", newAction->getName().c_str(), listAction->getName().c_str());
					
					listAction->addChildOverriddenAction(newAction.get());
				} else if (newAction->getOverridesGroupsBitMask() & listAction->getGroupsBitMap()) {
					// Override list action.

					//GlobalAccessLogHandler::devLogHandler->post_debug("Overriding list action: %s by: %s", listAction->getName().c_str(), newAction->getName().c_str());

					newAction->addChildOverriddenAction(listAction.get());
				}
				
				if (listAction->getReplaceGroupsBitMask() & newAction->getGroupsBitMap()) {
					// Replace new action.
					
					//GlobalAccessLogHandler::devLogHandler->post_debug("Replacing new action: %s by: %s", newAction->getName().c_str(), listAction->getName().c_str());
					
					listAction->pushDirectIndirectRemovedAction(newAction, IAction::Status::Failed);

					actionsList.endManualEnumerationB();
					return nullptr;
				} else if (newAction->getReplaceGroupsBitMask() & listAction->getGroupsBitMap()) {
					// Replace list action.

					//GlobalAccessLogHandler::devLogHandler->post_debug("Replacing list action: %s by: %s", listAction->getName().c_str(), newAction->getName().c_str());

					// The action from the list needs to be replace_cancelled.
					// Need to replace_cancel up until all parents are replace_canceled.
					// Also need to increment nextEnumeratingEntryB if that is the removed entry.
					newAction->pushDirectIndirectRemovedAction(listAction, IAction::Status::Failed);

					if (newAction->getIsReplaced()) {
						// Action got replace cancelled, skip the following checks.
						actionsList.endManualEnumerationB();
						return nullptr;
					}
				}
			}
		}
	}
	actionsList.endManualEnumerationB();

	return newListEntry;
}

AMActionsList::~AMActionsList() {
	//void
}
