#pragma once

#include "GPCC_PlayerEntityStyle.h"

namespace rpg3D {
class GamePartCustomisedContent : public GPCC_PlayerEntityStyle {priv typedef GPCC_PlayerEntityStyle super;pub dCtor(GamePartCustomisedContent);
    pub explicit GamePartCustomisedContent(base::IGameWorld* gw);

    pub ~GamePartCustomisedContent() override;
};
};
