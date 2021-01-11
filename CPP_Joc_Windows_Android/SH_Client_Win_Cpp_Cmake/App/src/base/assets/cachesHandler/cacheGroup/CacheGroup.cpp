#include "CacheGroup.h"
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/config/AppConfig.h>

using namespace base;

CacheGroup::CacheGroup(std::string& cacheGroupId)
	: super(cacheGroupId)
{
	//void
}

void CacheGroup::createMain() {
	super::createMain();

	bool isLockFile = readWriteLockFileStatusBlocking();
	if (isLockFile) {
		// A incomplete cache write occured. Delete the cache folder.
		//asd_x;// In the future show a info dialog.

		GlobalAccessLogHandler::devLogHandler->post_debug("Cache (%s) open detected a lock file and got cleared. Recovered from a incomplete cache write in a previous app run.", cacheGroupId.c_str());

		deleteCacheBlocking();

		// Create a new empty cache index.
		saveIndexFile();
	} else {
		LoadIndexFileRet ret = loadIndexFile(AppConfig::CACHE_DATA_VERSION);
		if (ret == Success) {
			//void
		} else if (ret == Fail_MissingOrCorrupt || ret == Fail_WrongVersion) {
			// Clear the index folder even if not found because cache data may exist but with no index file.
			deleteCacheBlocking();

			// Save the empty index file.
			saveIndexFile();
		} else {
			// Unhandled return value.
			throw LogicException(LOC);
		}
	}
}

CacheGroup::~CacheGroup() {
	//void
}
