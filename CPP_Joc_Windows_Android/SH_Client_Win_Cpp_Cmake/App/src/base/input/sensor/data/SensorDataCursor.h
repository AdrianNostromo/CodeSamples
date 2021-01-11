#pragma once

#include <base/gh.h>
#include "SensorData.h"
#include <base/math/Vector2.h>
#include <base/input/util/Touch.h>

namespace base {
class SensorDataCursor : public SensorData {priv typedef SensorData super;pub dCtor(SensorDataCursor);
    pub class Type {
        pub static const int CursorDown;
        pub static const int CursorDownCanceled;

        pub static const int CursorMoved;
        pub static const int CursorMoveCanceled;

        pub static const int CursorDrag;

        pub static const int CursorUp;
        
        pub static const int LockedCursorDown;
        pub static const int LockedCursorDownCanceled;
    
        pub static const int LockedCursorMoved;

        pub static const int LockedCursorDrag;
        
        pub static const int LockedCursorUp;
    };
    
    pub int type;

    int cursorIndex;
    base::Touch::ToolType* toolType;
    base::Touch::ButtonCode* buttonId;
    
    Vector2 screenPos;
    
	Vector2 delta;
    
	Vector2 lockedCursorSpace;

    priv explicit SensorDataCursor(int type);
    
    pub static SensorDataCursor* getNew_CursorDown(
        int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos);
    pub static SensorDataCursor* getNew_CursorDownCanceled(
        int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos);

    pub static SensorDataCursor* getNew_CursorUp(
        int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos);

    // This is always received for a mouse even if the a button is pressed.
    // This is never received for a touch_screen.
    pub static SensorDataCursor* getNew_CursorMoved(
        int cursorIndex, base::Touch::ToolType* toolType,
        Vector2& screenPos);
    pub static SensorDataCursor* getNew_CursorMoveCanceled(
        int cursorIndex,
        Vector2& screenPos);

    pub static SensorDataCursor* getNew_CursorDrag(
        int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos);

    pub static SensorDataCursor* getNew_LockedCursorDown(
        int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& lockedCursorSpace);

    pub static SensorDataCursor* getNew_LockedCursorMoved(
        int cursorIndex, base::Touch::ToolType* toolType,
        Vector2& screenPos, Vector2& delta);

	pub static SensorDataCursor* getNew_LockedCursorDrag(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& delta);

    pub static SensorDataCursor* getNew_LockedCursorUp(
        int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& lockedCursorSpace);
    pub static SensorDataCursor* getNew_LockedCursorDownCanceled(
        int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& lockedCursorSpace);
    
    pub ~SensorDataCursor() override;
};
};
