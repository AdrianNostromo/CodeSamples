#pragma once

#include <base/gh.h>
#include "IWorldInteractionFilter.h"

namespace rpg3D {
class WorldInteractionFilter : virtual public IWorldInteractionFilter {pub dCtor(WorldInteractionFilter);
    pub int cursorModeBitGroups;
    pub int buttonIdBitGroups;
    pub int touchTypeBitGroups;

    priv int inputPriority;

    priv bool isHooked = false;
    priv ArrayList<IListEntry*> hookedListEntries{};
    
    // Set to -1 to ignore.
    priv int cameraModesBitMaskAny;

    pub explicit WorldInteractionFilter(
        int cursorModeBitGroups, int buttonIdBitGroups, int touchTypeBitGroups,
        int inputPriority, int cameraModesBitMaskAny);

    pub int getCursorModeBitGroups() final;
    pub int getButtonIdBitGroups() final;
    pub int getTouchTypeBitGroups() final;

    pub int getInputPriority() final;
    pub int getCameraModesBitMaskAny() final;

    pub bool& getIsHookedRef() final;
    pub ArrayList<IListEntry*>& getHookedListEntries() final;

    // This must be fully extended when used.
    pub bool onCursorInput(
        base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
        int cursorIndex, Vector2& screenPos,
        IWorldEntity* controlledEntity, int cameraModeBitGroup, base::IGameWorld* gw) override;
    
    pub ~WorldInteractionFilter() override;
};
};
