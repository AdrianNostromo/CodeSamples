#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>
#include <base/list/ArrayList.h>

class IListEntry;
namespace graphics {
    class IRenderable;
    class Entry_GeometryManagerBucket;
    class IRenderablesManager;
};

namespace graphics {
class Entry_RenderablesManager {pub dCtor(Entry_RenderablesManager);
    pub IRenderable* renderable;

    pub IRenderablesManager* renderablesManager;
    // Use a typed entry because it is used in the sorted inserts used by 2d geometry.
    pub ListDL<Entry_RenderablesManager>::Entry* inRenderablesManager_listEntry = nullptr;

    // This is also used for insertAfter. In that case, this must have a count of 1 and the bucket must be the same as the inserting entry.
    pub ListDL<Entry_GeometryManagerBucket*> gManagerBucketEntriesList{};

    pub explicit Entry_RenderablesManager(IRenderable* renderable, IRenderablesManager* renderablesManager);

    pub void remove();

    pub virtual ~Entry_RenderablesManager();
};
};
