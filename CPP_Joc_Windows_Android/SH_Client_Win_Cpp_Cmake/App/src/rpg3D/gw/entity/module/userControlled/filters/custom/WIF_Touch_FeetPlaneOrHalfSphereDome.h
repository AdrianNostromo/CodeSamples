#pragma once

#include <base/gh.h>
#include "../WorldInteractionFilter.h"
#include <functional>
#include <base/math/Vector3.h>
#include <base/input/util/Touch.h>
#include <base/input/util/TouchEventType.h>

namespace rpg3D {
class WIF_Touch_FeetPlaneOrHalfSphereDome : public WorldInteractionFilter {priv typedef WorldInteractionFilter super;pub dCtor(WIF_Touch_FeetPlaneOrHalfSphereDome);
    priv std::function<bool(
        Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup)> cb_onFilterActivation_LocalFeetPlane;
    priv std::function<bool(
        Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup)> cb_onFilterActivation_HalfSphereDome;
    
    pub explicit WIF_Touch_FeetPlaneOrHalfSphereDome(
        int cursorModeBitGroups, int buttonIdBitGroups, int touchTypeBitGroups,
        int inputPriority, int cameraModesBitMaskAny,
        std::function<bool(Vector3& localFeetPlanePos, int cursorIndex, int cameraModeBitGroup)> cb_onFilterActivation_LocalFeetPlane,
        std::function<bool(Vector3& halfSphereDomeWorldDir, int cursorIndex, int cameraModeBitGroup)> cb_onFilterActivation_HalfSphereDome);

    pub bool onCursorInput(
        base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
        int cursorIndex, Vector2& screenPos,
        IWorldEntity* controlledEntity, int cameraModeBitGroup, base::IGameWorld* gw) final;
    
    pub ~WIF_Touch_FeetPlaneOrHalfSphereDome() override;
};
};
