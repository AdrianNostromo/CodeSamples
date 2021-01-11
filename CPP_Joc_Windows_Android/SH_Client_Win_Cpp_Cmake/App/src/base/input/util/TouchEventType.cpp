#include "TouchEventType.h"
#include <base/math/util/BoolUtil.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

TouchEventType* const TouchEventType::CURSOR_DOWN = newt TouchEventType(1/*index*/, BoolUtil::setBitAtIndexDirect(0, 0)/*bitGroup*/);

TouchEventType* const TouchEventType::CURSOR_UP = newt TouchEventType(2/*index*/, BoolUtil::setBitAtIndexDirect(0, 1)/*bitGroup*/);
TouchEventType* const TouchEventType::CURSOR_UP_OUTSIDE = newt TouchEventType(3/*index*/, BoolUtil::setBitAtIndexDirect(0, 2)/*bitGroup*/);

// Drag is per button, that is why move is dispatched independently of drag events.
TouchEventType* const TouchEventType::CURSOR_DRAG = newt TouchEventType(4/*index*/, BoolUtil::setBitAtIndexDirect(0, 3)/*bitGroup*/);

// Drag is per button, that is why move is dispatched independently of drag events.
TouchEventType* const TouchEventType::CURSOR_MOVE = newt TouchEventType(5/*index*/, BoolUtil::setBitAtIndexDirect(0, 4)/*bitGroup*/);

TouchEventType* const TouchEventType::CURSOR_OVER = newt TouchEventType(6/*index*/, BoolUtil::setBitAtIndexDirect(0, 5)/*bitGroup*/);
TouchEventType* const TouchEventType::CURSOR_OUT = newt TouchEventType(7/*index*/, BoolUtil::setBitAtIndexDirect(0, 6)/*bitGroup*/);
