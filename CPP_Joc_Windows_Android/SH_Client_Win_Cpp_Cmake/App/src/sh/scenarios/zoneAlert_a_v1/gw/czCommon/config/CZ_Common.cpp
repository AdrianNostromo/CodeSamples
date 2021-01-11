#include "CZ_Common.h"
#include <worldGame3D/gw/IGameWorld.h>
#include "../gw/view/mainui/MainInGameUIView.h"
#include "util/ZoneStartExtraData.h"
#include <sh/scenarios/zoneAlert_a_v1/gw/playerZeroHpHandling/IGPPlayerZeroHpHandling.h>
#include "../../czTownA/config/CZ_TownA.h"
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <base/app/IApp.h>
#include <base/fileSystem/fileHandler/async/IFileHandlerAsync.h>
#include <rpg3D/gw/contentZones/zone/contentData/ZoneContentData.h>
#include <functional>

using namespace zoneAlert_a_v1;
using namespace zoneAlert_a_v1::czCommon;

CZ_Common::CZ_Common(
	std::string& id,
	bool useAutoPlayerPositioning, std::shared_ptr<std::string> defaultZoneEntrance)
	: super(
		id,
		useAutoPlayerPositioning, defaultZoneEntrance)
{
	//void
}

int CZ_Common::getMainUIViewID() {
	return IMainInGameUIView::ID;
}

rpg3D::MainInGameUIView* CZ_Common::getNewMainUIViewUninitialised(base::IGameWorld* gw) {
	MainInGameUIView* ret = new MainInGameUIView(gw->getHandler()->getApp(), gw);

	return ret;
}

bool CZ_Common::loadActivationAssets(base::IGameWorld* gw) {
	bool isLoadedSuper = super::loadActivationAssets(gw);

	bool isLoadedLocally = false;

	if (assetsLoadStatus == PreStart) {
		std::string cacheEntryId = getId() + "/zoneData.dat";
		std::shared_ptr<base::IFileHandlerAsync> fhAsync = gw->getHandler()->getApp()->getEmbeddedAssetsManager()->getCacheGroupsHandler()
			->getCacheGroup(gw->getScenarioTemplate()->id)->getCacheFileHandlerAsync(cacheEntryId);

		if (fhAsync != nullptr) {
			// Cache item exist.

			// Add the needed events listeners.
			fhAsync->addEventListener(
				base::IFileHandlerAsync::EVENT_read_string_success->type, base::IFileHandlerAsync::EVENT_IO_failed->type,
				std::bind(&CZ_Common::onAsyncFHEvent, this, std::placeholders::_1, std::placeholders::_2),
				fhListenersList
			);

			// Start the async reads.
			assetsLoadStatus = LoadingData;

			fhAsync->readStringSPAsync();
		} else {
			assetsLoadStatus = Finished;
		}
	} else if (assetsLoadStatus == ProcessingData) {
		loadedZoneData = rpg3D::ZoneContentData::Parse(*loadingDataToProcess);

		loadingDataToProcess = nullptr;
		assetsLoadStatus = Finished;
	}
	
	// Keep this separate as a optimisation to avoid a 1 frame delay.
	if (assetsLoadStatus == Finished) {
		//void

		isLoadedLocally = true;
	}

	return isLoadedSuper && isLoadedLocally;
}

void CZ_Common::unloadActivationAssets() {
	super::unloadActivationAssets();

	loadedZoneData = nullptr;
	if (assetsLoadStatus != PreStart) {
		assetsLoadStatus = PreStart;
		fhListenersList.clear();
	}
}

void CZ_Common::onAsyncFHEvent(IEventListener& eventListener, base::AsyncFHEvent& event) {
	if (base::IFileHandlerAsync::EVENT_IO_failed->type == event.type) {
		// For now, just do a error.
		throw LogicException(LOC);
	} else if (base::IFileHandlerAsync::EVENT_read_string_success->type == event.type) {
		// Save the data for further processing.
		if (event.data_string == nullptr) {
			throw LogicException(LOC);
		}

		if (event.data_string->length() <= 0) {
			// Cache error occured. Clear the cache entry.
			std::string cacheEntryId = getId() + "/zoneData.dat";
			gw->getHandler()->getApp()->getEmbeddedAssetsManager()->getCacheGroupsHandler()
				->getCacheGroup(gw->getScenarioTemplate()->id)->clearCache(cacheEntryId);

			fhListenersList.clear();

			return;
		}

		loadingDataToProcess = event.data_string;

		fhListenersList.clear();

		assetsLoadStatus = ProcessingData;
	} else {
		throw LogicException(LOC);
	}
}

void CZ_Common::onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) {
	super::onZoneActivated(zoneStartExtraData, gw);

	ZoneStartExtraData* cZoneStartExtraData = zoneStartExtraData != nullptr ? zoneStartExtraData->getAsOptional<ZoneStartExtraData*>() : nullptr;

	std::shared_ptr<std::string> zeroHpZoneName;
	std::shared_ptr< std::string> zeroHpZonePlayerPosEntityId;

	if (cZoneStartExtraData != nullptr && cZoneStartExtraData->zeroHpZoneName != nullptr) {
		//cZoneStartExtraData->zeroHpZonePlayerPosEntityId can be nullptr and use the default pos then.
		if (cZoneStartExtraData->zeroHpZoneName == nullptr) {
			throw LogicException(LOC);
		}

		zeroHpZoneName = cZoneStartExtraData->zeroHpZoneName;
		zeroHpZonePlayerPosEntityId = cZoneStartExtraData->zeroHpZonePlayerPosEntityId;
	} else {
		zeroHpZoneName = std::make_shared<std::string>(czTownA::CZ_TownA::NAME);
		zeroHpZonePlayerPosEntityId = nullptr;
	}

	gw->getComponentAs<IGPPlayerZeroHpHandling*>(true/*mustExist*/)->setData(
		zeroHpZoneName, zeroHpZonePlayerPosEntityId,
		false/*playerDestroyed_restartScenario*/, false/*playerDestroyed_restartZoneIfActive*/
	);
}

void CZ_Common::onZoneDeactivate_pre(base::IGameWorld* gw) {
	super::onZoneDeactivate_pre(gw);

	//void
}

CZ_Common::~CZ_Common() {
	//void
}
