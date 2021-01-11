#pragma once

#include <base/gh.h>
#include <string>

namespace base {
	class ICacheGroup;
};

namespace base {
class ICachesGroupsHandler {
	pub static std::string CACHE_GROUP_ID_GLOBAL;

	pub virtual ICacheGroup* getCacheGroup(std::string& cacheGroupId) = 0;
    
    pub virtual ~ICachesGroupsHandler() = default;
};
};
