#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include "SEMActiveEffects.h"

namespace worldGame3D {
	class IActionsModule;
};

namespace rpg3D {
class SEMActions : public SEMActiveEffects {priv typedef SEMActiveEffects super;pub dCtor(SEMActions);
	priv class ActionDoStatusEffect : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionDoStatusEffect);
		pub static int TYPE;

		pub std::shared_ptr<CfgStatusEffect> effectConfig;

		// if(>=0);// The child will be removed when the parent is removed.
		pub int dependantTrackingId;
		pub bool isDependantTrackingParent;

		pub explicit ActionDoStatusEffect(
			bool isSyncedToServer,
			int localEntityId, int syncedEntityId,
			int priority,
			std::shared_ptr<CfgStatusEffect> effectConfig,
			int dependantTrackingId, bool isDependantTrackingParent)
			: super(
				TYPE, "ActionDoStatusEffect", 
				isSyncedToServer/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				0,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			effectConfig(effectConfig),
			  dependantTrackingId(dependantTrackingId), isDependantTrackingParent(isDependantTrackingParent)
			{ rawSubclassPointer = this; };

		pub ~ActionDoStatusEffect() override {};
	};

	priv worldGame3D::IActionsModule* actions = nullptr;

	priv int dependantTrackingIdCounter = 0;

	pub explicit SEMActions(
	    IWorldEntity* entity,
	    StatusEffectsTemplate* t);

	pub void addAction_ActionDoStatusEffect(std::shared_ptr<CfgStatusEffect> effectConfig) final;

	prot void createBMain() override;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);

	priv void onEntityAction_ActionDoStatusEffect(ActionDoStatusEffect* action);

	pub ~SEMActions() override;
};
};
