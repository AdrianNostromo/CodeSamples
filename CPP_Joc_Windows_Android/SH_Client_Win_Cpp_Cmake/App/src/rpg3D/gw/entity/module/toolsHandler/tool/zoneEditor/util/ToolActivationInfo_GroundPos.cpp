#include "ToolActivationInfo_GroundPos.h"

using namespace rpg3D;

const int ToolActivationInfo_GroundPos::TYPE = super::NewType();

ToolActivationInfo_GroundPos::ToolActivationInfo_GroundPos(int cursorIndex, Vector3& groundPos)
	: super(TYPE, cursorIndex),
	groundPos(groundPos)
{
	rawSubclassPointer = this;
}

ToolActivationInfo_GroundPos::~ToolActivationInfo_GroundPos() {
	//void
}
