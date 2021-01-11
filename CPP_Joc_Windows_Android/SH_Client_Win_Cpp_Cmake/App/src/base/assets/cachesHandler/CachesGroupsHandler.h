#pragma once

#include <base/gh.h>
#include "CGH_Base.h"
#include <base/map/Map1D.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <memory>

namespace base {
class CachesGroupsHandler : public CGH_Base {
priv typedef CGH_Base super;
	priv Map1D<std::string, std::shared_ptr<ICacheGroup>> cacheGroupsMap{};

    pub dCtor(CachesGroupsHandler);
    pub explicit CachesGroupsHandler();

	pub ICacheGroup* getCacheGroup(std::string& cacheGroupId) final;

    pub ~CachesGroupsHandler() override;
};
};
