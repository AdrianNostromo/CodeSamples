#include "IEntityModule.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>

int IEntityModule::ActionGroup_Stoppable = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

int IEntityModule::GetNew_ID() {
	static int ID_COUNTER = 0;

	int ret = ID_COUNTER;
	ID_COUNTER++;

	return ret;
}

int IEntityModule::getId() {
	return -1;
}
