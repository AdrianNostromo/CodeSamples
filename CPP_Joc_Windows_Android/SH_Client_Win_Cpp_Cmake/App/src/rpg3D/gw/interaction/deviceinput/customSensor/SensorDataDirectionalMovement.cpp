#include "SensorDataDirectionalMovement.h"
#include <base/input/util/Touch.h>

using namespace rpg3D;

const int SensorDataDirectionalMovement::Type::Data = 1;

SensorDataDirectionalMovement::SensorDataDirectionalMovement(Vector2& moveDirInput)
	: moveDirInput(moveDirInput)
{
	rawSubclassPointer = this;
}

SensorDataDirectionalMovement::~SensorDataDirectionalMovement() {
	//void
}
