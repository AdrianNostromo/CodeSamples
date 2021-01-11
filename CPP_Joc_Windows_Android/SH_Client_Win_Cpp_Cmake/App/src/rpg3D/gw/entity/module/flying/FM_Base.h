#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IFlyingModule.h"

namespace worldGame3D {
	class IActionsModule;
};
namespace rpg3D {
	class FlyingTemplate;
	class IPhysics3DModule;
};

namespace rpg3D {
class FM_Base : public EntityModule, virtual public IFlyingModule {priv typedef EntityModule super; pub dCtor(FM_Base);
	pub FlyingTemplate* t;

	prot worldGame3D::IActionsModule* actions = nullptr;
	prot rpg3D::IPhysics3DModule* physics3DModule = nullptr;

	pub FM_Base(
		IWorldEntity* entity,
		FlyingTemplate* t);
	prot void createBMain() override;

	pub ~FM_Base() override;
};
};
