#include "CCH_PlayerUnitPaint.h"
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <rpg3D/gw/cache/CacheIds.h>
#include <rpg3D/gw/util/Util_VoxlesGrid3DGeometry.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/app/IApp.h>
#include <base/assets/IAssetsManager.h>
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <base/fileSystem/fileHandler/async/IFileHandlerAsync.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/app/IApp.h>
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <base/exceptions/DataException.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace rpg3D;

CCH_PlayerUnitPaint::CCH_PlayerUnitPaint(IApp* app)
	: super(app)
{
	//void
}

std::shared_ptr<CCH_PlayerUnitPaint::EntityMaterialsAndVoxelsGrid> CCH_PlayerUnitPaint::getPlayerUnitMaterialsAndVoxelsGrid() {
	if(playerUnitMaterialsAndVoxelsGrid == nullptr) {
		playerUnitMaterialsAndVoxelsGrid = std::make_shared< EntityMaterialsAndVoxelsGrid>();
		playerUnitMaterialsAndVoxelsGrid->voxelGrid = rpg3D::UnitTemplates_PlayerA::defaultVoxelGridZXY;
		playerUnitMaterialsAndVoxelsGrid->materialsList = rpg3D::UnitTemplates_PlayerA::defaultMaterialsList;

		if (isLoadInProgress) {
			throw LogicException(LOC);
		}

		std::shared_ptr<base::IFileHandlerAsync> fh = app->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(CUSTOMISED_CONTENT_CACHE_GROUP)
			->getCacheFileHandlerAsync(rpg3D::CacheIds::playerUnitMaterialsAndVoxelsGrid);
		
		if (fh != nullptr) {
			// Cache item exist.
			
			// Add the needed events listeners.
			fh->addEventListener(
				base::IFileHandlerAsync::EVENT_read_string_success->type, base::IFileHandlerAsync::EVENT_IO_failed->type,
				std::bind(&CCH_PlayerUnitPaint::onAsyncFHEvent, this, std::placeholders::_1, std::placeholders::_2),
				fhListenersList
			);

			// Start the async reads.
			isLoadInProgress = true;

			fh->readStringSPAsync();
		}
	}

	return playerUnitMaterialsAndVoxelsGrid;
}

void CCH_PlayerUnitPaint::updatePlayerUnitMaterialsAndVoxelsGrid(VoxlesGrid3DGeometry* voxels3DGeometry, bool dispatchGWUpdateEvent) {
	// Serialise and write to cache.
	std::shared_ptr<std::stringstream> eTemplateDataSS = rpg3D::Util_VoxlesGrid3DGeometry::serialize(
		nullptr/*entity*/, voxels3DGeometry,
		false/*saveEntityGenerals*/, true/*saveMaterials*/, true/*saveVoxelsGrid*/
	);
	// Write the entity template data to a a cache file.
	app->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(CUSTOMISED_CONTENT_CACHE_GROUP)
		->setCache(rpg3D::CacheIds::playerUnitMaterialsAndVoxelsGrid, eTemplateDataSS);

	if (dispatchGWUpdateEvent && getActiveGameWorld() != nullptr) {
		getActiveGameWorld()->dispatchEvent(*ICustomisedContentHandler::GWEvent_PlayerUnitMaterialsAndVoxelsGrid_edited);
	}

	//asd_unit_1;// need to update playerUnitMaterialsAndVoxelsGrid;
}

void CCH_PlayerUnitPaint::onAsyncFHEvent(IEventListener& eventListener, base::AsyncFHEvent& event) {
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
			app->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(CUSTOMISED_CONTENT_CACHE_GROUP)
				->clearCache(rpg3D::CacheIds::playerUnitMaterialsAndVoxelsGrid);

			fhListenersList.clear();

			return;
		}

		data_waitingFileRead = event.data_string;

		fhListenersList.clear();

		app->getLoopDistribution()->addEventListener(
			base::ILoopDistribution::AppLoopEvent_gameWorld_pre->type,
			std::bind(&CCH_PlayerUnitPaint::onLoopEvent_processReadDataLoop, this, std::placeholders::_1, std::placeholders::_2),
			fhListenersList
		);
	} else {
		throw LogicException(LOC);
	}
}

void CCH_PlayerUnitPaint::onLoopEvent_processReadDataLoop(IEventListener& eventListener, base::LoopEvent& event) {
	if (data_waitingFileRead != nullptr) {
		std::shared_ptr<ParseData_VoxlesGrid3DGeometry> entityData = nullptr;

		try {
			entityData = rpg3D::Util_VoxlesGrid3DGeometry::parse(
				*data_waitingFileRead,
				false/*loadEntityGenerals*/, true/*loadMaterials*/, true/*loadVoxelsGrid*/
			);
		} catch (DataException& x) {
			app->getEmbeddedAssetsManager()->getCacheGroupsHandler()->getCacheGroup(CUSTOMISED_CONTENT_CACHE_GROUP)
				->clearCache(rpg3D::CacheIds::playerUnitMaterialsAndVoxelsGrid);
		}

		playerUnitMaterialsAndVoxelsGrid = std::make_shared< EntityMaterialsAndVoxelsGrid>();

		playerUnitMaterialsAndVoxelsGrid->_spRef_voxelGrid = entityData->grid;
		playerUnitMaterialsAndVoxelsGrid->_spRef_materialsList = entityData->materialsList;

		playerUnitMaterialsAndVoxelsGrid->voxelGrid = entityData->grid.get();
		playerUnitMaterialsAndVoxelsGrid->materialsList = entityData->materialsList.get();
		
		data_waitingFileRead = nullptr;

		fhListenersList.clear();

		getActiveGameWorld()->dispatchEvent(*ICustomisedContentHandler::GWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded);
	}
}

CCH_PlayerUnitPaint::~CCH_PlayerUnitPaint() {
	//void
}
