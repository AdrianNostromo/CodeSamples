#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>

namespace worldGame3D {
    class IEntsGroup;
};

namespace worldGame3D {
class IGCEntityGroups : virtual public IGamePart {priv typedef IGamePart super;
    pub static int TYPE;
    DEC_componentBasics();

    pub virtual IEntsGroup* createEntitiesGroup() = 0;
    pub virtual IEntsGroup* getEntitiesGroup(int groupId) = 0;
    pub virtual void removeEntitiesGroup(IEntsGroup* entsGroup, bool disposeGroupEntities) = 0;

    pub ~IGCEntityGroups() override = default;
};
};
