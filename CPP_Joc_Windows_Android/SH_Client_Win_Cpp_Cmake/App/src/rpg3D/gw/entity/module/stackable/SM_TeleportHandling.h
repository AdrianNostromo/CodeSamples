#pragma once

#include <base/gh.h>
#include "SM_Throw.h"

namespace rpg3D {
class SM_TeleportHandling : public SM_Throw {priv typedef SM_Throw super;pub dCtor(SM_TeleportHandling);
    pub explicit SM_TeleportHandling(
		IWorldEntity* entity,
		StackableTemplate* t);
	prot void createMain() override;

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

    pub ~SM_TeleportHandling() override;
};
};
