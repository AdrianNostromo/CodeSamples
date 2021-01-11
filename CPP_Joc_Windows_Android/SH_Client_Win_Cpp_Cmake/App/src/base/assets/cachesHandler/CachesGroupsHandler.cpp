#include "CachesGroupsHandler.h"
#include <base/assets/cachesHandler/cacheGroup/CacheGroup.h>

using namespace base;

CachesGroupsHandler::CachesGroupsHandler()
	: super()
{
	//void
}

ICacheGroup* CachesGroupsHandler::getCacheGroup(std::string& cacheGroupId) {
	// Cehck if it is in the map. If exists, use that.
	std::shared_ptr<ICacheGroup> cacheGroup = cacheGroupsMap.getDirect(cacheGroupId, nullptr);

	if (cacheGroup == nullptr) {
		std::shared_ptr<CacheGroup> newCacheGroup = std::make_shared< CacheGroup>(cacheGroupId);
		newCacheGroup->reservedCreate();

		cacheGroup = newCacheGroup;

		cacheGroupsMap.putReference(cacheGroupId, cacheGroup);
	}

	return cacheGroup.get();
}

CachesGroupsHandler::~CachesGroupsHandler() {
	//void
}
