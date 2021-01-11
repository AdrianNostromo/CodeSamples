#pragma once

#include <base/gh.h>
#include "GB_Users.h"
#include <graphics/renderer/geometryManager/combinedRenderable/CombinedRenderable.h>
#include <base/list/ArrayList.h>

namespace graphics {
class GB_CombinedRenderables : public GB_Users {priv typedef GB_Users super;pub dCtor(GB_CombinedRenderables);

    //asd_01;
    //asd_01;/// Use a list of groups that contains the lists of cRenderables.
    //asd_01;/// The groups are sorted by the render order.
    prot ListDL<CombinedRenderable> combinedRenderablesList{};
    priv ArrayList<int> combinedRenderablesList_usedUIndicesTracker{};

    pub explicit GB_CombinedRenderables(sp<GeometryBucketConfig> config);

    pub ListDL<CombinedRenderable>& getCombinedRenderablesList() final;

    prot CombinedRenderable* getNextCombinedRenderable(
        CombinedRenderable* currentCombinedRenderable,
        GeometryProviderInstance* gpi_Vertices, GeometryProviderInstance* gpi_Indices,
        GeometryProviderInstance* gpi_ModelTransforms);
    priv int getNextAvailableCombinedRenderableUIndex();

    prot void dispose() override;
    pub ~GB_CombinedRenderables() override;
};
};
