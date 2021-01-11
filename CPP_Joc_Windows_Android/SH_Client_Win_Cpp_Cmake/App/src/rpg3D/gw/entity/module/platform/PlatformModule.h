#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/template/platform/PlatformTemplate.h>
#include "IPlatformModule.h"
#include <base/math/Quaternion.h>
#include <base/math/Bounds2D.h>
#include <base/list/ArrayList.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>

class IWorldEntity;

namespace rpg3D {
	class IPhysics3DModule;
	class IPhysicsEntityCallbackFilter;
};

namespace rpg3D {
class PlatformModule : public EntityModule, public virtual IPlatformModule {priv typedef EntityModule super;pub dCtor(PlatformModule);
	pub static base::EntityEvent* EVENT_onPlatformContactAdded;
	pub static base::EntityEvent* EVENT_onPlatformContactRemoved;
	pub static base::EntityEvent* EVENT_Platform_activeStateChanged; 

	pub PlatformTemplate* t;

	priv rpg3D::IPhysics3DModule* physics3DModule = nullptr;

	priv IPhysicsEntityCallbackFilter* activeFilter = nullptr;

	prot std::shared_ptr<std::string> activation_script = nullptr;

	pub explicit PlatformModule(
        IWorldEntity* entity,
        PlatformTemplate* t);
	prot void createBMain() override;

	priv void onPlatformInteractorContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact);

	pub bool getIsActive() final;

	pub std::shared_ptr<std::string> getActivation_script() final;
	pub void setActivation_script(std::shared_ptr<std::string> activation_script) final;

	pub ~PlatformModule() override;
};
};
