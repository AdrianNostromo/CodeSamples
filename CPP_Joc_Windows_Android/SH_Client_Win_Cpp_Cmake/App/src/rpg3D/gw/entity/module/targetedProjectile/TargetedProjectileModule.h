#pragma once

#include "TPM_Base.h"
#include <memory>

namespace base {
	class GWEvent;
	class WrappedWorldEntity;
};
namespace rpg3D {
	class IPayloadReceiverTargetModule;
};

namespace rpg3D {
class TargetedProjectileModule : public TPM_Base {priv typedef TPM_Base super;pub dCtor(TargetedProjectileModule);
	priv std::shared_ptr<base::WrappedWorldEntity> targetEntity = nullptr;

	priv IPayloadReceiverTargetModule* targetEntity_projectileTarget = nullptr;

	priv bool isPayloadTransferred = false;

	pub explicit TargetedProjectileModule(
	    IWorldEntity* entity,
	    TargetedProjectileTemplate* t);

	priv void onEvent(IEventListener& eventListener, base::EntityEvent& event);

	pub void initTargetEntity(
		IWorldEntity* targetEntity) final;

	priv void onGWEvent_gameLoop_simulation(IEventListener& eventListener, base::GWEvent& event);

	pub ~TargetedProjectileModule() override;
};
};
