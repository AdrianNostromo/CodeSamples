#pragma once

#include <base/gh.h>
#include "CG_IndexFileHandler.h"

namespace base {
class CacheGroup : public CG_IndexFileHandler {priv typedef CG_IndexFileHandler super;pub dCtor(CacheGroup);
    pub explicit CacheGroup(std::string& cacheGroupId);
    prot void createMain() override;

    pub ~CacheGroup() override;
};
};
