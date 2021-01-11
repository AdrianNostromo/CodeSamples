#include "GPCZ_ActiveZone.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/list/Array2D.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <worldGame3D/gw/contentZones/zone/ContentZone.h>
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>

using namespace worldGame3D;

GPCZ_ActiveZone::GPCZ_ActiveZone(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_post->type,
		std::bind(&GPCZ_ActiveZone::onGWEvent_gameLoop_post, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

std::shared_ptr<ContentZone> GPCZ_ActiveZone::getActiveContentZone() {
	if (activeZone == nullptr) {
		throw LogicException(LOC);
	}

	return activeZone;
}

std::string GPCZ_ActiveZone::getActiveZoneName() {
	if (activeZone == nullptr) {
		return "";
	}

	return activeZone->getId();
}

void GPCZ_ActiveZone::setActiveZone(
	std::string zoneName, std::shared_ptr<worldGame3D::ZoneStartExtraData> zoneStartExtraData, bool restartZoneIfActive,
	bool useDelayedOnLoopActivation)
{
	if (useDelayedOnLoopActivation) {
		delayedZoneActivation_zoneName = zoneName;
		delayedZoneActivation_zoneStartExtraData = zoneStartExtraData;
		delayedZoneActivation_restartZoneIfActive = restartZoneIfActive;

		isDelayedZoneActivation = true;

		return;
	} else {
		isDelayedZoneActivation = false;
	}

	if (activeZone != nullptr && !restartZoneIfActive && activeZone->getId() == zoneName) {
		// Same zone requested and no restart.
		return;
	}

	if (activeZone != nullptr) {
		activeZone->deactivate(gw);
		activeZone = nullptr;
		isActiveZoneFinishedLoading = true;
	}

	if (!zoneName.empty()) {
		// Create the new zone.
		std::function<std::shared_ptr<worldGame3D::ContentZone>()> factoryFunction = gw->getScenarioTemplate()->zonesFactoriesMap.getDirect(zoneName, nullptr);
		if (factoryFunction == nullptr) {
			throw LogicException(LOC);
		}

		activeZone = factoryFunction();
		isActiveZoneFinishedLoading = false;

		handleMainUIViewSwitching(activeZone);

		activeZone->preActivate(zoneStartExtraData, gw);
		// Do 1 tick to finsih activation to avoid a 1 frame delay in certain cases.
		bool isActivationFinished = activeZone->finishActivation();
		if (isActivationFinished) {
			isActiveZoneFinishedLoading = true;
		}
	} else {
		handleMainUIViewSwitching(activeZone);
	}
}

void GPCZ_ActiveZone::onGWEvent_gameLoop_post(IEventListener& eventListener, base::GWEvent& event) {
	if (isDelayedZoneActivation) {
		setActiveZone(
			delayedZoneActivation_zoneName, delayedZoneActivation_zoneStartExtraData, delayedZoneActivation_restartZoneIfActive,
			false
		);
		isDelayedZoneActivation = false;
	}

	if (activeZone != nullptr && !isActiveZoneFinishedLoading) {
		bool isActivationFinished = activeZone->finishActivation();
		if (isActivationFinished) {
			isActiveZoneFinishedLoading = true;
		}
	}
}

void GPCZ_ActiveZone::disposePre() {
	if (activeZone != nullptr) {
		// The scenario handler must deactivate the zone before dispose is called.
		
		throw LogicException(LOC);
	}

	super::disposePre();
}

GPCZ_ActiveZone::~GPCZ_ActiveZone() {
	//void
}
