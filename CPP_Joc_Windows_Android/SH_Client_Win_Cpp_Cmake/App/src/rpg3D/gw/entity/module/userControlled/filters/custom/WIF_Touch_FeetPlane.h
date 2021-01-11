#pragma once

#include <base/gh.h>
#include "../WorldInteractionFilter.h"
#include <functional>
#include <base/math/Vector3.h>

namespace rpg3D {
class WIF_Touch_FeetPlane : public WorldInteractionFilter {priv typedef WorldInteractionFilter super;pub dCtor(WIF_Touch_FeetPlane);
    priv std::function<bool(
        Vector3& localFeetPlanePos, int cursorIndex)> cb_onFilterActivation;

    pub explicit WIF_Touch_FeetPlane(
        int cursorModeBitGroups, int buttonIdBitGroups, int touchTypeBitGroups,
        int inputPriority,
        std::function<bool(Vector3& groundPos, int cursorIndex)> cb_onFilterActivation);

    pub bool onCursorInput(
        base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
        int cursorIndex, Vector2& screenPos,
        IWorldEntity* controlledEntity, int cameraMode, base::IGameWorld* gw) final;
    
    pub ~WIF_Touch_FeetPlane() override;
};
};
