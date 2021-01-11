#pragma once

#include <base/math/Vector3.h>
#include <base/gh.h>
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include <base/list/ArrayList.h>
#include <rpg3D/gw/projectile/EffectPayload.h>
#include <rpg3D/gw/entity/template/grid2DContent/Grid2DContentTemplate.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IGrid2DContentModule.h"
#include <base/audio/sound/ISound.h>

namespace rpg3D {
class G2DCM_Base : public EntityModule, virtual public IGrid2DContentModule {priv typedef EntityModule super;pub dCtor(G2DCM_Base);
	prot Grid2DContentTemplate* t;

	pub explicit G2DCM_Base(IWorldEntity* entity, Grid2DContentTemplate* t);
	
	pub ~G2DCM_Base() override;
};
};
