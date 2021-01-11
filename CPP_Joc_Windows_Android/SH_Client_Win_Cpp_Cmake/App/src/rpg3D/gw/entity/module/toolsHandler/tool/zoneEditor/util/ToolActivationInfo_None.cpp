#include "ToolActivationInfo_None.h"

using namespace rpg3D;

const int ToolActivationInfo_None::TYPE = super::NewType();

ToolActivationInfo_None::ToolActivationInfo_None(int cursorIndex)
	: super(TYPE, cursorIndex)
{
	rawSubclassPointer = this;
}

ToolActivationInfo_None::~ToolActivationInfo_None() {
	//void
}
