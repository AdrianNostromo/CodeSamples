#pragma once

#include <base/gh.h>
#include "CCCTM_Base.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>

namespace towerDefense_a_v1 {
class CCCTM_Reload : public CCCTM_Base {priv typedef CCCTM_Base super;pub dCtor(CCCTM_Reload);
	pub static int ActionGroup_Reload;

	priv class ActionReload : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionReload);
		pub class Phases {
			pub static const int INITIAL_ROTATION;
			pub static const int LOWERING;
			pub static const int RISING;
			pub static const int FINISHED;
		};
		pub static int TYPE;

		pub int phase = Phases::INITIAL_ROTATION;
		pub bool isPhaseInit = false;
		pub float phaseCompletionPercent = 0.0f;

		pub explicit ActionReload(
			int localEntityId, int syncedEntityId,
			int priority)
			: super(
				TYPE, "ActionReload",
				true/*isSyncedToServer*/, -1/*persistenceDurationMS*/,
				localEntityId, syncedEntityId,
				priority/*priority*/,
				ActionGroup_Reload,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_Reload/*replaceGroupsBitMask*/,
				ActionGroup_AimToTarget/*overridesGroupsBitMask*/)
		{
			rawSubclassPointer = this;
		}

		pub ~ActionReload() override {};
	};

	priv bool isProjectileLoaded = true;
	priv bool isProjectilePreviewVisible = true;

	pub CCCTM_Reload(
		IWorldEntity* entity,
		CustomCubesCannonTurretTemplate* t,
		base::audio::ISound* activationSound);
	prot void createBMain() override;

	pub bool getIsProjectileLoaded() final;
	prot void setIsProjectileLoaded(bool isProjectileLoaded);
	prot void setIsProjectilePreviewVisible(bool isProjectilePreviewVisible);

	pub void addAction_ActionReload() final;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);
	priv void onEntityAction_ActionReload(ActionReload* action);

	pub ~CCCTM_Reload() override;
};
};
