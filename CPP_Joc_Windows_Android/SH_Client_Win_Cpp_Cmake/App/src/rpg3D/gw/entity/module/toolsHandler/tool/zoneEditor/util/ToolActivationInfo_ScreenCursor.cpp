#include "ToolActivationInfo_ScreenCursor.h"

using namespace rpg3D;

const int ToolActivationInfo_ScreenCursor::TYPE = super::NewType();

ToolActivationInfo_ScreenCursor::ToolActivationInfo_ScreenCursor(int cursorIndex, Vector2& screenPos, Vector2& sensitisedScreenCursorMoveDelta)
	: super(TYPE, cursorIndex),
	screenPos(screenPos), sensitisedScreenCursorMoveDelta(sensitisedScreenCursorMoveDelta)
{
	rawSubclassPointer = this;
}

ToolActivationInfo_ScreenCursor::ToolActivationInfo_ScreenCursor(int cursorIndex, Vector2& screenPos)
	: super(TYPE, cursorIndex),
	screenPos(screenPos), sensitisedScreenCursorMoveDelta()
{
	rawSubclassPointer = this;
}

void ToolActivationInfo_ScreenCursor::clearCumulators() {
	super::clearCumulators();

	sensitisedScreenCursorMoveDelta.set(0.0f, 0.0f);
}

ToolActivationInfo_ScreenCursor::~ToolActivationInfo_ScreenCursor() {
	//void
}
