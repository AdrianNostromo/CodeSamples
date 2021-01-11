#pragma once

#include <base/gh.h>
#include "IGamePartLocalCacheContent.h"
#include <worldGame3D/gw/gamepart/GamePart.h>

namespace startarea_a_v1 {
class GPLCC_Base : public GamePart, virtual public IGamePartLocalCacheContent {priv typedef GamePart super;pub dCtor(GPLCC_Base);
	pub explicit GPLCC_Base(base::IGameWorld* gw);

    pub ~GPLCC_Base() override;
};
};
