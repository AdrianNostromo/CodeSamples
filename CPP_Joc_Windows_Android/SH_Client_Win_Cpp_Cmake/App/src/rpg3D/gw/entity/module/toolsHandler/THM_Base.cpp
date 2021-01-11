#include "THM_Base.h"
#include <rpg3D/gw/entity/template/ToolsHandlerTemplate.h>
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

THM_Base::THM_Base(
	IWorldEntity* entity,
	ToolsHandlerTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	//void
}

bool THM_Base::getIsAutomaticToolsEnabled() {
	if (isNonUserSelectableToolsEnabledIncremental >= 1) {
		return true;
	}
	
	return false;
}

void THM_Base::setIsNonUserSelectableToolsEnabledIncremental(bool isEnabled) {
	if (isNonUserSelectableToolsEnabledIncremental >= 1 && isEnabled) {
		// Enabled called too amny times. Must be called once for each disable.

		throw LogicException(LOC);
	}

	if (isNonUserSelectableToolsEnabledIncremental < -100 && !isEnabled) {
		// This is probably a logic bug. Disable called 100 times more than enabled.

		throw LogicException(LOC);
	}

	isNonUserSelectableToolsEnabledIncremental += isEnabled ? 1 : -1;

	if (!isEnabled && isNonUserSelectableToolsEnabledIncremental == 0) {
		// Just disabled.
		e->dispatchEvent(*EVENT_toolsHandler_nonUserSelectableToolsActivation_disabled);
	}else if(isEnabled && isNonUserSelectableToolsEnabledIncremental == 1) {
		// Just enabled.
		e->dispatchEvent(*EVENT_toolsHandler_nonUserSelectableToolsActivation_enabled);
	}
}

bool THM_Base::getIsTurnBasedToolsModeActive() {
	return isTurnBasedToolsModeActive;
}

void THM_Base::setIsTurnBasedToolsModeActive(bool isTurnBasedToolsModeActive) {
	this->isTurnBasedToolsModeActive = isTurnBasedToolsModeActive;
}

int THM_Base::getTurnBasedActionStateCounter() {
	return turnBasedActionStateCounter;
}

void THM_Base::incrementTurnBasedActionStateCounter() {
	turnBasedActionStateCounter++;
}

THM_Base::~THM_Base() {
	//void
}
