#include "GCPP_Phases.h"
#include <base/exceptions/LogicException.h>
#include <base/app/config/AppConfig.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/event/GWEvent.h>
#include "util/IPhaseEntity.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/playPhases/util/TurnPhaseConfig.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/grid2DContent/IGPGrid2DContent.h>
#include <worldGame3D/gw/content/IGPContent.h>

using namespace rpg3D;
using namespace rpg3D::playPhases;

const bool GCPP_Phases::IS_DEBUG_LOCAL = true;

GCPP_Phases::GCPP_Phases(
	base::IGameWorld* gw,
	sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList)
	: super(
		gw,
		playPhaseConfigsList
	)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_contentCreation->type,
		std::bind(&GCPP_Phases::onGWEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GCPP_Phases::createBMain() {
	super::createBMain();

	gpGrid2DContent = gw->getComponentAs<rpg3D::IGPGrid2DContent*>(true/*mustExist*/);
}

int GCPP_Phases::getTurnIndex() {
	return turnIndex;
}

void GCPP_Phases::incrementTurnIndex() {
	turnIndex++;

	gw->dispatchEvent(*GWEvent_turnChanged);
}

ListDL<sp<GCPP_Phases::RegisteredEntry>>::Entry* GCPP_Phases::GetNextTurnFocusEntity(ListDL<sp<RegisteredEntry>>::Entry* nextTurnFocusEntity, int turnTeamId, int turnIndex) {
	// nextTurnFocusEntity is nullptr only at the end.
	// nextTurnFocusEntity is always initialised correctly at the start of enumerations.
	ListDL<sp<GCPP_Phases::RegisteredEntry>>::Entry* nextValidEntry = nullptr;

	for (ListDL<sp<GCPP_Phases::RegisteredEntry>>::Entry* entry = nextTurnFocusEntity; entry != nullptr; entry = entry->next) {
		if ((entry->data->startsActingAfterTurnIndex < 0/*not_used*/ || entry->data->startsActingAfterTurnIndex < turnIndex/*test_passed*/)
			&& entry->data->turnBasedEntity->getTeam() == turnTeamId)
		{
			nextValidEntry = entry;

			break;
		}
	}

	return nextValidEntry;
}

void GCPP_Phases::onGWEvent(IEventListener& eventListener, base::GWEvent& event) {
	if (event.type == base::IGameWorld::GWEvent_gameLoop_contentCreation->type) {
		if (isPlayPhasesEnabled) {
			float bakedDeltaS = 0;
			if (activePhaseIndex == -1) {
				setActivePhaseIndex(0);
				bakedDeltaS = 0;
			} else {
				bakedDeltaS = event.deltaS;
			}

			int newPhaseIndex = tickActivePhase(bakedDeltaS);
			if (activePhaseIndex != newPhaseIndex) {
				setActivePhaseIndex(newPhaseIndex);
			}
		}
	}

	else {
		throw LogicException(LOC);
	}
}

void GCPP_Phases::setActivePhaseIndex(int newPhaseIndex) {
	if (this->activePhaseIndex == newPhaseIndex) {
		throw LogicException(LOC);
	}

	if (activePhaseIndex >= 0) {
		onActivePhaseDeactivationPre();
	}

	activePhaseIndex = newPhaseIndex;
	activePhaseConfig = playPhaseConfigsList->getPointer(activePhaseIndex);
	userPhaseEndInputSpamSupressorS = 0.0f;
	isUserPhaseEndRequest = false;

	if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
		GlobalAccessLogHandler::devLogHandler->post_debug("PlayPhase changed to: %s.", activePhaseConfig != nullptr ? activePhaseConfig->name.c_str() : "nullptr");
	}

	if (activePhaseIndex >= 0) {
		onActivePhaseActivated();
	}
}

float asd_01_ct = 0.0f;

int GCPP_Phases::tickActivePhase(float deltaS) {
	if (activePhaseConfig->isUserEndable) {
		if (userPhaseEndInputSpamSupressorS < 0.2f) {
			userPhaseEndInputSpamSupressorS += deltaS;

			if (userPhaseEndInputSpamSupressorS >= 0.2f) {
				// Clear any spam requests that may have been received during the cooldown.
				isUserPhaseEndRequest = false;
			}
		} else {
			if (isUserPhaseEndRequest) {
				// User requested a phase end.
				return (activePhaseIndex + 1) % playPhaseConfigsList->count();
			}
		}
	} else {
		//asd_011;// implement;
		
		asd_01_ct += deltaS;
		if (asd_01_ct >= 5.0f) {
			asd_01_ct = 0.0f;

			return (activePhaseIndex + 1) % playPhaseConfigsList->count();
		}
	}

	return activePhaseIndex;
}

void GCPP_Phases::onActivePhaseDeactivationPre() {
	if (activePhaseIndex == 0) {
		gpGrid2DContent->hidePathableGCellsIndicator();
	} else if (activePhaseIndex == 1) {
		//void
	} else {
		throw LogicException(LOC);
	}
}

void GCPP_Phases::onActivePhaseActivated() {
	if (activePhaseIndex == 0) {
		Vector2Int gPos{};
		gpGrid2DContent->worldPosToGPos(*gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/)->getPos(), gPos);
		gpGrid2DContent->showPathableGCellsIndicator(gPos, -1/*drawRadius*/, true/*showStartGPosAsValidDestination*/);
	} else if (activePhaseIndex == 1) {
		//void
	} else {
		throw LogicException(LOC);
	}
}

void GCPP_Phases::setIsPlayPhasesEnabled(bool isPlayPhasesEnabled) {
	if (this->isPlayPhasesEnabled == isPlayPhasesEnabled) {
		// Must change this at good moments currently. If needed, remove this throw.
		throw LogicException(LOC);
	}

	this->isPlayPhasesEnabled = isPlayPhasesEnabled;
	activePhaseIndex = -1;
	activePhaseConfig = nullptr;
	userPhaseEndInputSpamSupressorS = 0.0f;
	isUserPhaseEndRequest = false;

	if (AppConfig::IS_DEBUG && IS_DEBUG_LOCAL) {
		if (!this->isPlayPhasesEnabled) {
			GlobalAccessLogHandler::devLogHandler->post_debug("PlayPhase deactivated while phase: %s.", activePhaseConfig != nullptr ? activePhaseConfig->name.c_str() : "nullptr");
		} else {
			GlobalAccessLogHandler::devLogHandler->post_debug("PlayPhase activated with phase: %s.", activePhaseConfig != nullptr ? activePhaseConfig->name.c_str() : "nullptr");
		}
	}
}

void GCPP_Phases::onUserInput_endCurrentPlayPhase() {
	isUserPhaseEndRequest = true;
}

void GCPP_Phases::disposeMain() {
	if (registeredTurnBasedEntities.count() > 0) {
		// The entities must be unregistered before this is disposed.
		// The player unit may be the only one still registered, make sure to disable it's turn based lBlock before a zone gets deactivated.
		throw LogicException(LOC);
	}

	super::disposeMain();
}

GCPP_Phases::~GCPP_Phases() {
	//void
}
