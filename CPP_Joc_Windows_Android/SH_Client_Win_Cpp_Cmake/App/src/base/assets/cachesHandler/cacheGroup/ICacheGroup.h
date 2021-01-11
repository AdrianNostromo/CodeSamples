#pragma once

#include <base/gh.h>
#include <string>
#include <base/buffer/Buffer.h>
#include <base/sp.h>

namespace base {
	class IFileHandlerAsync;
	class IFileHandlerBlocking;
};

namespace base {
class ICacheGroup {
	pub virtual bool isCache(std::string& entryId) = 0;
	pub virtual void setCache(std::string& entryId, base::Buffer data) = 0;
	pub virtual void setCache(std::string& entryId, std::shared_ptr<std::string> data) = 0;
	pub virtual void setCache(std::string& entryId, sp<std::string> data) = 0;
	pub virtual void setCache(std::string& entryId, std::shared_ptr<std::stringstream> data) = 0;
	pub virtual void setCache(std::string& entryId) = 0;
	pub virtual bool clearCache(std::string& entryId) = 0;
	
	pub virtual std::shared_ptr<IFileHandlerAsync> getCacheFileHandlerAsync(std::string& entryId) = 0;

	pub virtual void cacheIntegrityErrorDetected() = 0;

	pub virtual void setWriteLockStatus(bool writeLockStatus) = 0;

    pub virtual ~ICacheGroup() = default;
};
};
