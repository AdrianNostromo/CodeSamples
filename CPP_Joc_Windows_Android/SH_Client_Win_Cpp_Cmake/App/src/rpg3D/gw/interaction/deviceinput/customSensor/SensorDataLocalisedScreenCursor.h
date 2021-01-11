#pragma once

#include <base/gh.h>
#include <base/input/sensor/data/SensorData.h>
#include <base/math/Vector2.h>
#include <base/input/util/Touch.h>
#include <base/input/util/TouchEventType.h>

namespace rpg3D {
class SensorDataLocalisedScreenCursor : public base::SensorData {priv typedef base::SensorData super;pub dCtor(SensorDataLocalisedScreenCursor);
    pub class Type {
        pub static const int Data;
    };

	pub TouchEventType* touchEventType;
	pub int cursorIndex;
	pub base::Touch::ButtonCode* buttonId;

	pub Vector2 screenPos;

    pub explicit SensorDataLocalisedScreenCursor(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, Vector2& screenPos);

    pub ~SensorDataLocalisedScreenCursor() override;
};
};
