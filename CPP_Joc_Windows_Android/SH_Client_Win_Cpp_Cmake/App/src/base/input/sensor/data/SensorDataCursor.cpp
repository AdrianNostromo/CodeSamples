#include "SensorDataCursor.h"
#include <base/input/util/Touch.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace base;

const int SensorDataCursor::Type::CursorDown = 1;
const int SensorDataCursor::Type::CursorDownCanceled = 2;

const int SensorDataCursor::Type::CursorMoved = 3;
const int SensorDataCursor::Type::CursorMoveCanceled = 4;

const int SensorDataCursor::Type::CursorDrag = 5;

const int SensorDataCursor::Type::CursorUp = 6;

const int SensorDataCursor::Type::LockedCursorDown = 7;
const int SensorDataCursor::Type::LockedCursorDownCanceled = 8;

const int SensorDataCursor::Type::LockedCursorMoved = 9;

const int SensorDataCursor::Type::LockedCursorDrag = 10;

const int SensorDataCursor::Type::LockedCursorUp = 11;

SensorDataCursor::SensorDataCursor(int type)
	: type(type)
{
	rawSubclassPointer = this;
}

SensorDataCursor* SensorDataCursor::getNew_CursorDown(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::CursorDown);
	
	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;
	
	ret->screenPos.set(screenPos);
	
	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_CursorDownCanceled(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::CursorDownCanceled);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_CursorUp(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::CursorUp);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_CursorMoved(
	int cursorIndex, base::Touch::ToolType* toolType,
	Vector2& screenPos)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::CursorMoved);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;

	ret->screenPos.set(screenPos);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_CursorMoveCanceled(
	int cursorIndex,
	Vector2& screenPos)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::CursorMoveCanceled);

	ret->cursorIndex = cursorIndex;

	ret->screenPos.set(screenPos);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_CursorDrag(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::CursorDrag);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_LockedCursorDown(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& lockedCursorSpace)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::LockedCursorDown);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);
	ret->lockedCursorSpace.set(lockedCursorSpace);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_LockedCursorMoved(
	int cursorIndex, base::Touch::ToolType* toolType,
	Vector2& screenPos, Vector2& delta)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::LockedCursorMoved);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;

	ret->screenPos.set(screenPos);
	ret->delta.set(delta);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_LockedCursorDrag(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& delta)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::LockedCursorDrag);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);
	ret->delta.set(delta);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_LockedCursorUp(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& lockedCursorSpace)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::LockedCursorUp);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);
	ret->lockedCursorSpace.set(lockedCursorSpace);

	return ret;
}

SensorDataCursor* SensorDataCursor::getNew_LockedCursorDownCanceled(
	int cursorIndex, base::Touch::ToolType* toolType, base::Touch::ButtonCode* buttonId,
	Vector2& screenPos, Vector2& lockedCursorSpace)
{
	SensorDataCursor* ret = newt SensorDataCursor(Type::LockedCursorDownCanceled);

	ret->cursorIndex = cursorIndex;
	ret->toolType = toolType;
	ret->buttonId = buttonId;

	ret->screenPos.set(screenPos);
	ret->lockedCursorSpace.set(lockedCursorSpace);

	return ret;
}

SensorDataCursor::~SensorDataCursor() {
	//void
}
