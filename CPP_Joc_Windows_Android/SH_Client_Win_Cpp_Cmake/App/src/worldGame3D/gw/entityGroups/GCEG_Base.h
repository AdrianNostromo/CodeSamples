#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGCEntityGroups.h"

namespace worldGame3D {
class GCEG_Base : public GamePart, public virtual IGCEntityGroups {priv typedef GamePart super;pub dCtor(GCEG_Base);
    pub explicit GCEG_Base(base::IGameWorld* gw);

    pub ~GCEG_Base() override;
};
};
