#pragma once

#include <base/gh.h>
#include "CfgPActorController.h"
#include <base/math/Vector3.h>

namespace rpg3D {
class CfgCharacterPActorController : public CfgPActorController {priv typedef CfgPActorController super;pub dCtor(CfgCharacterPActorController);
	pub static int TYPE;

    pub explicit CfgCharacterPActorController();

    pub ~CfgCharacterPActorController() override;
};
};
