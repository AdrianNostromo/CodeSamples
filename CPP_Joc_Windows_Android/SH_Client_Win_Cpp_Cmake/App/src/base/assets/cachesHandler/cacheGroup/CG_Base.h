#pragma once

#include <base/gh.h>
#include "ICacheGroup.h"
#include <string>
#include <base/object/ManagedObject.h>

namespace base {
class CG_Base : public ManagedObject, virtual public ICacheGroup {priv typedef ManagedObject super; pub dCtor(CG_Base);
	prot std::string cacheGroupId;

    pub explicit CG_Base(std::string& cacheGroupId);

    pub void cacheIntegrityErrorDetected() override;

    // This is used to delete cache when the index is loaded and a write lock was found or the cache has the wrong version.
	prot void deleteCacheBlocking();

    pub ~CG_Base() override;
};
};
