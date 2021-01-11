#pragma once

#include <base/gh.h>
#include "FM_Base.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include <memory>

namespace base {
	class WrappedWorldEntity;
};

namespace rpg3D {
class FM_GeneralFlight : public FM_Base {priv typedef FM_Base super;pub dCtor(FM_GeneralFlight);
	pub static int ActionGroup_GeneralFlight;

	priv class ActionDirectionFlight : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionDirectionFlight);
		pub class Phases {
			pub static const int INITIAL_ROTATION;
			pub static const int LOWERING;
			pub static const int RISING;
			pub static const int FINISHED;
		};
		pub static int TYPE;

		// [0.0f, 1.0f]
		pub Vector3 velocityDir;

		pub explicit ActionDirectionFlight(
			int localEntityId, int syncedEntityId,
			int priority,
			Vector3& velocityDir)
			: super(
				TYPE, "ActionDirectionFlight",
				true/*isSyncedToServer*/, -1/*persistenceDurationMS*/,
				localEntityId, syncedEntityId,
				priority/*priority*/,
				ActionGroup_GeneralFlight,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_GeneralFlight/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/),
			velocityDir(velocityDir)
		{
			rawSubclassPointer = this;
		}

		pub ~ActionDirectionFlight() override {};
	};
	priv class ActionApplyImpulse : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionApplyImpulse);
		pub class Phases {
			pub static const int INITIAL_ROTATION;
			pub static const int LOWERING;
			pub static const int RISING;
			pub static const int FINISHED;
		};
		pub static int TYPE;

		pub Vector3 dir;
		pub float physicsImpulse;

		pub explicit ActionApplyImpulse(
			int localEntityId, int syncedEntityId,
			int priority,
			Vector3& dir, float physicsImpulse)
			: super(
				TYPE, "ActionApplyImpulse",
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/,
				localEntityId, syncedEntityId,
				priority/*priority*/,
				ActionGroup_GeneralFlight,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_GeneralFlight/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/),
			dir(dir), physicsImpulse(physicsImpulse)
		{
			rawSubclassPointer = this;
		}

		pub ~ActionApplyImpulse() override {};
	};
	priv class ActionDestinationFlight : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionDestinationFlight);
		pub class Phases {
			pub static const int INITIAL_ROTATION;
			pub static const int LOWERING;
			pub static const int RISING;
			pub static const int FINISHED;
		};
		pub static int TYPE;

		pub std::shared_ptr<base::WrappedWorldEntity> targetEntity;
		// This is used in case the target entity is disposed. This will make it so the flight flies to the last known targetEntity pos.
		pub Vector3 cachedTargetEntityPos{};
		// targetEntity != nullptr: global coords offset from the targetEntity pos.
		// targetEntity == nullptr: global pos.
		pub Vector3 targetPosOrGlobalOffset;

		// This causes flying to continue even after target removed. It uses the last known position for that.
		pub bool flyToLastKnownTargetPosOnTargetLoss;
		
		pub bool stopFlyingAtDestination;

		pub explicit ActionDestinationFlight(
			int localEntityId, int syncedEntityId,
			int priority,
			std::shared_ptr<base::WrappedWorldEntity>& targetEntity, Vector3& targetPosOrGlobalOffset,
			Vector3& initialCachedTargetEntityPos,
			bool flyToLastKnownTargetPosOnTargetLoss, bool stopFlyingAtDestination)
			: super(
				TYPE, "ActionDestinationFlight",
				true/*isSyncedToServer*/, -1/*persistenceDurationMS*/,
				localEntityId, syncedEntityId,
				priority/*priority*/,
				ActionGroup_GeneralFlight,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_GeneralFlight/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/),
			targetEntity(targetEntity), cachedTargetEntityPos(initialCachedTargetEntityPos), targetPosOrGlobalOffset(targetPosOrGlobalOffset),
			flyToLastKnownTargetPosOnTargetLoss(flyToLastKnownTargetPosOnTargetLoss), stopFlyingAtDestination(stopFlyingAtDestination)
		{
			rawSubclassPointer = this;
		}

		pub ~ActionDestinationFlight() override {};
	};
;
	pub explicit FM_GeneralFlight(
		IWorldEntity* entity,
		FlyingTemplate* t);
	prot void createBMain() override;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);

	pub void addAction_ActionDirectionFlight(Vector3& velocityDir) final;
	priv void onEntityAction_ActionDirectionFlight(ActionDirectionFlight* action);

	pub void addAction_ActionApplyImpulse(
		Vector3& dir,
		float physicsImpulse) final;
	priv void onEntityAction_ActionApplyImpulse(ActionApplyImpulse* action);
	
	pub void addAction_ActionDestinationFlight(
		IWorldEntity* targetEntity, Vector3& targetPosOrGlobalOffset,
		bool flyToLastKnownTargetPosOnTargetLoss, bool stopFlyingAtDestination) final;
	priv void onEntityAction_ActionDestinationFlight(ActionDestinationFlight* action);

    pub ~FM_GeneralFlight() override;
};
};
