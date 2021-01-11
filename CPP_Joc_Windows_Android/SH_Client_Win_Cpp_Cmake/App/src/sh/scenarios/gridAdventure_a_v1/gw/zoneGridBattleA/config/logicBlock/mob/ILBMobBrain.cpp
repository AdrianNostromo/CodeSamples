#include "ILBMobBrain.h"

using namespace gridAdventure_a_v1;
using namespace gridAdventure_a_v1::GridBattleA;

int ILBMobBrain::GroupID = GetNew_GroupID();

int ILBMobBrain::getGroupID() {
	return GroupID;
}
