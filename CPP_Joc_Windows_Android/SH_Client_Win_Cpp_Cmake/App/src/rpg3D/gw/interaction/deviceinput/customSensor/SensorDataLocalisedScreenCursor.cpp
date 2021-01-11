#include "SensorDataLocalisedScreenCursor.h"
#include <base/input/util/Touch.h>

using namespace rpg3D;

const int SensorDataLocalisedScreenCursor::Type::Data = 1;

SensorDataLocalisedScreenCursor::SensorDataLocalisedScreenCursor(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, Vector2& screenPos)
	: touchEventType(touchEventType), cursorIndex(cursorIndex), buttonId(buttonId), 
	screenPos(screenPos)
{
	rawSubclassPointer = this;
}

SensorDataLocalisedScreenCursor::~SensorDataLocalisedScreenCursor() {
	//void
}
