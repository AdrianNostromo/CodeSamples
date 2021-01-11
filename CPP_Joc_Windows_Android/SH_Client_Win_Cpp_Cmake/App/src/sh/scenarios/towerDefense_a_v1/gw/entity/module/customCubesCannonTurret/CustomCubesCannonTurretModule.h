#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include "CCCTM_Reload.h"

class IWorldEntity;
namespace base {
	class WrappedWorldEntity;
};
namespace rpg3D {
	class IPayloadReceiverTargetModule;
};

namespace towerDefense_a_v1 {
class CustomCubesCannonTurretModule : public CCCTM_Reload {priv typedef CCCTM_Reload super;pub dCtor(CustomCubesCannonTurretModule);
	pub static int ActionGroup_AutoAttackTarget;

	priv class ActionSetAimTarget : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionSetAimTarget);
		pub static int TYPE;

		// If nullptr, the barrel will tilt up to the horrizon line.
		// If an entity and it got removed, the targeting stops instantly.
		pub std::shared_ptr<base::WrappedWorldEntity> aimTargetEnity;
		pub rpg3D::IPayloadReceiverTargetModule* aimTargetEnity_projectileTarget = nullptr;

		pub explicit ActionSetAimTarget(
			int localEntityId, int syncedEntityId,
			int priority,
			std::shared_ptr<base::WrappedWorldEntity> aimTargetEnity)
			: super(
				TYPE, "ActionSetAimTarget",
				true/*isSyncedToServer*/, -1/*persistenceDurationMS*/,
				localEntityId, syncedEntityId,
				priority/*priority*/,
				ActionGroup_AimToTarget,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_AimToTarget/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/),
			aimTargetEnity(aimTargetEnity)
		{
			rawSubclassPointer = this;
		}

		pub ~ActionSetAimTarget() override {};
	};
	priv class ActionFireProjectileAtTarget : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionFireProjectileAtTarget);
		pub static int TYPE;

		pub std::shared_ptr<base::WrappedWorldEntity> attackTargetEnity;
		
		pub explicit ActionFireProjectileAtTarget(
			int localEntityId, int syncedEntityId,
			int priority,
			std::shared_ptr<base::WrappedWorldEntity> attackTargetEnity)
			: super(
				TYPE, "ActionFireProjectileAtTarget",
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/,
				localEntityId, syncedEntityId,
				priority/*priority*/,
				0,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_Reload/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/),
			attackTargetEnity(attackTargetEnity)
		{
			rawSubclassPointer = this;
		}

		pub ~ActionFireProjectileAtTarget() override {};
	};

	priv bool isProjectileLoaded = true;

	pub CustomCubesCannonTurretModule(
        IWorldEntity* entity,
        CustomCubesCannonTurretTemplate* t,
		base::audio::ISound* activationSound);
	prot void createBMain() override;

	pub bool isPositionInAimAnglesRange(Vector3& pos, float maxYawOffset, float maxPitchOffset) final;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);
	
	pub void addAction_ActionSetAimTarget(IWorldEntity* aimTargetEnity) final;
	priv void onEntityAction_ActionSetAimTarget(ActionSetAimTarget* action);
	priv bool aimTick_turret(IWorldEntity* aimTargetEnity, ActionSetAimTarget* action, float deltaS);
	priv bool aimTick_barrel(IWorldEntity* aimTargetEnity, ActionSetAimTarget* action, float deltaS);

	pub void addAction_ActionFireProjectileAtTarget(IWorldEntity* attackTargetEnity) final;
	priv void onEntityAction_ActionFireProjectileAtTarget(ActionFireProjectileAtTarget* action);

    pub ~CustomCubesCannonTurretModule() override;
};
};
