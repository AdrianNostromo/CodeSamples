#pragma once

#include "VisualCreateConfig.h"

class VCC_Touchable : public VisualCreateConfig {priv typedef VisualCreateConfig super;pub dCtor(VCC_Touchable);
    pub class TouchShape {
        pub static const int Rectangle;
        pub static const int Circle;
    };

    pub int touchShape;

    pub explicit VCC_Touchable(const int type, const int touchShape);
    pub explicit VCC_Touchable(const int type);

    pub virtual ~VCC_Touchable();

};
