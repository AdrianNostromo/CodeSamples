#pragma once

#include <base/gh.h>
#include "CG_WriteLock.h"
#include <base/map/Map1D.h>
#include <string>

namespace base {
class CG_Entries : public CG_WriteLock {priv typedef CG_WriteLock super;pub dCtor(CG_Entries);
	// The int is only for show. It is always 1.
	priv Map1D<std::string, int> entriesMap{};

    pub explicit CG_Entries(std::string& cacheGroupId);

	prot Map1D<std::string, int>* getEntriesMap();

	pub bool isCache(std::string& entryId) override;
	pub void setCache(std::string& entryId, base::Buffer data) override;
	pub void setCache(std::string& entryId, std::shared_ptr<std::string> data) override;
	pub void setCache(std::string& entryId, sp<std::string> data) override;
	pub void setCache(std::string& entryId, std::shared_ptr<std::stringstream> data) override;
	priv void setCache(std::string& entryId) override;
	pub bool clearCache(std::string& entryId) override;

	pub std::shared_ptr<IFileHandlerAsync> getCacheFileHandlerAsync(std::string& entryId) override;

	pub void cacheIntegrityErrorDetected() override;

	prot virtual void onEntryChanged();

    pub ~CG_Entries() override;
};
};
