#pragma once

#include <base/gh.h>
#include "MEMM_Base.h"
#include <base/math/Vector3.h>

namespace worldGame3D {
	class IActionsModule;
};
namespace rpg3D {
	class IPhysics3DModule;
}
class ILookAtModule;

class MEMM_BasicMovements : public rpg3D::MEMM_Base {priv typedef rpg3D::MEMM_Base super;pub dCtor(MEMM_BasicMovements);
	priv class ActionTeleportToDestination : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionTeleportToDestination);
		pub static int TYPE;

		pub Vector3 destPos;

		pub explicit ActionTeleportToDestination(
			int localEntityId, int syncedEntityId,
			int priority,
			Vector3& destPos);

		pub ~ActionTeleportToDestination() override;
	};
	priv class ActionStopMovement : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionStopMovement);
		pub static int TYPE;

		bool stopPhysicsLinearVelocitiesAlso;

		pub explicit ActionStopMovement(
			int localEntityId, int syncedEntityId,
			int priority,
			bool stopPhysicsLinearVelocitiesAlso);

		pub ~ActionStopMovement() override;
	};
	priv class ActionGeneralStop : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionGeneralStop);
		pub static int TYPE;

		pub explicit ActionGeneralStop(
			int localEntityId, int syncedEntityId,
			int priority);

		pub ~ActionGeneralStop() override;
	};
	priv class ActionCustomStop : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionCustomStop);
		pub static int TYPE;

		pub explicit ActionCustomStop(
			int localEntityId, int syncedEntityId,
			int priority,
			int extraReplaceGroupsBitMask);

		pub ~ActionCustomStop() override;
	};
	
	priv worldGame3D::IActionsModule* actions = nullptr;
	priv ILookAtModule* lookAt = nullptr;
	priv rpg3D::IPhysics3DModule* physics3DModule = nullptr;

	pub explicit MEMM_BasicMovements(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);
	prot void createBMain() override;

	pub void addAction_teleportToDestination(Vector3& destPos) final;

	pub void addAction_stopMovement(bool stopPhysicsLinearVelocitiesAlso) final;
	pub void addAction_ActionGeneralStop() final;
	pub void addAction_ActionCustomStop(
		int priority,
		int extraReplaceGroupsBitMask) final;

	prot void disposePre() override;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);

	priv void onEntityAction_ActionTeleportToDestination(ActionTeleportToDestination* action);
	priv void onEntityAction_ActionStopMovement(ActionStopMovement* action);
	priv void onEntityAction_ActionGeneralStop(ActionGeneralStop* action);
	priv void onEntityAction_ActionCustomStop(ActionCustomStop* action);

    pub ~MEMM_BasicMovements() override;
};
