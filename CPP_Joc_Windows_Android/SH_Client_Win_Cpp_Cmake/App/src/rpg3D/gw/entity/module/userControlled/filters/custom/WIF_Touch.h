#pragma once

#include <base/gh.h>
#include "../WorldInteractionFilter.h"
#include <functional>
#include <base/math/Vector3.h>

namespace rpg3D {
class WIF_Touch : public WorldInteractionFilter {priv typedef WorldInteractionFilter super;pub dCtor(WIF_Touch);
    priv std::function<bool(int cursorIndex, Vector2& screenPos)> cb_onFilterActivation;

    pub explicit WIF_Touch(
        int cursorModeBitGroups, int buttonIdBitGroups, int touchTypeBitGroups,
        int inputPriority,
        std::function<bool(int cursorIndex, Vector2& screenPos)> cb_onFilterActivation);

    pub bool onCursorInput(
        base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
        int cursorIndex, Vector2& screenPos,
        IWorldEntity* controlledEntity, int cameraMode, base::IGameWorld* gw) final;
    
    pub ~WIF_Touch() override;
};
};
