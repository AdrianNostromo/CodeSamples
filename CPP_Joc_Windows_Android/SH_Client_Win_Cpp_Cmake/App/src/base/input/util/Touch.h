#pragma once

#include <base/gh.h>

namespace base {
class Touch {
    pub class ButtonCode {pub dCtor(ButtonCode);
        pub static ButtonCode* const None;

        pub static ButtonCode* const Btn_1;
        pub static ButtonCode* const Btn_2;
        pub static ButtonCode* const Btn_3;
        pub static ButtonCode* const Btn_4;
        pub static ButtonCode* const Btn_5;
        pub static ButtonCode* const Btn_6;
        pub static ButtonCode* const Btn_7;
        pub static ButtonCode* const Btn_8;
        pub static ButtonCode* const Btn_9;
        pub static ButtonCode* const Btn_10;
        pub static ButtonCode* const Btn_11;
        pub static ButtonCode* const Btn_12;
        pub static ButtonCode* const Btn_13;
        pub static ButtonCode* const Btn_14;
        pub static ButtonCode* const Btn_15;
        pub static ButtonCode* const Btn_16;
        pub static ButtonCode* const Btn_17;
        pub static ButtonCode* const Btn_18;
        pub static ButtonCode* const Btn_19;
        pub static ButtonCode* const Btn_20;

        pub static ButtonCode* const IndexedButtons[20];

        pub static ButtonCode* const LeftOrTouch;
        pub static ButtonCode* const Right;

        pub const int index;
        pub const int bitGroup;

        pub explicit ButtonCode(const int index, const int bitGroup)
            : index(index), bitGroup(bitGroup) {}
    };

    pub class CursorMode {pub dCtor(CursorMode);
        // Normal mouse mode with a cursor.
        pub static CursorMode* const Normal;
        // This locks the mouse cursor to the window, hides the mouse cursor.
        // Uses continuous mouse position.
        // The current system will dispatch on a custom callback function.
        pub static CursorMode* const Locked;

        pub const int index;
        pub const int bitGroup;

        pub explicit CursorMode(const int index, const int bitGroup)
            : index(index), bitGroup(bitGroup) {}
    };

    pub class ToolType {pub dCtor(ToolType);
        pub static ToolType* const None;

        pub static ToolType* const Mouse;
        pub static ToolType* const TouchScreen;

        pub const int index;
        pub const int bitGroup;

        pub explicit ToolType(const int index, const int bitGroup)
            : index(index), bitGroup(bitGroup) {}
    };

    pub class SensorType {pub dCtor(SensorType);
        pub static SensorType* const Accelerometer;
        pub static SensorType* const DeviceOrientation;
        pub static SensorType* const RotationsPitchRoll_Oriented;
        pub static SensorType* const Cursor;
        pub static SensorType* const Keyboard;
        
        pub static SensorType* const InputHandler;

        pub const int index;
        pub const int bitGroup;

        pub explicit SensorType(const int index, const int bitGroup)
            : index(index), bitGroup(bitGroup) {}
    };

    pub class DeviceOrientation {pub dCtor(DeviceOrientation);
        pub static DeviceOrientation* const Undefined;

        pub static DeviceOrientation* const Portrait;
        // Portrait + 90 degrees counter-clockwise.
        // Like holding the phone in the right hand and naturally turning the hand to the left.
        pub static DeviceOrientation* const Landscape;
        pub static DeviceOrientation* const ReversePortrait;
        pub static DeviceOrientation* const ReverseLandscape;

        pub const int index;
        pub const int bitGroup;

        pub explicit DeviceOrientation(const int index, const int bitGroup)
            : index(index), bitGroup(bitGroup) {}
    };
};
};
