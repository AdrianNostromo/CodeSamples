#pragma once

#include <base/gh.h>
#include <base/input/sensor/data/SensorData.h>
#include <base/math/Vector2.h>
#include <base/input/util/Touch.h>
#include <base/input/util/TouchEventType.h>

namespace rpg3D {
class SensorDataSensitizedLockedCursor : public base::SensorData {priv typedef base::SensorData super;pub dCtor(SensorDataSensitizedLockedCursor);
    pub class Type {
        pub static const int Data;
    };
	
	pub TouchEventType* touchType;
	
	pub int cursorIndex;
	pub base::Touch::ToolType* toolType;
	pub base::Touch::ButtonCode* buttonId;
	
	pub Vector2 screenPos;
	
	pub Vector2 delta;
	pub Vector2 sensitisedDelta;

    pub explicit SensorDataSensitizedLockedCursor(TouchEventType* touchType);

	pub static SensorDataSensitizedLockedCursor* getNew_LockedCursorDown(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos);
	
	pub static SensorDataSensitizedLockedCursor* getNew_LockedCursorMove(
		int cursorIndex, base::Touch::ToolType* toolType,
		Vector2& screenPos,
		Vector2& delta, Vector2& sensitisedDelta);
	
	pub static SensorDataSensitizedLockedCursor* getNew_LockedCursorDrag(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos,
		Vector2& delta, Vector2& sensitisedDelta);
	
	pub static SensorDataSensitizedLockedCursor* getNew_LockedCursorUp(
		int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos);

    pub ~SensorDataSensitizedLockedCursor() override;
};
};
