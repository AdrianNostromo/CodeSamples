#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IGeometryBucket.h"
#include <base/memory/SharedPointer.h>

class IListEntry;
namespace graphics {
    class GeometryBucketConfig;
};

namespace graphics {
class GB_Base : public base::ManagedObject, public virtual IGeometryBucket {priv typedef base::ManagedObject super; pub dCtor(GB_Base);
    pub IListEntry* inGeometryManager_listEntry = nullptr;

    pub sp<GeometryBucketConfig> config;

    prot bool isLogicTickInProgress = false;

    pub explicit GB_Base(sp<GeometryBucketConfig> config);

    pub ~GB_Base() override;
};
};
