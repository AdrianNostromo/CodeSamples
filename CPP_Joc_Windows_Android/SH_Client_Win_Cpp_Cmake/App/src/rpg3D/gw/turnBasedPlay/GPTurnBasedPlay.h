#pragma once

#include <base/gh.h>
#include "GPTBP_TurnControl.h"

namespace rpg3D {
class GPTurnBasedPlay : public GPTBP_TurnControl {priv typedef GPTBP_TurnControl super;pub dCtor(GPTurnBasedPlay);
    pub explicit GPTurnBasedPlay(
        base::IGameWorld* gw,
        TurnMode turnMode, sp<ArrayList<int>> turnBasedTeamsList);

    pub ~GPTurnBasedPlay() override;
};
};
