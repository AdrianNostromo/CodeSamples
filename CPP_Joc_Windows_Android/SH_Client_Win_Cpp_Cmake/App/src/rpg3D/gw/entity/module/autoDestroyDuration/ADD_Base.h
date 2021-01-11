#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IAutoDestroyDurationModule.h"

class EntityRemoveReason;
namespace rpg3D {
	class AutoDestroyDurationTemplate;
};

namespace rpg3D {
class ADD_Base : public EntityModule, virtual public IAutoDestroyDurationModule {priv typedef EntityModule super; pub dCtor(ADD_Base);
	prot static EntityRemoveReason* const ENT_REM_REASON_PURPOSE_FINISHED;
	
	pub AutoDestroyDurationTemplate* t;

	pub ADD_Base(
		IWorldEntity* entity,
		AutoDestroyDurationTemplate* t);
	prot void createBMain() override;

	pub ~ADD_Base() override;
};
};
