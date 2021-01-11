#include "MovementConfigDirect.h"

using namespace rpg3D;

const int MovementConfigDirect::TYPE = super::NewType();

MovementConfigDirect::MovementConfigDirect(float moveSpeedS)
	: super(TYPE),
	moveSpeedS(moveSpeedS)
{
	rawSubclassPointer = this;
}

MovementConfigDirect::~MovementConfigDirect() {
	//void
}
