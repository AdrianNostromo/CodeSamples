#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGamePartPlayLogicHandler.h"

class EntityRemoveReason;
namespace roomgeons_a_v1 {
namespace ArenaRoom {
	class IMainInGameUIView;
};
};

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class GPPLH_Base : public GamePart, virtual public IGamePartPlayLogicHandler {priv typedef GamePart super;pub dCtor(GPPLH_Base);
	pub explicit GPPLH_Base(base::IGameWorld* gw);

    pub ~GPPLH_Base() override;
};
};
};
