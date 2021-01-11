#include "SensorDataSensitizedLockedCursor.h"
#include <base/input/util/Touch.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace rpg3D;

const int SensorDataSensitizedLockedCursor::Type::Data = 1;

SensorDataSensitizedLockedCursor::SensorDataSensitizedLockedCursor(TouchEventType* touchType)
	: touchType(touchType)
{
	rawSubclassPointer = this;
}

SensorDataSensitizedLockedCursor* SensorDataSensitizedLockedCursor::getNew_LockedCursorDown(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos)
{
	SensorDataSensitizedLockedCursor* ret = newt SensorDataSensitizedLockedCursor(TouchEventType::CURSOR_DOWN);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);

	return ret;
}

SensorDataSensitizedLockedCursor* SensorDataSensitizedLockedCursor::getNew_LockedCursorMove(
	int cursorIndex, base::Touch::ToolType* toolType,
	Vector2& screenPos,
	Vector2& delta, Vector2& sensitisedDelta)
{
	SensorDataSensitizedLockedCursor* ret = newt SensorDataSensitizedLockedCursor(TouchEventType::CURSOR_MOVE);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;

	ret->screenPos.set(screenPos);

	ret->delta.set(delta);
	ret->sensitisedDelta.set(sensitisedDelta);

	return ret;
}

SensorDataSensitizedLockedCursor* SensorDataSensitizedLockedCursor::getNew_LockedCursorDrag(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos,
	Vector2& delta, Vector2& sensitisedDelta)
{
	SensorDataSensitizedLockedCursor* ret = newt SensorDataSensitizedLockedCursor(TouchEventType::CURSOR_DRAG);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);

	ret->delta.set(delta);
	ret->sensitisedDelta.set(sensitisedDelta);

	return ret;
}

SensorDataSensitizedLockedCursor* SensorDataSensitizedLockedCursor::getNew_LockedCursorUp(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos)
{
	SensorDataSensitizedLockedCursor* ret = newt SensorDataSensitizedLockedCursor(TouchEventType::CURSOR_UP);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);

	return ret;
}

SensorDataSensitizedLockedCursor::~SensorDataSensitizedLockedCursor() {
	//void
}
