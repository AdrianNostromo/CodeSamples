#pragma once

#include <base/gh.h>
#include "CT_Triggering.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>

class IMovingEntityModule;
class IEventListener;
namespace base {
	class EntityEvent;
};
namespace worldGame3D {
	class IActionsModule;
};

namespace rpg3D {
class CT_Actions : public CT_Triggering {priv typedef CT_Triggering super;pub dCtor(CT_Actions);
    priv class ActionOperate : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionOperate);
		pub enum SourceFilterType { Activate, Update };

		pub static int TYPE;

		pub int cursorIndex;
		pub std::shared_ptr<Vector3> worldPos;
		pub std::shared_ptr<Vector3> worldDir;
		pub sp<Vector2> worldPosXY;

		pub bool isEnabledPreTurning;

		pub bool isTurnPreInjected = false;

		// This is used to have only 1 action for activate_and_update and to detect when activation occurs.
		pub SourceFilterType sourceFilterType;
		// This is used when sourceFilterType == Active; It makes so only once activation can occur by the activation.
		pub bool isOperationStarted = false;

		pub explicit ActionOperate(
			int localEntityId, int syncedEntityId,
			int priority,
			int cursorIndex,
			std::shared_ptr<Vector3>& worldPos, std::shared_ptr<Vector3>& worldDir, sp<Vector2> worldPosXY,
			bool isEnabledPreTurning, SourceFilterType sourceFilterType)
			: super(
				TYPE, "ActionOperate",
				true/*isSyncedToServer*/, -1/*persistenceDurationMS*/,
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				ActionGroup_ToolOperate,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				// Note. Don't replace self because multiple reactivations will cause all the activate actions to stop even if the first one must remain active.
				ActionGroup_ToolOperate/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			cursorIndex(cursorIndex),
			worldPos(worldPos), worldDir(worldDir), worldPosXY(worldPosXY),
			isEnabledPreTurning(isEnabledPreTurning),
			sourceFilterType(sourceFilterType)
		{ rawSubclassPointer = this; };

		pub ~ActionOperate() override {};
	};
	priv class ActionStopOperation : public worldGame3D::EntityAction {priv typedef worldGame3D::EntityAction super; pub dCtor(ActionStopOperation);
		pub static int TYPE;

		pub int cursorIndex;

		pub explicit ActionStopOperation(
			int localEntityId, int syncedEntityId,
			int priority,
			int cursorIndex)
			: super(
				TYPE, "ActionStopOperation", 
				true/*isSyncedToServer*/, 0/*persistenceDurationMS*/, 
				localEntityId, syncedEntityId,
				priority/*priority*/, 
				ActionGroup_ToolStopOperation,/*groupsBitMap*/
				0/*preRequirementsGroupsBitMask*/,
				ActionGroup_ToolOperate/*replaceGroupsBitMask*/,
				0/*overridesGroupsBitMask*/
			),
			  cursorIndex(cursorIndex)
		{
			rawSubclassPointer = this;
			canHaveNoListeners = true;
		};

		pub ~ActionStopOperation() override {};
	};

	priv worldGame3D::IActionsModule* actions = nullptr;
	priv IMovingEntityModule* movingEntity = nullptr;

	priv ArrayList<std::shared_ptr<IEventListener>> autoListenersList{};
	
	pub explicit CT_Actions(ToolConfigPersistentCooldown* config);

	prot void onEntityChanged() override;

	prot void onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) override;
	prot void onIsOperatingChanged(bool isOperating) override;

	pub void addAction_ActionOperate_Activate(
		int cursorIndex,
		std::shared_ptr<Vector3> worldPos, std::shared_ptr<Vector3> worldDir, sp<Vector2> worldPosXY,
		bool isEnabledPreTurning);
	pub void addAction_ActionOperate_Update(
		int cursorIndex,
		std::shared_ptr<Vector3> worldPos, std::shared_ptr<Vector3> worldDir, sp<Vector2> worldPosXY,
		bool isEnabledPreTurning);
	pub void addAction_ActionStopOperation(int cursorIndex);

	priv void onAction(IEventListener& eventListener, worldGame3D::IAction& action);

	priv void onEntityAction_ActionOperate(ActionOperate* action);
	priv void onEntityAction_ActionStopOperation(ActionStopOperation* action);

    pub ~CT_Actions() override;
};
};
