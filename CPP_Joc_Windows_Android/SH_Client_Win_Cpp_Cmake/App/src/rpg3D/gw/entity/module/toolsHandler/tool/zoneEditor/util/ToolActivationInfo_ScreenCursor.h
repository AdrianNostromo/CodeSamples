#pragma once

#include <base/gh.h>
#include "ToolActivationInfo_Base.h"
#include <base/math/Vector2.h>

namespace rpg3D {
class ToolActivationInfo_ScreenCursor : public ToolActivationInfo_Base {priv typedef ToolActivationInfo_Base super;pub dCtor(ToolActivationInfo_ScreenCursor);
    pub static const int TYPE;

    pub Vector2 screenPos;
    // This is a cumulator and is cleared after processing.
    //asd_01;// try to acumulate this;
    pub Vector2 sensitisedScreenCursorMoveDelta;

    pub explicit ToolActivationInfo_ScreenCursor(int cursorIndex, Vector2& screenPos, Vector2& sensitisedScreenCursorMoveDelta);
    pub explicit ToolActivationInfo_ScreenCursor(int cursorIndex, Vector2& screenPos);

    pub void clearCumulators() override;

    pub ~ToolActivationInfo_ScreenCursor() override;
};
};
