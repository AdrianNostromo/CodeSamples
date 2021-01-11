#pragma once

#include <base/gh.h>
#include "CT_Base.h"
#include <base/memory/SharedPointer.h>

namespace rpg3D {
class CT_Triggering : public CT_Base {priv typedef CT_Base super;pub dCtor(CT_Triggering);
	prot std::shared_ptr<Vector3> operating_worldPos = nullptr;
	prot std::shared_ptr<Vector3> operating_worldDir = nullptr;
	prot sp<Vector2> operating_worldPosXY = nullptr;

	priv bool isTriggering = false;
	priv float triggerDurationS = 0.0f;
	priv int triggerTickIndex = 0;
	priv bool isTriggerPointOfNoReturnReached = false;
	
	// This is persistent, don't reset it on trigger changes.
	priv int lastTrigger_turnBasedActionStateCounter = -1;
	
	priv float triggerCooldownS = 0.0f;
	
	pub explicit CT_Triggering(ToolConfigPersistentCooldown* config);

	pub virtual bool canStartTriggering();

	pub bool getIsTriggering();
	pub bool getIsTriggerPointOfNoReturnReached();
	pub int getLastTrigger_turnBasedActionStateCounter();
	prot void startNewTriggering();
	prot virtual void onTriggeringStart();
	// This is called when operation stop occurs.
	prot void cancelActiveTriggering();
	prot virtual void onActiveTriggeringCancelPre();
	// Return:
	// true: triggering still.
	// false: finished.
	prot bool managedTickActiveTriggering(float deltaS);
	prot virtual bool tickTriggerInProgress(float triggerDurationS, int tickIndex) = 0;

	prot void disconnectFromGamePartsPre() override;

	prot void chargeCooldown();
	// This clears the cooldown. This is used by turn based tools at tool activation finish.
	prot void zeroCooldownIfExists();
	
	prot virtual void onCooldownStateChanged();

	pub void onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) override;

	prot void onIsOperatingChanged(bool isOperating) override;

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;

    pub ~CT_Triggering() override;
};
};
