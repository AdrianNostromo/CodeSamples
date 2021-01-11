#include "SensorDataKeyboard.h"
#include <base/input/util/Touch.h>

using namespace base;

const int SensorDataKeyboard::Type::Key = 1;

SensorDataKeyboard::SensorDataKeyboard(int keyCode, int actionType, int modifierBits)
	: keyCode(keyCode), actionType(actionType), modifierBits(modifierBits)
{
	rawSubclassPointer = this;
}

SensorDataKeyboard::~SensorDataKeyboard() {
	//void
}
