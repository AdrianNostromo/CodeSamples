#pragma once

#include <base/gh.h>
#include "MEMM_DirectionalContinuous.h"

namespace base {
	class WrappedWorldEntity;
};
namespace rpg3D {
	class IPhysics3DModule;
}

namespace rpg3D {
class MEMM_ToTarget : public MEMM_DirectionalContinuous {priv typedef MEMM_DirectionalContinuous super; pub dCtor(MEMM_ToTarget);
	priv class ActionToTargetMovementContinuous : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionToTargetMovementContinuous);
		pub static int TYPE;

		pub int controlType;

		// This is a offset from the targetEntity if it exists in global coords, not local coords.
		pub Vector2 targetPosOrOffset;
		// Optional.
		pub std::shared_ptr<base::WrappedWorldEntity> targetEntity;

		pub float maxRange;
		pub bool isStopAtDestination;

		pub bool isAutoTurn;
		pub bool isInstantTurn;

		pub std::string initiatorIdentifier;

		// >= 0; Overrides the default move type.// This is used for move to position.
		pub int movementTypeOverride;

		//asdA36;// stopped by teleports also.
		pub explicit ActionToTargetMovementContinuous(
			int localEntityId, int syncedEntityId,
			int priority,
			int controlType,
			Vector2& targetPosOrOffset, std::shared_ptr<base::WrappedWorldEntity>& targetEntity,
			float maxRange, bool isStopAtDestination,
			bool isAutoTurn, bool isInstantTurn,
			std::string& initiatorIdentifier,
			int movementTypeOverride)
			: super(
				TYPE, "ActionToTargetMovementContinuous", 
				true/*isSyncedToServer*/, -1/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/,
				ActionGroup_Move | ((isAutoTurn) ? ActionGroup_Turn : 0),/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_Move | ((isAutoTurn) ? ActionGroup_Turn : 0)/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			controlType(controlType),
			targetPosOrOffset(targetPosOrOffset), targetEntity(targetEntity),
			maxRange(maxRange), isStopAtDestination(isStopAtDestination),
			isAutoTurn(isAutoTurn), isInstantTurn(isInstantTurn),
			initiatorIdentifier(initiatorIdentifier),
			movementTypeOverride(movementTypeOverride)
		{
			rawSubclassPointer = this;
		};

		pub ~ActionToTargetMovementContinuous() override {};
	};

	priv static const int MOVE_REQUEST_TYPE_ToTarget;

	priv worldGame3D::IActionsModule* actions = nullptr;
	priv ILookAtModule* lookAt = nullptr;
	priv rpg3D::IPhysics3DModule* physics3DModule = nullptr;

    pub explicit MEMM_ToTarget(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);

	pub std::shared_ptr<worldGame3D::EntityAction> buildAction_ActionToTargetMovementContinuous(
		int controlType,
		Vector2& targetPosOrOffset, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
		float maxRange, bool isStopAtDestination,
		bool isAutoTurn, bool isInstantTurn,
		std::string& initiatorIdentifier = StringUtil::empty,
		int movementTypeOverride = -1) override;
	pub void addAction_ActionToTargetMovementContinuous(
		int controlType,
		Vector2& targetPosOrOffset, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
		float maxRange, bool isStopAtDestination,
		bool isAutoTurn, bool isInstantTurn,
		std::string& initiatorIdentifier = StringUtil::empty,
		int movementTypeOverride = -1) override;
	prot void createBMain() override;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);
	priv void onEntityAction_ActionToTargetMovementContinuous(ActionToTargetMovementContinuous* action);

    pub ~MEMM_ToTarget() override;
};
};
