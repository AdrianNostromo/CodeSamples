#pragma once

#include <base/gh.h>
#include <base/math/Vector2.h>
#include <base/list/ArrayList.h>
#include <base/input/util/Touch.h>
#include <base/input/util/TouchEventType.h>

class IListEntry;
class IWorldEntity;
namespace base {
    class IGameWorld;
};

namespace rpg3D {
class IWorldInteractionFilter {
    pub class InputPriorities {
        // Most tools use this.
        pub static const int Normal;
        // Use for grid movement so it blocks tool_bow for activating.
        pub static const int Normal_Plus1;
    };

    pub virtual int getCursorModeBitGroups() = 0;
    pub virtual int getButtonIdBitGroups() = 0;
    pub virtual int getTouchTypeBitGroups() = 0;

    pub virtual int getInputPriority() = 0;
    pub virtual int getCameraModesBitMaskAny() = 0;

    pub virtual bool& getIsHookedRef() = 0;
    pub virtual ArrayList<IListEntry*>& getHookedListEntries() = 0;

    // Return true if input processed and false if ignorred.
    /// This is used for filter priorities so that higher priority filters can block optionally.
    pub virtual bool onCursorInput(
        base::Touch::CursorMode* cursorMode, base::Touch::ButtonCode* buttonId, TouchEventType* touchType,
        int cursorIndex, Vector2& screenPos,
        IWorldEntity* controlledEntity, int cameraModeBitGroup, base::IGameWorld* gw) = 0;
    
    pub virtual ~IWorldInteractionFilter() = default;
};
};
