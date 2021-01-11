#pragma once

#include <base/gh.h>

class TouchEventType {pub dCtor(TouchEventType);
    pub static TouchEventType* const CURSOR_DOWN;

	pub static TouchEventType* const CURSOR_UP;
	pub static TouchEventType* const CURSOR_UP_OUTSIDE;

	// Drag is per button, that is why move is dispatched independently of drag events.
	pub static TouchEventType* const CURSOR_DRAG;

	// Drag is per button, that is why move is dispatched independently of drag events.
	pub static TouchEventType* const CURSOR_MOVE;

	pub static TouchEventType* const CURSOR_OVER;
	pub static TouchEventType* const CURSOR_OUT;

	pub const int index;
	pub const int bitGroup;

	pub explicit TouchEventType(const int index, const int bitGroup)
		: index(index), bitGroup(bitGroup) {}
};
