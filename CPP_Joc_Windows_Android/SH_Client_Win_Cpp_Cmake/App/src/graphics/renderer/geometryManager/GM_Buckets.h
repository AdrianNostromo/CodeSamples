#pragma once

#include <base/gh.h>
#include "GM_Base.h"

namespace graphics {
    class IRenderPhase;
    class GeometryBucket;
    class GeometryBucketConfig;
};

namespace graphics {
class GM_Buckets : public GM_Base {priv typedef GM_Base super;pub dCtor(GM_Buckets);
    prot ListDL<GeometryBucket> geometryBucketsList{};

    //asd_x; Currently this must be done before any renderables are added. When needed to have optional rPhases, enum all renderables and add them to the new buckets as needed.
    priv bool isAnyRenderableAdded = false;

    pub explicit GM_Buckets();
    prot void createMain() override;

    pub void onRenderPhaseAdded(IRenderPhase* renderPhase);
    pub void onRenderPhaseRemovePre(IRenderPhase* renderPhase);

    pub void addRenderable(
        IRenderable* renderable, ListDL<Entry_GeometryManagerBucket*>& gManagerBucketEntriesList,
        bool usesInsertAfterMode, IRenderable* insertAfterNodePartTarget) final;

    priv GeometryBucket* getBucketWithConfig(GeometryBucketConfig& bucketConfig);

    prot void dispose() override;
    pub ~GM_Buckets() override;
};
};
