#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGamePartPlayLogicHandler.h"

class EntityRemoveReason;
namespace towerDefense_a_v1 {
namespace PathTD {
	class IMainInGameUIView;
};
};

namespace towerDefense_a_v1 {
namespace PathTD {
class GPPLH_Base : public GamePart, virtual public IGamePartPlayLogicHandler {priv typedef GamePart super;pub dCtor(GPPLH_Base);
	priv int currentLives = 0;
	priv int currentGold = 0;

	prot IMainInGameUIView* mainUIVIew = nullptr;

	prot static EntityRemoveReason* const ENT_REM_REASON_PURPOSE_FINISHED;
	prot static EntityRemoveReason* const ERR_DESTINATION_REACHED;
	
	pub explicit GPPLH_Base(base::IGameWorld* gw);
	prot void createBMain() override;

	pub int getCurrentLives() final;
	pub void setCurrentLives(int currentLives) final;

	pub int getCurrentGold() final;
	pub void setCurrentGold(int currentGold) final;

    pub ~GPPLH_Base() override;
};
};
};
