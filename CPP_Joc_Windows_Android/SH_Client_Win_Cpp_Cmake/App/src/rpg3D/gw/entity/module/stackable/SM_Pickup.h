#pragma once

#include <base/gh.h>
#include "SM_Base.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>

class IMovingEntityModule;
namespace base {
	class GWEvent;
};
namespace worldGame3D {
	class IActionsModule;
};

namespace rpg3D {
class SM_Pickup : public SM_Base {priv typedef SM_Base super; pub dCtor(SM_Pickup)
	priv class StackablePickupActionContinuous : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(StackablePickupActionContinuous);
		pub class States {
			pub static const int Start;
			pub static const int ManagedMoveInRange;
			pub static const int ManagedTurnToTarget;
			pub static const int Pickup;
			pub static const int Finished;
		};

		pub static int TYPE;

		pub std::shared_ptr<base::WrappedWorldEntity> targetEntity;
		
		pub float actionRange;

		// Data.
		pub int state = States::Start;
		pub bool isStateInit = false;

		pub explicit StackablePickupActionContinuous(
			int localEntityId, int syncedEntityId,
			int priority,
			std::shared_ptr<base::WrappedWorldEntity>& targetEntity,
			float actionRange);

		pub ~StackablePickupActionContinuous() override;
	};

	priv worldGame3D::IActionsModule* actions = nullptr;
	priv IMovingEntityModule* movingEntity = nullptr;

    pub explicit SM_Pickup(
		IWorldEntity* entity,
		StackableTemplate* t);
	prot void createBMain() override;

	// actionRange <0; Inifnite.
	pub void addAction_StackablePickupActionContinuous(std::shared_ptr<base::WrappedWorldEntity> targetEntity, float actionRange) override;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);
	priv void onEntityAction_StackablePickupActionContinuous(StackablePickupActionContinuous* action);

    pub ~SM_Pickup() override;
};
};
