#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IWorldItemsCollectorModule.h"

class EntityRemoveReason;
namespace rpg3D {
	class WorldItemsCollectorTemplate;
	class IPhysics3DModule;
	class IPhysicsEntityCallbackFilter;
};

namespace rpg3D {
class WICM_Base : public EntityModule, virtual public IWorldItemsCollectorModule {priv typedef EntityModule super; pub dCtor(WICM_Base);
	pub WorldItemsCollectorTemplate* t;

	priv rpg3D::IPhysics3DModule* physics3DModule = nullptr;
	priv IPhysicsEntityCallbackFilter* activeWorldItemFilter = nullptr;

	pub WICM_Base(
		IWorldEntity* entity,
		WorldItemsCollectorTemplate* t);
	prot void createBMain() override;

	priv void onWorldItemContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact);

	pub ~WICM_Base() override;
};
};
