#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGamePartPlayLogicHandler.h"

class EntityRemoveReason;
namespace gridAdventure_a_v1 {
namespace GridBattleA {
	class IMainInGameUIView;
};
};

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class GPPLH_Base : public GamePart, virtual public IGamePartPlayLogicHandler {priv typedef GamePart super;pub dCtor(GPPLH_Base);
	pub explicit GPPLH_Base(base::IGameWorld* gw);

    pub ~GPPLH_Base() override;
};
};
};
