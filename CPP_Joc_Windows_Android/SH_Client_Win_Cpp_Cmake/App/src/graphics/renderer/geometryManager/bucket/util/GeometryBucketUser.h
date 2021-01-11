#pragma once

#include <base/gh.h>
#include <base/memory/SharedPointer.h>
#include <memory>

class IListEntry;
namespace graphics {
    class IGeometryBucket;
    class GeometryBucketConfig;
};

namespace graphics {
class GeometryBucketUser {pub dCtor(GeometryBucketUser);
    pub sp<GeometryBucketConfig> config;

    pub IGeometryBucket* bucket = nullptr;
    pub IListEntry* inBucket_listEntry = nullptr;

    pub explicit GeometryBucketUser(sp<GeometryBucketConfig> config);

    pub virtual ~GeometryBucketUser();
};
};
