#include "GPTBP_TurnControl.h"
#include <base/exceptions/LogicException.h>
#include <base/app/config/AppConfig.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include "util/ITurnBasedEntity.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

const bool GPTBP_TurnControl::IS_DEBUG_LOCAL = true;

GPTBP_TurnControl::RegisteredEntry::RegisteredEntry(
	GPTBP_TurnControl* handler,
	ITurnBasedEntity* turnBasedEntity)
	: handler(handler),
	turnBasedEntity(turnBasedEntity)
{
	//void
};

void GPTBP_TurnControl::RegisteredEntry::unregister() {
	handler->unregisterTurnBasedEntity(this);
}

ITurnBasedInteractor::ActionType GPTBP_TurnControl::RegisteredEntry::getCurrentActionType() {
	return currentActionType;
}

void GPTBP_TurnControl::RegisteredEntry::setCurrentActionType(ActionType currentActionType) {
	this->currentActionType = currentActionType;

	turnBasedEntity->onCurrentActionChanged();
}

bool GPTBP_TurnControl::RegisteredEntry::getIsCurrentActionFinshed() {
	return isCurrentActionFinshed;
}

void GPTBP_TurnControl::RegisteredEntry::currentActionFinished(ActionType finishedActionType) {
	if (currentActionType == AnyAction) {
		if (finishedActionType != Movement && finishedActionType != Ability && finishedActionType != None) {
			// Wrong action type got finished.
			throw LogicException(LOC);
		}
	} else if (currentActionType == Movement) {
		if (finishedActionType != Movement && finishedActionType != None) {
			// Wrong action type got finished.
			throw LogicException(LOC);
		}
	} else if (currentActionType == Ability) {
		if (finishedActionType != Ability && finishedActionType != None) {
			// Wrong action type got finished.
			throw LogicException(LOC);
		}
	} else {
		// Unahndled action type.
		throw LogicException(LOC);
	}

	if (isCurrentActionFinshed) {
		// Already received the finish notification.
		throw LogicException(LOC);
	}

	isCurrentActionFinshed = true;
}

GPTBP_TurnControl::GPTBP_TurnControl(
	base::IGameWorld* gw,
	TurnMode turnMode, sp<ArrayList<int>> turnBasedTeamsList)
	: super(
		gw,
		turnMode, turnBasedTeamsList
	)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_contentCreation->type,
		std::bind(&GPTBP_TurnControl::onGWEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

int GPTBP_TurnControl::getTurnIndex() {
	return turnIndex;
}

void GPTBP_TurnControl::incrementTurnIndex() {
	turnIndex++;

	gw->dispatchEvent(*GWEvent_turnChanged);
}

ListDL<sp<GPTBP_TurnControl::RegisteredEntry>>::Entry* GPTBP_TurnControl::GetNextTurnFocusEntity(ListDL<sp<RegisteredEntry>>::Entry* nextTurnFocusEntity, int turnTeamId, int turnIndex) {
	// nextTurnFocusEntity is nullptr only at the end.
	// nextTurnFocusEntity is always initialised correctly at the start of enumerations.
	ListDL<sp<GPTBP_TurnControl::RegisteredEntry>>::Entry* nextValidEntry = nullptr;

	for (ListDL<sp<GPTBP_TurnControl::RegisteredEntry>>::Entry* entry = nextTurnFocusEntity; entry != nullptr; entry = entry->next) {
		if ((entry->data->startsActingAfterTurnIndex < 0/*not_used*/ || entry->data->startsActingAfterTurnIndex < turnIndex/*test_passed*/)
			&& entry->data->turnBasedEntity->getTeam() == turnTeamId)
		{
			nextValidEntry = entry;

			break;
		}
	}

	return nextValidEntry;
}

ITurnBasedInteractor* GPTBP_TurnControl::registerTurnBasedEntity(ITurnBasedEntity* turnBasedEntity) {
	sp<RegisteredEntry> registeredEntry = new RegisteredEntry(this, turnBasedEntity);
	
	ListDL<sp<RegisteredEntry>>::Entry* newRegisteredEntry = registeredEntry->inHandler_listEntry = registeredTurnBasedEntities.appendReference(registeredEntry);

	if (!isTurnBasedPlayEnabled || turnTeamId != turnBasedEntity->getTeam()) {
		// The entity will act on this turn if the team is next in the teams ordering list.
		registeredEntry->startsActingAfterTurnIndex = -1;
	} else {
		// The team is already acting so the entity will start acting on the next turn.
		registeredEntry->startsActingAfterTurnIndex = turnIndex;
	}

	// Need special behaviour when close to turnFocusEntity and nextTurnFocusEntity;
	if (turnFocusEntity != nullptr) {
		// In case the insert occured right after turnFocusEntity, set nextTurnFocusEntity to the new one.
		if (turnFocusEntity->next == newRegisteredEntry) {
			nextTurnFocusEntity = newRegisteredEntry;
		}
	} else {
		// Do nothing here because there is no turnFocusEntity so it means the insert occured after a already processed entry and before the place where turnFocusEntity existed in the list.
		//if (newRegisteredEntry->next == nextTurnFocusEntity) {
		//	nextTurnFocusEntity = newRegisteredEntry;
		//}
	}

	return registeredEntry.get();
}

void GPTBP_TurnControl::unregisterTurnBasedEntity(RegisteredEntry* turnBasedInteractor) {
	if (turnBasedInteractor->handler == nullptr || turnBasedInteractor->inHandler_listEntry == nullptr) {
		// Not registered or logic error.
		throw LogicException(LOC);
	}
	turnBasedInteractor->handler = nullptr;
	
	if (turnBasedInteractor->inHandler_listEntry == turnFocusEntity) {
		// Removing the current entry.
		turnFocusEntity = nullptr;
	} else if (turnBasedInteractor->inHandler_listEntry == nextTurnFocusEntity) {
		// Removing the next entry.
		nextTurnFocusEntity = turnBasedInteractor->inHandler_listEntry->next;
	}

	turnBasedInteractor->inHandler_listEntry->remove();
	turnBasedInteractor->inHandler_listEntry = nullptr;
}

void GPTBP_TurnControl::onGWEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == base::IGameWorld::GWEvent_gameLoop_contentCreation->type) {
		if (turnMode == SeparateMoveAbilityTurns) {
			tickTurnState_SeparateMoveAbilityTurns(event.deltaS);
		} else if (turnMode == SingleActionTurn) {
			tickTurnState_SingleActionTurn(event.deltaS);
		} else {
			throw LogicException(LOC);
		}
	}

	else {
		throw LogicException(LOC);
	}
}

void GPTBP_TurnControl::tickTurnState_SeparateMoveAbilityTurns(float deltaS) {
	throw LogicException(LOC);
	// Implement when needed.

	//asd_x;
	//if (cachedTurnControlInputsList.count() > 0) {
	//	for (int i = 0; i < cachedTurnControlInputsList.count(); i++) {
	//		TurnControlInput& turnControlInput = cachedTurnControlInputsList.getReference(i);

	//		// This is for move, ability turn mode.
	//		if (turnMode == SeparateMoveAbilityTurns) {
	//			if (turnControlInput.type == TurnControlInput::Type::StartPlayLogic) {
	//				//GlobalAccessLogHandler::devLogHandler->post_debug("turnStateChanged.StartPlayLogic");

	//				if (turnState != PreStart) {
	//					throw LogicException(LOC);
	//				}

	//				setTurnState(TurnState::Movement_Pre);
	//			} else if (turnControlInput.type == TurnControlInput::Type::MoveStarted) {
	//				//GlobalAccessLogHandler::devLogHandler->post_debug("turnStateChanged.MoveStarted");

	//				if (turnState != Movement_Pre) {
	//					throw LogicException(LOC);
	//				}

	//				setTurnState(TurnState::Movement_WaitingToFinish);
	//			} else if (turnControlInput.type == TurnControlInput::Type::MoveSkipped) {
	//				//GlobalAccessLogHandler::devLogHandler->post_debug("turnStateChanged.MoveSkipped");

	//				if (turnState != Movement_Pre) {
	//					throw LogicException(LOC);
	//				}

	//				setTurnState(TurnState::Ability_Pre);
	//			} else if (turnControlInput.type == TurnControlInput::Type::MoveFinished) {
	//				//GlobalAccessLogHandler::devLogHandler->post_debug("turnStateChanged.MoveFinished");

	//				if (turnState != Movement_WaitingToFinish) {
	//					throw LogicException(LOC);
	//				}

	//				setTurnState(TurnState::Ability_Pre);
	//			} else if (turnControlInput.type == TurnControlInput::Type::AbilityStarted) {
	//				//GlobalAccessLogHandler::devLogHandler->post_debug("turnStateChanged.AbilityStarted");

	//				if (turnState != Ability_Pre) {
	//					throw LogicException(LOC);
	//				}

	//				setTurnState(TurnState::Ability_WaitingToFinish);
	//			} else if (turnControlInput.type == TurnControlInput::Type::AbilitySkipped) {
	//				//GlobalAccessLogHandler::devLogHandler->post_debug("turnStateChanged.AbilitySkipped");

	//				if (turnState != Ability_Pre) {
	//					throw LogicException(LOC);
	//				}

	//				setTurnState(TurnState::Movement_Pre);
	//				incrementTurnIndex();
	//			} else if (turnControlInput.type == TurnControlInput::Type::ExtendedAbilityFinished) {
	//				//GlobalAccessLogHandler::devLogHandler->post_debug("turnStateChanged.ExtendedAbilityFinished");

	//				if (turnState != Ability_WaitingToFinish) {
	//					throw LogicException(LOC);
	//				}

	//				setTurnState(TurnState::Movement_Pre);
	//				incrementTurnIndex();
	//			} else {
	//				throw LogicException(LOC);
	//			}
	//		}
	//		
}

void GPTBP_TurnControl::tickTurnState_SingleActionTurn(float deltaS) {
	if (!isTurnBasedPlayEnabled) {
		return;
	}
	if (isTurnBasedPlayTickChanged) {
		// Do a restart.
		isTurnBasedPlayTickChanged = false;

		turnFocusEntity = nullptr;
		nextTurnFocusEntity = registeredTurnBasedEntities.getFirst();
		turnTeamIndexInTurnBasedTeamsList = 0;
		turnTeamId = turnBasedTeamsList->getDirect(turnTeamIndexInTurnBasedTeamsList);
	}

	if (turnFocusEntity == nullptr) {
		// Starting the team or the turnFocusEntity got unregistered.
		// Grab next entity for the current team.

		turnFocusEntity = GetNextTurnFocusEntity(nextTurnFocusEntity, turnTeamId, turnIndex);
		if (turnFocusEntity != nullptr) {
			if (turnFocusEntity->data->getCurrentActionType() != ITurnBasedInteractor::None || turnFocusEntity->data->isCurrentActionFinshed) {
				// The currentActionType should have the reseted value of None here.
				throw LogicException(LOC);
			}

			nextTurnFocusEntity = turnFocusEntity->next;
		} else {
			nextTurnFocusEntity = nullptr;
		}
	}

	if (turnFocusEntity != nullptr) {
		if (turnFocusEntity->data->getCurrentActionType() == ITurnBasedInteractor::None) {
			turnFocusEntity->data->setCurrentActionType(ITurnBasedInteractor::AnyAction);
		} else {
			if (turnFocusEntity->data->isCurrentActionFinshed) {
				turnFocusEntity->data->setCurrentActionType(ITurnBasedInteractor::None);
				turnFocusEntity->data->isCurrentActionFinshed = false;
				
				turnFocusEntity = GetNextTurnFocusEntity(nextTurnFocusEntity, turnTeamId, turnIndex);
				if (turnFocusEntity != nullptr) {
					if (turnFocusEntity->data->getCurrentActionType() != ITurnBasedInteractor::None || turnFocusEntity->data->isCurrentActionFinshed) {
						// The currentActionType should have the reseted value of None here.
						throw LogicException(LOC);
					}

					nextTurnFocusEntity = turnFocusEntity->next;
				} else {
					nextTurnFocusEntity = nullptr;
				}
			}
		}
	}

	if (turnFocusEntity == nullptr) {
		// Last entity in team finished it's actions, go to the next team.
		turnTeamIndexInTurnBasedTeamsList++;

		turnFocusEntity = nullptr;
		nextTurnFocusEntity = registeredTurnBasedEntities.getFirst();

		if (turnTeamIndexInTurnBasedTeamsList >= turnBasedTeamsList->count()) {
			turnTeamIndexInTurnBasedTeamsList = 0;
			incrementTurnIndex();
		}

		turnTeamId = turnBasedTeamsList->getDirect(turnTeamIndexInTurnBasedTeamsList);
	}
}

void GPTBP_TurnControl::setIsTurnBasedPlayEnabled(bool isTurnBasedPlayEnabled) {
	if (this->isTurnBasedPlayEnabled == isTurnBasedPlayEnabled) {
		// Must change this at good moments currently. If needed, remove this throw.
		throw LogicException(LOC);
	}

	this->isTurnBasedPlayEnabled = isTurnBasedPlayEnabled;
	this->isTurnBasedPlayTickChanged = true;
}

void GPTBP_TurnControl::disposeMain() {
	if (registeredTurnBasedEntities.count() > 0) {
		// The entities must be unregistered before this is disposed.
		// The player unit may be the only one still registered, make sure to disable it's turn based lBlock before a zone gets deactivated.
		throw LogicException(LOC);
	}

	super::disposeMain();
}

GPTBP_TurnControl::~GPTBP_TurnControl() {
	//void
}
