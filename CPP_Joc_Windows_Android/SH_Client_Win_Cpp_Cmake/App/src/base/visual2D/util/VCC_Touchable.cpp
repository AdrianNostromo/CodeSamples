#include "VCC_Touchable.h"

const int VCC_Touchable::TouchShape::Rectangle = 1;
const int VCC_Touchable::TouchShape::Circle = 2;

VCC_Touchable::VCC_Touchable(const int type, const int touchShape)
    : super(type),
    touchShape(touchShape)
{
    //void
}

VCC_Touchable::VCC_Touchable(const int type)
    : VCC_Touchable(type, TouchShape::Rectangle)
{
    //void
}

VCC_Touchable::~VCC_Touchable() {
    //void
}
