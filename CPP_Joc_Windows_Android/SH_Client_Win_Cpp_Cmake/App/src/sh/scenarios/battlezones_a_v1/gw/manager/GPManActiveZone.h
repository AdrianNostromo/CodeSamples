#pragma once

#include <base/list/Array3D.h>
#include <base/visual3d/material/Visual3DModelMaterial.h>
#include "GPManAZMobs.h"
#include <sh/scenarios/battlezones_a_v1/gw/zone/ZoneTemplate.h>
#include <base/container/flags/ManagedFlags.h>

namespace battlezones_a_v1 {
class GPManActiveZone : public GPManAZMobs {priv typedef GPManAZMobs super;pub dCtor(GPManActiveZone);
	priv bool isDelayedZoneActivation = false;
	priv std::string delayedZoneActivation_zonesGroupId;
	priv std::string delayedZoneActivation_zoneName;
	priv std::shared_ptr<std::string> delayedZoneActivation_spawnTargetEntityId = nullptr;
	priv bool delayedZoneActivation_isZonePrecleared = false;

	priv bool isRegenerateZonesGroupFlag = true;

	pub explicit GPManActiveZone(base::IGameWorld* gw);

	priv void onGWEvent_gameLoop_post(IEventListener& eventListener, base::GWEvent& event);

	prot ZoneConfig* getActiveZoneConfig();
	prot void setActiveZone(
		std::string& zonesGroupId, std::string& zoneName,
		std::shared_ptr<std::string> spawnTargetEntityId,
		bool isZonePrecleared,
		bool activateOnLoopTick);

	prot void setRegenerateZonesGroupFlag();

	priv void createPlayerUnit(float x, float y, float z);

	priv void onEvent(IEventListener& eventListener, base::GWEvent& event);

	prot void disposePrePre() override;
	pub ~GPManActiveZone() override;
};
};
