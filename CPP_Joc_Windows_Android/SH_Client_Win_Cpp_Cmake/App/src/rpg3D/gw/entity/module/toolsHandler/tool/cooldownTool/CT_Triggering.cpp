#include "CT_Triggering.h"
#include <base/math/Math.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/config/ToolConfigPersistentCooldown.h>
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>
#include <base/log/GlobalAccessLogHandler.h>

using namespace rpg3D;

CT_Triggering::CT_Triggering(ToolConfigPersistentCooldown* config)
	: super(config)
{
	//void
}

bool CT_Triggering::canStartTriggering() {
	if (isTriggering || triggerCooldownS > 0) {
		return false;
	}

	return true;
}

bool CT_Triggering::getIsTriggering() {
	return isTriggering;
}

bool CT_Triggering::getIsTriggerPointOfNoReturnReached() {
	return isTriggerPointOfNoReturnReached;
}

int CT_Triggering::getLastTrigger_turnBasedActionStateCounter() {
	return lastTrigger_turnBasedActionStateCounter;
}

void CT_Triggering::startNewTriggering() {
	//GlobalAccessLogHandler::devLogHandler->post_debug("Tool trigger. Start new.");

	if (isTriggering) {
		// Already triggering in progress.
		throw LogicException(LOC);
	}
	triggerDurationS = 0.0f;
	triggerTickIndex = 0;
	isTriggerPointOfNoReturnReached = false;
	isTriggering = true;

	onTriggeringStart();
}

void CT_Triggering::onTriggeringStart() {
	//void
}

void CT_Triggering::cancelActiveTriggering() {
	//GlobalAccessLogHandler::devLogHandler->post_debug("Tool trigger. Cancelled.");

	if (!isTriggering) {
		// No triggering in progress.
		throw LogicException(LOC);
	}

	onActiveTriggeringCancelPre();

	isTriggering = false;
	triggerDurationS = 0.0f;
	triggerTickIndex = 0;
	isTriggerPointOfNoReturnReached = false;

	// Not sure if needed but stop the operation is probably correct here (probably already stopped or will stop after the current function returns).
	if (getIsOperatingAny()) {
		setIsOperating(false, getOperating_cursorIndex());
	}
}

void CT_Triggering::onActiveTriggeringCancelPre() {
	//void
}

bool CT_Triggering::managedTickActiveTriggering(float deltaS) {
	triggerDurationS += deltaS;

	bool isStillTriggering = tickTriggerInProgress(triggerDurationS, triggerTickIndex);
	triggerTickIndex++;
	if (!isStillTriggering) {
		// Triggering finished.
		//GlobalAccessLogHandler::devLogHandler->post_debug("Tool trigger. Finished.");

		isTriggering = false;
		
		// This is used only for turn_based mode to stop activation after a trigger finishes.
		// This also allows for another trigger to be done on the same turn if the previous one was cancelled before anything happened (eg. no projectiles launched).
		if (getIsOperatingAny() && (!config->supportsContinuousReactivation || (toolsHandler->getIsTurnBasedToolsModeActive() && isTriggerPointOfNoReturnReached))) {
			setIsOperating(false, getOperating_cursorIndex());
		}

		return false;
	}

	return true;
}

void CT_Triggering::disconnectFromGamePartsPre() {
	if (isTriggering) {
		cancelActiveTriggering();
	}

	super::disconnectFromGamePartsPre();
}

void CT_Triggering::chargeCooldown() {
	if (!isTriggerPointOfNoReturnReached) {
		// At this location, also set the trigger turn_index if turn_based logic is used.
		if (toolsHandler->getIsTurnBasedToolsModeActive()) {
			lastTrigger_turnBasedActionStateCounter = toolsHandler->getTurnBasedActionStateCounter();
		}
		isTriggerPointOfNoReturnReached = true;
	}

	triggerCooldownS = getLocalConfig()->cooldownS;

	onCooldownStateChanged();
}

void CT_Triggering::zeroCooldownIfExists() {
	if (triggerCooldownS > 0.0f) {
		triggerCooldownS = 0.0f;

		onCooldownStateChanged();
	}
}

void CT_Triggering::onCooldownStateChanged() {
	//void
}

void CT_Triggering::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (triggerCooldownS > 0.0f) {
		triggerCooldownS = Math::max(0.0f, triggerCooldownS - deltaS);

		if (triggerCooldownS <= 0) {
			onCooldownStateChanged();
		}
	}

	if (isTriggering) {
		managedTickActiveTriggering(deltaS);
	}
}

void CT_Triggering::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating && isTriggering) {
		// Don't stop the current trigger on operation stop (on mouse tap, a full attack should be done).
		//cancelActiveTriggering();
	}
}

void CT_Triggering::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	// Show or hide the sword visual. Keep the model in memory, just remove it from the display list.
	if (!isSelectedAndGWCapabilitiesExist) {
		if (isTriggering) {
			cancelActiveTriggering();
		}
	} else {
		//void
	}
}

CT_Triggering::~CT_Triggering() {
	//void
}
