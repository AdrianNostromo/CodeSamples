#pragma once

#include <base/gh.h>
#include "SM_Pickup.h"
#include <base/math/Vector3.h>

class IMovingEntityModule;
namespace worldGame3D {
	class IActionsModule;
};

namespace rpg3D {
class SM_Throw : public SM_Pickup {priv typedef SM_Pickup super; pub dCtor(SM_Throw);
	priv class StackableThrowActionContinuous : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(StackableThrowActionContinuous);
		pub class States {
			pub static const int Start;
			pub static const int ManagedMoveInRange;
			pub static const int ManagedTurnToTarget;
			pub static const int Throw;
			pub static const int Finished;
		};

		pub static int TYPE;

		pub std::shared_ptr<base::WrappedWorldEntity> carriedEntityToThrow;
		
		// Optional.
		pub std::shared_ptr<base::WrappedWorldEntity> throwDestinationHaulerEntity;
		// Offset if throwDestinationHaulerEntity used; else this is a world pos.
		pub std::shared_ptr<Vector3> throwPosOrOffset;

		pub float actionRange;
		
		// Data.
		pub int state = States::Start;
		pub bool isStateInit = false;

		pub std::shared_ptr<float> throwRotZDeg;

		pub explicit StackableThrowActionContinuous(
			int localEntityId, int syncedEntityId,
			int priority,
			std::shared_ptr<base::WrappedWorldEntity>& carriedEntityToThrow,
			std::shared_ptr<base::WrappedWorldEntity>& throwDestinationHaulerEntity, std::shared_ptr<Vector3>& throwPosOrOffset,
			float actionRange,
			std::shared_ptr<float>& throwRotZDeg);

		pub ~StackableThrowActionContinuous() override;
	};

	priv worldGame3D::IActionsModule* actions = nullptr;
	priv IMovingEntityModule* movingEntity = nullptr;

    pub explicit SM_Throw(
		IWorldEntity* entity,
		StackableTemplate* t);
	prot void createBMain() override;

	// If throwPosOrOffset is used, the throw occurs on the ground.
	pub void addAction_StackableThrowActionContinuous(
		std::shared_ptr<base::WrappedWorldEntity>& carriedEntityToThrow,
		std::shared_ptr<base::WrappedWorldEntity> throwDestinationHaulerEntity/*optional*/, std::shared_ptr<Vector3> throwPosOrOffset,
		float throwRange,
		std::shared_ptr<float> throwRotZDeg) override;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);
	priv void onEntityAction_StackableThrowActionContinuous(StackableThrowActionContinuous* action);

    pub ~SM_Throw() override;
};
};
