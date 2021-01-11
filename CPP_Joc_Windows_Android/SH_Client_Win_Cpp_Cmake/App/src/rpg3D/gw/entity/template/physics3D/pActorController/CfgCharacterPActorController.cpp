#include "CfgCharacterPActorController.h"

using namespace rpg3D;

int CfgCharacterPActorController::TYPE = GetNew_TYPE();

CfgCharacterPActorController::CfgCharacterPActorController()
	: super(TYPE)
{
	rawTopSubclassPointer = this;
}

CfgCharacterPActorController::~CfgCharacterPActorController() {
	//void
}
