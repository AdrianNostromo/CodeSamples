#pragma once

#include <base/gh.h>
#include "P3DM_Contacts.h"

namespace rpg3D {
class Physics3DModule : public P3DM_Contacts {priv typedef P3DM_Contacts super;pub dCtor(Physics3DModule);
	pub explicit Physics3DModule(
		IWorldEntity* entity,
		Physics3DTemplate* t,
		int physicsGroupsBitMap,
		int anyInteraction_maskBitMap, int hardImpact_maskBitMap);

    pub ~Physics3DModule() override;
};
};
