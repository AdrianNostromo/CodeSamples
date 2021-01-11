#pragma once

#include <base/gh.h>
#include "GB_Validation.h"

namespace graphics {
    class GeometryBucketConfig;
};

namespace graphics {
class GeometryBucket : public GB_Validation {priv typedef GB_Validation super;pub dCtor(GeometryBucket);
    pub explicit GeometryBucket(sp<GeometryBucketConfig> config);

    pub ~GeometryBucket() override;
};
};
