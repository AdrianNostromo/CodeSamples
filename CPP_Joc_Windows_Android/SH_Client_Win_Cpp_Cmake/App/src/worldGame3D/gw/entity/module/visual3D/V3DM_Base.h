#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IVisual3DModule.h"

class Visual3DTemplate;

namespace worldGame3D {
class V3DM_Base : public EntityModule, virtual public IVisual3DModule {priv typedef EntityModule super;pub dCtor(V3DM_Base);
	pub Visual3DTemplate* t;
	
	pub explicit V3DM_Base(
		IWorldEntity* entity,
		Visual3DTemplate* t);

    pub ~V3DM_Base() override;
};
};
