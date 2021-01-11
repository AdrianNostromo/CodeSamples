#include "GPTBP_Base.h"

using namespace rpg3D;

GPTBP_Base::GPTBP_Base(
	base::IGameWorld* gw,
	TurnMode turnMode, sp<ArrayList<int>> turnBasedTeamsList)
	: super(gw),
	turnMode(turnMode), turnBasedTeamsList(turnBasedTeamsList)
{
	//void
}

GPTBP_Base::~GPTBP_Base() {
	//void
}
