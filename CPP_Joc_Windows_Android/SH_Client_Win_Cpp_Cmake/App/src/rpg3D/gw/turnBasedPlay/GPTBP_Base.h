#pragma once

#include <base/gh.h>
#include <base/MM.h>
#include "IGPTurnBasedPlay.h"
#include <worldGame3D/gw/gamepart/GamePart.h>
#include <base/memory/SharedPointer.h>

namespace rpg3D {
class GPTBP_Base : public GamePart, virtual public IGPTurnBasedPlay {priv typedef GamePart super;pub dCtor(GPTBP_Base);
    pub enum TurnMode { SeparateMoveAbilityTurns, SingleActionTurn};

    prot TurnMode turnMode;
    prot sp<ArrayList<int>> turnBasedTeamsList;

    pub explicit GPTBP_Base(
        base::IGameWorld* gw,
        TurnMode turnMode, sp<ArrayList<int>> turnBasedTeamsList);

    pub ~GPTBP_Base() override;
};
};
