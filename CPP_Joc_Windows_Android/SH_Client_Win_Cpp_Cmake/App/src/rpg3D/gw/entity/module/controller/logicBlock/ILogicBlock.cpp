#include "ILogicBlock.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>

using namespace rpg3D;

int ILogicBlock::GetNew_ID() {
	static int ID_COUNTER = 0;

	int ret = ID_COUNTER;
	ID_COUNTER++;

	return ret;
}

int ILogicBlock::GetNew_GroupID() {
	static int GroupID_COUNTER = 0;

	int ret = GroupID_COUNTER;
	GroupID_COUNTER++;

	return ret;
}
