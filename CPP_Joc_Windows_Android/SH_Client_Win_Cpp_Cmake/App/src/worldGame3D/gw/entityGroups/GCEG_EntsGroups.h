#pragma once

#include <base/gh.h>
#include "GCEG_Base.h"
#include <base/list/ArrayList.h>
#include <base/sp.h>
#include <base/map/Map1D.h>

namespace worldGame3D {
    class EntsGroup;
};

namespace worldGame3D {
class GCEG_EntsGroups : public GCEG_Base {priv typedef GCEG_Base super;pub dCtor(GCEG_EntsGroups);
	// Null padded. entry.uid is the index.
    priv ArrayList<sp<EntsGroup>> entGroupsList{};

    pub explicit GCEG_EntsGroups(base::IGameWorld* gw);

    pub IEntsGroup* createEntitiesGroup() final;
    pub IEntsGroup* getEntitiesGroup(int groupId) final;
    pub void removeEntitiesGroup(IEntsGroup* entsGroup, bool disposeGroupEntities) final;

    priv int getNextGroupIndexUid();

    pub ~GCEG_EntsGroups() override;
};
};
