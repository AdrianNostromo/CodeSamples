#include "ILB_GridMob_A_V1.h"

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::mobUnit;

int ILB_GridMob_A_V1::ID = GetNew_ID();

int ILB_GridMob_A_V1::getId() {
	return ID;
}

int ILB_GridMob_A_V1::GroupID = GetNew_GroupID();

int ILB_GridMob_A_V1::getGroupID() {
	return GroupID;
}
