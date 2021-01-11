#pragma once

#include <base/gh.h>
#include "GB_CombinedRenderables.h"
#include <base/list/Array1D.h>
#include <graphics/renderer/geometryManager/combinedRenderable/entry/ICRB_ModelTransforms.h>

namespace graphics {
class GB_Validation : public GB_CombinedRenderables {priv typedef GB_CombinedRenderables super;pub dCtor(GB_Validation);
    // This is used by the defragmenter.
    priv static float PreferredCrendFillRate;
    // This is used in the case when the entry is already in the next cRend, will switch to the next cRend if the fill_rate is >= than this.
    priv static float NextCRendJumpIfAlreadyInItFillRate;

    priv Array1D<ListDL<GeometryProviderInstance*>> invalidGPInstancesListsArray{ GEOMETRY_PROVIDER_TYPES_COUNT, new ListDL<GeometryProviderInstance*>[GEOMETRY_PROVIDER_TYPES_COUNT] {
        ListDL<GeometryProviderInstance*>(),
        ListDL<GeometryProviderInstance*>(),
        ListDL<GeometryProviderInstance*>(),
    }};

    ListDL<ICRB_ModelTransforms::ToRemoveUnusedEntry> markedForRemovalGPIModelTransformsList{};

    pub explicit GB_Validation(sp<GeometryBucketConfig> config);

    pub void invalidate(GeometryProviderInstance* gpInstance, int invalidationBitMask) final;

    pub void tickLogic() final;
    priv void tickLogic_toRemoveEntries();
    priv void tickLogic_invalidationGProviderInstances();
    priv void tickLogic_prepareCRenderablesForEntriesEnumerationInsertionPhase();
    priv void tickLogic_combineEntriesNoDataWrites();
    priv void tickLogic_defragmentations();
    priv void tickLogic_writeChangedDatas();
    priv void tickLogic_clearGPInstancesInvalidationFlags();
    priv void tickLogic_handleDelayedICRModelTransformsRemoval();

    pub void prepareForRemoval();

    pub ~GB_Validation() override;
};
};
