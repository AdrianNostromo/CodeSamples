#pragma once

#include <base/gh.h>
#include "AM_Loaders.h"

namespace base {
	class CachesGroupsHandler;
};

namespace base {
class AM_CachesHandler : public AM_Loaders {
priv typedef AM_Loaders super;
	priv CachesGroupsHandler* cacheGroupsHandler = nullptr;

    pub dCtor(AM_CachesHandler);
    pub explicit AM_CachesHandler(IApp* app);
	prot void createMain() override;

	pub ICachesGroupsHandler* getCacheGroupsHandler() override;

	prot void disposeMain() override;
    pub ~AM_CachesHandler() override;
};
};
