#pragma once

#include <base/gh.h>
#include "GPUEC_ControlledEntity.h"
#include "IUserInputDispatcher.h"

class IListEntry;

namespace rpg3D {
class GPUEC_CustomFilters : public GPUEC_ControlledEntity, virtual public IUserInputDispatcher {priv typedef GPUEC_ControlledEntity super;pub dCtor(GPUEC_CustomFilters);
    pub explicit GPUEC_CustomFilters(base::IGameWorld* gw);

    prot void onControlledEntityChange_pre() override;
    prot void onControlledEntityChanged() override;

    pub void hookCustomFilter(rpg3D::IWorldInteractionFilter* filter) final;
    pub void releaseCustomFilter(rpg3D::IWorldInteractionFilter* filter) final;
    prot virtual void onHookCustomFilter(rpg3D::IWorldInteractionFilter* filter);

    prot static IListEntry* InsertFilterInList(rpg3D::IWorldInteractionFilter* filter, ListDL<IWorldInteractionFilter*>& customFiltersLists);

    pub ~GPUEC_CustomFilters() override;
};
};
