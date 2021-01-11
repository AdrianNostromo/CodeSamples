#include "CfgPActorController.h"

using namespace rpg3D;

int CfgPActorController::GetNew_TYPE() {
	static int COUNTER = 0;

	int ret = COUNTER;
	COUNTER++;

	return ret;
}

CfgPActorController::CfgPActorController(int type)
	: type(type)
{
	//void
}

int CfgPActorController::getType() {
	return type;
}

CfgPActorController::~CfgPActorController() {
	//void
}
