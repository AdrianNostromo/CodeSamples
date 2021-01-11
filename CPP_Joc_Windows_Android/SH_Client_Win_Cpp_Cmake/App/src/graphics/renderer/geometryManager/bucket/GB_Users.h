#pragma once

#include <base/gh.h>
#include "GB_Entries.h"
#include <base/list/ListDL.h>

namespace graphics {
    class GeometryBucketUser;
};

namespace graphics {
class GB_Users : public GB_Entries {priv typedef GB_Entries super;pub dCtor(GB_Users);
    // Unmanaged
    pub ListDL<GeometryBucketUser*> usersList{};

    pub explicit GB_Users(sp<GeometryBucketConfig>& config);

    pub ~GB_Users() override;
};
};
