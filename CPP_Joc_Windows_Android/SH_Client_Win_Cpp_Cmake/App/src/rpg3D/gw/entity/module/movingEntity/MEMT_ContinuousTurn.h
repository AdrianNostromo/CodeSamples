#pragma once

#include <base/gh.h>
#include "MEMT_InstantTurn.h"
#include <base/math/Vector1.h>

namespace rpg3D {
class MEMT_ContinuousTurn : public MEMT_InstantTurn {priv typedef MEMT_InstantTurn super;pub dCtor(MEMT_ContinuousTurn);
	priv class ActionDirectionalTurnZContinuous : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionDirectionalTurnZContinuous);
		pub static int TYPE;

		pub Vector1 dir;

		pub explicit ActionDirectionalTurnZContinuous(
			int localEntityId, int syncedEntityId,
			int priority,
			Vector1& dir)
			: super(
				TYPE, "ActionDirectionalTurnZContinuous", 
				true/*isSyncedToServer*/, -1/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/,
				ActionGroup_Turn | ActionGroup_DirectionalTurnZ,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0 | ActionGroup_DirectionalTurnZ/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			dir(dir)
		{
			rawSubclassPointer = this;
		};

		pub ~ActionDirectionalTurnZContinuous() override {};
	};

	priv static const int TURN_REQUEST_TYPE_Directional;
	// Use a hard limit here of about 180 degrees per second.
	priv static const float MaxTurnSpeedZDegMax;

	priv worldGame3D::IActionsModule* actions = nullptr;
	priv ILookAtModule* lookAt = nullptr;

    pub explicit MEMT_ContinuousTurn(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);
    prot void createBMain() override;

    pub void addAction_ActionDirectionalTurnZContinuous(
    	Vector1& dir) final;

    priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);
	priv void onEntityAction_ActionDirectionalTurnZContinuous(ActionDirectionalTurnZContinuous* action);

    pub ~MEMT_ContinuousTurn() override;
};
};
