#pragma once

#include <base/gh.h>
#include "RP_Base.h"
#include <string>
#include <base/MM.h>
#include <base/list/ListDL.h>
#include <graphics/renderer/geometryManager/bucket/util/GeometryBucketUser.h>

namespace graphics {
    class GeometryManager;
    class IGeometryManager;
};

namespace graphics {
class RP_GeometryBuckets : public RP_Base {priv typedef RP_Base super;pub dCtor(RP_GeometryBuckets);
    // Entries are initialised in the constructor of the localisd rPhase classes.
    prot ListDL<GeometryBucketUser> geometryBucketUsers{};

    pub explicit RP_GeometryBuckets(int renderOrder, bool useDepthTest);

    pub ListDL<GeometryBucketUser>& getGeometryBucketUsers() final;
   
    pub ~RP_GeometryBuckets() override;
};
};
