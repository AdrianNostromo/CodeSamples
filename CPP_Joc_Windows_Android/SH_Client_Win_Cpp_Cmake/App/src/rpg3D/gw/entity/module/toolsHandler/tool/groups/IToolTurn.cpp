#include "IToolTurn.h"

using namespace rpg3D;

int IToolTurn::GroupID = GetNew_GroupID();

int IToolTurn::getGroupID() {
	return GroupID;
}
