#include "IToolWeapon.h"

using namespace rpg3D;

int IToolWeapon::GroupID = GetNew_GroupID();

int IToolWeapon::getGroupID() {
	return GroupID;
}
