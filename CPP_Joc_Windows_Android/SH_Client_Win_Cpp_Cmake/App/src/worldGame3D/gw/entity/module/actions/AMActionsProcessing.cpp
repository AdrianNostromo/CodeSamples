#include <base/math/util/BoolUtil.h>
#include "AMActionsProcessing.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace worldGame3D;

const bool AMActionsProcessing::IS_DEBUG_ACTIONS = true;

AMActionsProcessing::AMActionsProcessing(
	IWorldEntity* entity,
	ActionsTemplate* t)
	: super(entity, t)
{
	e->getGWEvents()->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_actionsProcessingSimulation->type,
		std::bind(&AMActionsProcessing::onGWEvent_gameLoop_actionsProcessingSimulation, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void AMActionsProcessing::onGWEvent_gameLoop_actionsProcessingSimulation(IEventListener& eventListener, base::GWEvent& event) {
	float gameTimeS = event.gameTimeS;
	float deltaS = event.deltaS;
	int deltaMS = event.deltaMS;

	ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* listEntry;
	actionsList.startManualEnumeration();
	actionsList.nextEnumeratingEntry = actionsList.getFirst();
	while ((listEntry = actionsList.nextEnumeratingEntry) != nullptr) {
		actionsList.nextEnumeratingEntry = actionsList.nextEnumeratingEntry->next;

		std::shared_ptr<worldGame3D::IAction> action = listEntry->data;

		if (action->getDirectIndirectRemovedActionsList()->count() > 0) {
			dispatchReplacedActionsAndClear(
				action->getDirectIndirectRemovedActionsList()
			);
		}

		// Compute localised delta times to not surpass the action timer.
		int localised_deltaMS;
		if (action->getPersistenceDurationMS() < 0) {
			// Infinite timer.
			localised_deltaMS = deltaMS;
		} else {
			// Limited timer.
			localised_deltaMS = Math::min(action->getPersistenceDurationMS() - action->getTotalDeltaMS(), deltaMS);
		}

		action->setTotalDeltaTimes(
			action->getTotalDeltaMS() + localised_deltaMS
		);
		
		action->setData(localised_deltaMS);
		dispatchAction(*action);
		action->clearData();

		if (action->getChildDependencyActionsToPreInjectList()->count() > 0) {
			// Handle actions pre-injecting and re-calling of actions.
			ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* firstPreInjectedActionEntry = nullptr;
			for (int i = 0; i < action->getChildDependencyActionsToPreInjectList()->count(); i++) {
				IAction::CfgPreInjectedAction* cfgActionToPreInject = action->getChildDependencyActionsToPreInjectList()->getPointer(i);

				ListDL<std::shared_ptr<worldGame3D::IAction>>::Entry* preInjectedActionListEntry = addAction(
					cfgActionToPreInject->action,
					listEntry, cfgActionToPreInject->addAsDependency, cfgActionToPreInject->addAsDependant,
					cfgActionToPreInject->triggerDependencyFailOnFinishSuccess, cfgActionToPreInject->triggerDependencyFailOnFail
				);

				// Set the next action to the earliest one that was not pre-replaced on pre-injection insertion.
				if (preInjectedActionListEntry != nullptr && firstPreInjectedActionEntry == nullptr) {
					firstPreInjectedActionEntry = preInjectedActionListEntry;
				}
			}
			
			action->getChildDependencyActionsToPreInjectList()->clear();

			if (firstPreInjectedActionEntry != nullptr) {
				// Because pre-injection occured and a previous action is the next one, "action" will be dispatched again. Need to remove the delta times from the action total delta times.
				action->setTotalDeltaTimes(
					action->getTotalDeltaMS() - localised_deltaMS
				);

				actionsList.nextEnumeratingEntry = firstPreInjectedActionEntry;
			}

			continue;
		}

		action->incrementTicksCounter();

		if (action->getIsSyncedToServer()) {
			//asd_x;// use this when server sync required.
			//syncActionsList.appendReference(action);
		}

		if (action->getIsFinished() || !action->getIsPreRequirementsMet()) {
			// Action finished normally, not replaced_cancelled.
			//GlobalAccessLogHandler::devLogHandler->post_debug("Persistent action-: %s", action->getName().c_str());

			action->removeFromActionsList(IAction::Status::Success);

			if (action->getDirectIndirectRemovedActionsList()->count() > 0) {
				// Call any dependants that require triggering when this dependency finishes_success.
				dispatchReplacedActionsAndClear(
					action->getDirectIndirectRemovedActionsList()
				);
			}
		}
	}
	actionsList.endManualEnumeration();
}

void AMActionsProcessing::dispatchReplacedActionsAndClear(
	ArrayList<std::shared_ptr<worldGame3D::IAction>>* replacedActionsList)
{
	if (replacedActionsList->count() > 0) {
		for (int i = 0; i < replacedActionsList->count(); i++) {
			std::shared_ptr<worldGame3D::IAction>& replacedAction = replacedActionsList->getReference(i);

			// This call order makes sure at least 1 child is called with replaced state before it's parent is.
			replacedAction->setData(0);
			dispatchAction(*replacedAction);
			replacedAction->clearData();
			if (replacedAction->getChildDependencyActionsToPreInjectList()->count() > 0) {
				// Replaced_cancelled actions should not request actions pre-injecting.
				throw LogicException(LOC);
			}
			if (replacedAction->getDirectIndirectRemovedActionsList()->count() > 0) {
				dispatchReplacedActionsAndClear(
					replacedAction->getDirectIndirectRemovedActionsList()
				);
			}
		}

		replacedActionsList->clear();
	}
}

AMActionsProcessing::~AMActionsProcessing() {
	//void
}
