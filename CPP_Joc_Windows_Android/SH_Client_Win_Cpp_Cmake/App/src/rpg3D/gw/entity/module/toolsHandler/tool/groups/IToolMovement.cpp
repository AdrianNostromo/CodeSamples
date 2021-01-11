#include "IToolMovement.h"

using namespace rpg3D;

int IToolMovement::GroupID = GetNew_GroupID();

int IToolMovement::getGroupID() {
	return GroupID;
}
