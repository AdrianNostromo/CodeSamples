#include "Touch.h"
#include <base/math/util/BoolUtil.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace base;

Touch::ButtonCode* const Touch::ButtonCode::None = newt Touch::ButtonCode(-1/*index*/, 0/*bitGroup*/);
Touch::ButtonCode* const Touch::ButtonCode::Btn_1 = newt Touch::ButtonCode(0/*index*/, BoolUtil::setBitAtIndexDirect(0, 0)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_2 = newt Touch::ButtonCode(1/*index*/, BoolUtil::setBitAtIndexDirect(0, 1)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_3 = newt Touch::ButtonCode(2/*index*/, BoolUtil::setBitAtIndexDirect(0, 2)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_4 = newt Touch::ButtonCode(3/*index*/, BoolUtil::setBitAtIndexDirect(0, 3)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_5 = newt Touch::ButtonCode(4/*index*/, BoolUtil::setBitAtIndexDirect(0, 4)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_6 = newt Touch::ButtonCode(5/*index*/, BoolUtil::setBitAtIndexDirect(0, 5)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_7 = newt Touch::ButtonCode(6/*index*/, BoolUtil::setBitAtIndexDirect(0, 6)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_8 = newt Touch::ButtonCode(7/*index*/, BoolUtil::setBitAtIndexDirect(0, 7)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_9 = newt Touch::ButtonCode(8/*index*/, BoolUtil::setBitAtIndexDirect(0, 8)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_10 = newt Touch::ButtonCode(9/*index*/, BoolUtil::setBitAtIndexDirect(0, 9)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_11 = newt Touch::ButtonCode(10/*index*/, BoolUtil::setBitAtIndexDirect(0, 10)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_12 = newt Touch::ButtonCode(11/*index*/, BoolUtil::setBitAtIndexDirect(0, 11)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_13 = newt Touch::ButtonCode(12/*index*/, BoolUtil::setBitAtIndexDirect(0, 12)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_14 = newt Touch::ButtonCode(13/*index*/, BoolUtil::setBitAtIndexDirect(0, 13)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_15 = newt Touch::ButtonCode(14/*index*/, BoolUtil::setBitAtIndexDirect(0, 14)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_16 = newt Touch::ButtonCode(15/*index*/, BoolUtil::setBitAtIndexDirect(0, 15)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_17 = newt Touch::ButtonCode(16/*index*/, BoolUtil::setBitAtIndexDirect(0, 16)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_18 = newt Touch::ButtonCode(17/*index*/, BoolUtil::setBitAtIndexDirect(0, 17)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_19 = newt Touch::ButtonCode(18/*index*/, BoolUtil::setBitAtIndexDirect(0, 18)/*bitGroup*/ );
Touch::ButtonCode* const Touch::ButtonCode::Btn_20 = newt Touch::ButtonCode(19/*index*/, BoolUtil::setBitAtIndexDirect(0, 19)/*bitGroup*/ );

Touch::ButtonCode* const Touch::ButtonCode::IndexedButtons[20]{
	Touch::ButtonCode::Btn_1,
	Touch::ButtonCode::Btn_2,
	Touch::ButtonCode::Btn_3,
	Touch::ButtonCode::Btn_4,
	Touch::ButtonCode::Btn_5,
	Touch::ButtonCode::Btn_6,
	Touch::ButtonCode::Btn_7,
	Touch::ButtonCode::Btn_8,
	Touch::ButtonCode::Btn_9,
	Touch::ButtonCode::Btn_10,
	Touch::ButtonCode::Btn_11,
	Touch::ButtonCode::Btn_12,
	Touch::ButtonCode::Btn_13,
	Touch::ButtonCode::Btn_14,
	Touch::ButtonCode::Btn_15,
	Touch::ButtonCode::Btn_16,
	Touch::ButtonCode::Btn_17,
	Touch::ButtonCode::Btn_18,
	Touch::ButtonCode::Btn_19,
	Touch::ButtonCode::Btn_20
};

Touch::ButtonCode* const Touch::ButtonCode::LeftOrTouch = Touch::ButtonCode::Btn_1;
Touch::ButtonCode* const Touch::ButtonCode::Right = Touch::ButtonCode::Btn_2;


// Normal mouse mode with a cursor.
Touch::CursorMode* const Touch::CursorMode::Normal = newt Touch::CursorMode(1/*index*/, BoolUtil::setBitAtIndexDirect(0, 0)/*bitGroup*/ );
// This locks the mouse cursor to the window, hides the mouse cursor.
// Uses continuous mouse position.
// The current system will dispatch on a custom callback function.
Touch::CursorMode* const Touch::CursorMode::Locked = newt Touch::CursorMode(2/*index*/, BoolUtil::setBitAtIndexDirect(0, 1)/*bitGroup*/ );


Touch::ToolType* const Touch::ToolType::None = newt Touch::ToolType(0/*index*/, 0/*bitGroup*/ );
Touch::ToolType* const Touch::ToolType::Mouse = newt Touch::ToolType(1/*index*/, BoolUtil::setBitAtIndexDirect(0, 0)/*bitGroup*/ );
Touch::ToolType* const Touch::ToolType::TouchScreen = newt Touch::ToolType(2/*index*/, BoolUtil::setBitAtIndexDirect(0, 1)/*bitGroup*/ );


Touch::SensorType* const Touch::SensorType::Accelerometer = newt Touch::SensorType(1/*index*/, BoolUtil::setBitAtIndexDirect(0, 0)/*bitGroup*/ );
Touch::SensorType* const Touch::SensorType::DeviceOrientation = newt Touch::SensorType(2/*index*/, BoolUtil::setBitAtIndexDirect(0, 1)/*bitGroup*/ );
Touch::SensorType* const Touch::SensorType::RotationsPitchRoll_Oriented = newt Touch::SensorType(3/*index*/, BoolUtil::setBitAtIndexDirect(0, 2)/*bitGroup*/ );
Touch::SensorType* const Touch::SensorType::Cursor = newt Touch::SensorType(4/*index*/, BoolUtil::setBitAtIndexDirect(0, 3)/*bitGroup*/ );
Touch::SensorType* const Touch::SensorType::Keyboard = newt Touch::SensorType(5/*index*/, BoolUtil::setBitAtIndexDirect(0, 4)/*bitGroup*/ );
Touch::SensorType* const Touch::SensorType::InputHandler = newt Touch::SensorType(6/*index*/, BoolUtil::setBitAtIndexDirect(0, 5)/*bitGroup*/ );


Touch::DeviceOrientation* const Touch::DeviceOrientation::Undefined = newt Touch::DeviceOrientation(-1/*index*/, 0/*bitGroup*/ );
Touch::DeviceOrientation* const Touch::DeviceOrientation::Portrait = newt Touch::DeviceOrientation(0/*index*/, BoolUtil::setBitAtIndexDirect(0, 0)/*bitGroup*/ );
Touch::DeviceOrientation* const Touch::DeviceOrientation::Landscape = newt Touch::DeviceOrientation(1/*index*/, BoolUtil::setBitAtIndexDirect(0, 1)/*bitGroup*/ );
Touch::DeviceOrientation* const Touch::DeviceOrientation::ReversePortrait = newt Touch::DeviceOrientation(2/*index*/, BoolUtil::setBitAtIndexDirect(0, 2)/*bitGroup*/ );
Touch::DeviceOrientation* const Touch::DeviceOrientation::ReverseLandscape = newt Touch::DeviceOrientation(3/*index*/, BoolUtil::setBitAtIndexDirect(0, 3)/*bitGroup*/ );
