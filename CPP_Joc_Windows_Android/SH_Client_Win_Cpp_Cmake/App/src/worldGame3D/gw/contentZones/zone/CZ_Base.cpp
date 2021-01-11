#include "CZ_Base.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <assert.h>
#include <worldGame3D/gw/handler/IHandler_GameWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/content/IGPContent.h>

using namespace worldGame3D;

CZ_Base::CZ_Base(std::string& id)
	: id(id)
{
	//void
}

std::string& CZ_Base::getId() {
	return id;
}

bool CZ_Base::getIsActive() {
	return isActive;
}

std::shared_ptr<worldGame3D::ZoneStartExtraData> CZ_Base::getZoneStartExtraData() {
	if (!isActive) {
		throw LogicException(LOC);
	}

	return zoneStartExtraData;
}

void CZ_Base::preActivate(
	std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData,
	base::IGameWorld* gw)
{
	if (isActive) {
		throw LogicException(LOC);
	}
	this->gw = gw;
	this->zoneStartExtraData = zoneStartExtraData;
	isActive = true;
}

bool CZ_Base::finishActivation() {
	bool isLoadingFinished = loadActivationAssets(gw);
	if (!isLoadingFinished) {
		return false;
	}

	onZoneActivatedPre(zoneStartExtraData, gw);
	onZoneActivated(zoneStartExtraData, gw);
	onZoneActivatedPositionPlayer(zoneStartExtraData, gw);

	// Do this connect here because error may occur if grid positioning is used while the player has his position from the prev zone.
	gw->getMain()->connectAllEntitiesToGameParts();

	IWorldEntity* selectedPlayerEntity = gw->getGPContent()->getSelectedPlayerEntity(false/*mustExist*/);
	if (selectedPlayerEntity != nullptr) {
		selectedPlayerEntity->setIsActiveIncremental(true);
	}

	gw->dispatchEvent(*GWEvent_onZoneActivated);

	onZoneActivatedB(zoneStartExtraData, gw);

	return true;
}

void CZ_Base::deactivate(base::IGameWorld* gw) {
	if (!isActive) {
		throw LogicException(LOC);
	}
	isActive = false;

	unloadActivationAssets();

	gw->dispatchEvent(*base::IGameWorld::GWEvent_saveData);
	gw->dispatchEvent(*base::IGameWorld::GWEvent_zoneDeactivate_pre);
	gw->dispatchEvent(*GWEvent_onZoneDeactivatePre);

	onZoneDeactivate_pre(gw);

	this->gw = nullptr;
}

bool CZ_Base::loadActivationAssets(base::IGameWorld* gw) {
	return true;
}

void CZ_Base::unloadActivationAssets() {
	//void
}

void CZ_Base::onZoneActivatedPre(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	//void
}

void CZ_Base::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	//void
}

void CZ_Base::onZoneActivatedPositionPlayer(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	//void
}

void CZ_Base::onZoneActivatedB(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	//void
}

void CZ_Base::onZoneDeactivate_pre(base::IGameWorld* gw) {
	autoListenersList.clear();

	gw->getMain()->removeAllZoneOwnedEntities();
	gw->getFlags().removeAllZoneOwnedFlags();
	IWorldEntity* selectedPlayerEntity = gw->getGPContent()->getSelectedPlayerEntity(false/*mustExist*/);
	if (selectedPlayerEntity != nullptr) {
		selectedPlayerEntity->setIsActiveIncremental(false);
	}

	gw->getMain()->disconnectAllEntitiesFromGameParts();
}

CZ_Base::~CZ_Base() {
	if (isActive) {
		assert(0);
	}
}
