#pragma once

#include <base/gh.h>
#include "MEMT_ContinuousTurn.h"

namespace rpg3D {
class MEMT_TurnToTargetAngle : public MEMT_ContinuousTurn {priv typedef MEMT_ContinuousTurn super;pub dCtor(MEMT_TurnToTargetAngle);
    priv class ActionToTargetTurning : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionToTargetTurning);
		pub static int TYPE;

		// This is a offset from the targetEntity if it exists.
		pub float targetRotZDeg;
		// Optional.
		pub std::shared_ptr<base::WrappedWorldEntity> targetEntity;

		pub bool isStopAtDestination;

		pub bool isInstantTurn;

		pub explicit ActionToTargetTurning(
			int localEntityId, int syncedEntityId,
			int persistenceDurationMS,
			int priority,
			int extraGroupsBitMap,
			int extraPreRequirementsGroupsBitMask,
			int extraReplaceGroupsBitMask,
			int extraOverridesGroupsBitMask,
			float targetRotZDeg, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
			bool isStopAtDestination,
			bool isInstantTurn)
			: super(
				TYPE, "ActionToTargetTurning", 
				true/*isSyncedToServer*/, persistenceDurationMS,
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				ActionGroup_Turn | extraGroupsBitMap,/*groupsBitMap*/
				0 | extraPreRequirementsGroupsBitMask/*preRequirementsGroupsBitMask*/,
				0 | extraReplaceGroupsBitMask/*replaceGroupsBitMask*/,
				0 | extraOverridesGroupsBitMask/*overridesGroupsBitMask*/
			),
			targetRotZDeg(targetRotZDeg), targetEntity(targetEntity),
			isStopAtDestination(isStopAtDestination),
			isInstantTurn(isInstantTurn)
		{
			rawSubclassPointer = this;
		};

		pub ~ActionToTargetTurning() override {};
	};

	priv static const int TURN_REQUEST_TYPE_ToTarget;

	priv worldGame3D::IActionsModule* actions = nullptr;
	priv ILookAtModule* lookAt = nullptr;

    pub explicit MEMT_TurnToTargetAngle(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);
	prot void createBMain() override;

	pub std::shared_ptr<worldGame3D::EntityAction> buildAction_ActionToTargetTurning(
		int persistenceDurationMS,
		int priority,
		int extraGroupsBitMap,
		int extraPreRequirementsGroupsBitMask,
		int extraReplaceGroupsBitMask,
		int extraOverridesGroupsBitMask,
		float targetRotZDeg, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
		bool isStopAtDestination,
		bool isInstantTurn) override;
	pub void addAction_ActionToTargetTurning(
		int persistenceDurationMS,
		int priority,
		int extraGroupsBitMap,
		int extraPreRequirementsGroupsBitMask,
		int extraReplaceGroupsBitMask,
		int extraOverridesGroupsBitMask,
		float targetRotZDeg, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
		bool isStopAtDestination,
		bool isInstantTurn) override;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);
	priv void onEntityAction_ActionToTargetTurning(ActionToTargetTurning* action);

    pub ~MEMT_TurnToTargetAngle() override;
};
};
