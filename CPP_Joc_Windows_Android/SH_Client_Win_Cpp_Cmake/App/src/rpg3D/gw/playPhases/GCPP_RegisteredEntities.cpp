#include "GCPP_RegisteredEntities.h"
#include <base/exceptions/LogicException.h>
#include <base/app/config/AppConfig.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include "util/IPhaseEntity.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;
using namespace rpg3D::playPhases;

GCPP_RegisteredEntities::RegisteredEntry::RegisteredEntry(
	GCPP_RegisteredEntities* handler,
	IPhaseEntity* turnBasedEntity)
	: handler(handler),
	turnBasedEntity(turnBasedEntity)
{
	//void
};

void GCPP_RegisteredEntities::RegisteredEntry::unregister() {
	handler->unregisterTurnBasedEntity(this);
}

IPlayPhaseInteractor::ActionType GCPP_RegisteredEntities::RegisteredEntry::getCurrentActionType() {
	return currentActionType;
}

void GCPP_RegisteredEntities::RegisteredEntry::setCurrentActionType(ActionType currentActionType) {
	this->currentActionType = currentActionType;

	turnBasedEntity->onCurrentActionChanged();
}

bool GCPP_RegisteredEntities::RegisteredEntry::getIsCurrentActionFinshed() {
	return isCurrentActionFinshed;
}

void GCPP_RegisteredEntities::RegisteredEntry::currentActionFinished(ActionType finishedActionType) {
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

GCPP_RegisteredEntities::GCPP_RegisteredEntities(
	base::IGameWorld* gw,
	sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList)
	: super(
		gw,
		playPhaseConfigsList
	)
{
	//void
}

IPlayPhaseInteractor* GCPP_RegisteredEntities::registerTurnBasedEntity(IPhaseEntity* turnBasedEntity) {
	throw LogicException(LOC);
	//asd_011;// implement something.

	//sp<RegisteredEntry> registeredEntry = new RegisteredEntry(this, turnBasedEntity);

	//ListDL<sp<RegisteredEntry>>::Entry* newRegisteredEntry = registeredEntry->inHandler_listEntry = registeredTurnBasedEntities.appendReference(registeredEntry);

	//if (!isTurnBasedPlayEnabled || turnTeamId != turnBasedEntity->getTeam()) {
	//	// The entity will act on this turn if the team is next in the teams ordering list.
	//	registeredEntry->startsActingAfterTurnIndex = -1;
	//} else {
	//	// The team is already acting so the entity will start acting on the next turn.
	//	registeredEntry->startsActingAfterTurnIndex = turnIndex;
	//}

	//// Need special behaviour when close to turnFocusEntity and nextTurnFocusEntity;
	//if (turnFocusEntity != nullptr) {
	//	// In case the insert occured right after turnFocusEntity, set nextTurnFocusEntity to the new one.
	//	if (turnFocusEntity->next == newRegisteredEntry) {
	//		nextTurnFocusEntity = newRegisteredEntry;
	//	}
	//} else {
	//	// Do nothing here because there is no turnFocusEntity so it means the insert occured after a already processed entry and before the place where turnFocusEntity existed in the list.
	//	//if (newRegisteredEntry->next == nextTurnFocusEntity) {
	//	//	nextTurnFocusEntity = newRegisteredEntry;
	//	//}
	//}

	//return registeredEntry.get();
}

void GCPP_RegisteredEntities::unregisterTurnBasedEntity(RegisteredEntry* turnBasedInteractor) {
	throw LogicException(LOC);
	//asd_011;// implement something.
	
	//if (turnBasedInteractor->handler == nullptr || turnBasedInteractor->inHandler_listEntry == nullptr) {
	//	// Not registered or logic error.
	//	throw LogicException(LOC);
	//}
	//turnBasedInteractor->handler = nullptr;

	//if (turnBasedInteractor->inHandler_listEntry == turnFocusEntity) {
	//	// Removing the current entry.
	//	turnFocusEntity = nullptr;
	//} else if (turnBasedInteractor->inHandler_listEntry == nextTurnFocusEntity) {
	//	// Removing the next entry.
	//	nextTurnFocusEntity = turnBasedInteractor->inHandler_listEntry->next;
	//}

	//turnBasedInteractor->inHandler_listEntry->remove();
	//turnBasedInteractor->inHandler_listEntry = nullptr;
}

GCPP_RegisteredEntities::~GCPP_RegisteredEntities() {
	//void
}
