#include "ToolActivationInfo_Base.h"

using namespace rpg3D;

int ToolActivationInfo_Base::NewType() {
	static int TYPE_COUNTER = 0;

	TYPE_COUNTER++;
	return TYPE_COUNTER;
}

ToolActivationInfo_Base::ToolActivationInfo_Base(int type, int cursorIndex)
	: type(type),
	cursorIndex(cursorIndex)
{
	//void
}

int ToolActivationInfo_Base::getType() {
	return type;
}

void ToolActivationInfo_Base::clearCumulators() {
	//void
}

//asd_01;// This may be needed to avoid inputs dropping if on the same frame.
//void ToolActivationInfo_Base::addCumulators(ToolActivationInfo_Base* other) {
//	//void
//}

ToolActivationInfo_Base::~ToolActivationInfo_Base() {
	//void
}
