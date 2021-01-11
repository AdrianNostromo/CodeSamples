#pragma once

#include "GWParts.h"

namespace rpg3D {
class GameWorldExtendable : public GWParts {priv typedef GWParts super;
public:
    GameWorldExtendable(GameWorldExtendable const&) = delete;
    GameWorldExtendable(GameWorldExtendable &&) = default;
    GameWorldExtendable& operator=(GameWorldExtendable const&) = delete;
    GameWorldExtendable& operator=(GameWorldExtendable &&) = default;

    explicit GameWorldExtendable(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

    ~GameWorldExtendable() override;
};
};
