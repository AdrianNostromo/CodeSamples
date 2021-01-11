#include "GPTurnBasedPlay.h"

using namespace rpg3D;

GPTurnBasedPlay::GPTurnBasedPlay(
	base::IGameWorld* gw,
	TurnMode turnMode, sp<ArrayList<int>> turnBasedTeamsList)
	: super(
		gw,
		turnMode, turnBasedTeamsList
	)
{
	//void
}

GPTurnBasedPlay::~GPTurnBasedPlay() {
	//void
}
