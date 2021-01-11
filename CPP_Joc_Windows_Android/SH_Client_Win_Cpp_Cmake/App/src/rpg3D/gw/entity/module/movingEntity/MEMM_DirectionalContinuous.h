#pragma once

#include <base/gh.h>
#include "MEMM_BasicMovements.h"
#include <base/math/Vector2.h>

namespace worldGame3D {
	class IActionsModule;
};
namespace rpg3D {
	class IPhysics3DModule;
}
class ILookAtModule;

namespace rpg3D {
class MEMM_DirectionalContinuous : public MEMM_BasicMovements {priv typedef MEMM_BasicMovements super; pub dCtor(MEMM_DirectionalContinuous);
	priv class ActionSetDirectionalMovementContinuous : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionSetDirectionalMovementContinuous);
		pub static int TYPE;

		pub int controlType;

		pub Vector2 dir;

		pub bool isAutoTurn;
		pub bool isInstantTurn;

		pub explicit ActionSetDirectionalMovementContinuous(
			int localEntityId, int syncedEntityId,
			int priority,
			int controlType,
			Vector2& dir,
			bool isAutoTurn, bool isInstantTurn)
			: super(
				TYPE, "MEMM_DirectionalContinuous::ActionSetDirectionalMovementContinuous", 
				true/*isSyncedToServer*/, -1/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/,
				ActionGroup_Move | ((isAutoTurn) ? ActionGroup_Turn : 0),/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_Move | ((isAutoTurn) ? ActionGroup_Turn : 0)/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			controlType(controlType),
			dir(dir),
			isAutoTurn(isAutoTurn), isInstantTurn(isInstantTurn)
		{
			rawSubclassPointer = this;
		};

		pub ~ActionSetDirectionalMovementContinuous() override {};
	};

	priv static const int MOVE_REQUEST_TYPE_Directional;

	priv worldGame3D::IActionsModule* actions = nullptr;
	priv ILookAtModule* lookAt = nullptr;
	priv rpg3D::IPhysics3DModule* physics3DModule = nullptr;

    pub explicit MEMM_DirectionalContinuous(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);
    prot void createBMain() override;

    pub void addAction_ActionSetDirectionalMovementContinuous(
    	int controlType,
    	Vector2& dir,
    	bool isAutoTurn, bool isInstantTurn) override;

    pub void addAction_ActionDirectionalMovementBackFrontContinuous(
    	int controlType,
		float backFrontMoveVector) override;

    priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);
	priv void onEntityAction_ActionSetDirectionalMovementContinuous(ActionSetDirectionalMovementContinuous* action);

	priv static void ConvertStrafeMovementToGlobalMovement(Vector2& moveDir, Quaternion* entityRot, Vector2* oGlobalMoveDir);

    pub ~MEMM_DirectionalContinuous() override;
};
};
