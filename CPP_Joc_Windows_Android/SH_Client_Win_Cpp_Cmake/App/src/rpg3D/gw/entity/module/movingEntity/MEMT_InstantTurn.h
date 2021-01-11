#pragma once

#include <base/gh.h>
#include "MEMT_Base.h"
#include <string>
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>
#include <worldGame3D/gw/event/GWEvent.h>

namespace worldGame3D {
    class IActionsModule;
};
class ILookAtModule;

namespace rpg3D {
class MEMT_InstantTurn : public MEMT_Base {priv typedef MEMT_Base super;pub dCtor(MEMT_InstantTurn);
    priv class ActionDirectRotateZElevationDelta : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionDirectRotateZElevationDelta);
		pub static int TYPE;

		pub float rotZDegDelta;
		pub float rotElevationDelta;

		pub explicit ActionDirectRotateZElevationDelta(
			int localEntityId, int syncedEntityId,
			int priority,
			float rotZDegDelta, float rotElevationDelta)
			: super(
				TYPE, "ActionDirectRotateZElevationDelta", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				ActionGroup_Turn,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				// Replace other ActionGroup_Turn because when switching to first person, there is a persistent (4 seconds) turn from 3rd person still active that interferes with this turn code.
				//asd_01;// this replace introduces a bug because it instantly replaces the sensor front, back tilt actions.
				ActionGroup_Turn,/*replaceGroupsBitMask*/
				0/*overridesGroupsBitMask*/
			), 
			rotZDegDelta(rotZDegDelta), rotElevationDelta(rotElevationDelta)
		{ rawSubclassPointer = this; };

		pub ~ActionDirectRotateZElevationDelta() override {};
	};
	priv class ActionDirectRotateZ : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionDirectRotateZ);
		pub static int TYPE;

		pub float rotZDeg;

		pub explicit ActionDirectRotateZ(
			int localEntityId, int syncedEntityId,
			int priority, 
			float rotZDeg)
			: super(
				TYPE, "ActionDirectRotateZ", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				ActionGroup_Turn,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			), 
			rotZDeg(rotZDeg)
		{ rawSubclassPointer = this; };

		pub ~ActionDirectRotateZ() override {};
	};
	priv class ActionDirectRotateZElevation : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionDirectRotateZElevation);
		pub static int TYPE;

		pub float rotZDeg;
		pub float rotElevationDeg;

		pub explicit ActionDirectRotateZElevation(
			int localEntityId, int syncedEntityId,
			int priority,
			float rotZDeg, float rotElevationDeg)
			: super(
				TYPE, "ActionDirectRotateZElevation", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				ActionGroup_Turn,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			), 
			rotZDeg(rotZDeg), rotElevationDeg(rotElevationDeg)
		{ rawSubclassPointer = this; };

		pub ~ActionDirectRotateZElevation() override {};
	};
	priv class ActionDirectRotateElevation : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super;pub dCtor(ActionDirectRotateElevation);
		pub static int TYPE;

		pub float rotElevationDeg;

		pub explicit ActionDirectRotateElevation(
			int localEntityId, int syncedEntityId,
			int priority,
			float rotElevationDeg)
			: super(
				TYPE, "ActionDirectRotateElevation", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				ActionGroup_Turn,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				0/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			), 
			rotElevationDeg(rotElevationDeg)
		{ rawSubclassPointer = this; };

		pub ~ActionDirectRotateElevation() override {};
	};

	priv worldGame3D::IActionsModule* actions = nullptr;
	priv ILookAtModule* lookAt = nullptr;

    pub explicit MEMT_InstantTurn(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);
    prot void createBMain() override;

    pub void addAction_ActionDirectRotateZElevationDelta(float rotZDegDelta, float rotElevationDelta) override;
	pub void addAction_ActionDirectRotateZ(int priority, float rotZDeg) override;
	pub void addAction_ActionDirectRotateZElevation(float rotZDeg, float rotElevationDeg) override;
	pub void addAction_ActionDirectRotateElevation(float rotElevationDeg) override;

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);

	priv void onEntityAction_ActionDirectRotateZElevationDelta(ActionDirectRotateZElevationDelta* action);
	priv void onEntityAction_ActionDirectRotateZ(ActionDirectRotateZ* action);
	priv void onEntityAction_ActionDirectRotateZElevation(ActionDirectRotateZElevation* action);
	priv void onEntityAction_ActionDirectRotateElevation(ActionDirectRotateElevation* action);

	prot void disposePre() override;
    pub ~MEMT_InstantTurn() override;
};
};
