#include "CT_Actions.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/config/ToolConfigPersistentCooldown.h>
#include <worldGame3D/gw/entity/module/actions/IActionsModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/math/Math.h>
#include <base/exceptions/NotImplementedException.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <base/event/listener/IEventListener.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>

using namespace rpg3D;

int CT_Actions::ActionOperate::TYPE = super::NewTypeIndex();
int CT_Actions::ActionStopOperation::TYPE = super::NewTypeIndex();

CT_Actions::CT_Actions(ToolConfigPersistentCooldown* config)
	: super(config)
{
	//void
}

void CT_Actions::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		actions = nullptr;
		movingEntity = nullptr;
	} else {
		actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	}
}

void CT_Actions::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		// Remove the event listeners.
		autoListenersList.clear();
	} else {
		actions->setActionProcessor(
			ActionOperate::TYPE, ActionStopOperation::TYPE,
			std::bind(&CT_Actions::onAction, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList
		);
	}
}

void CT_Actions::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating) {
		// Do clearing on the deactivation because doing it on activation may cause problems with certain activation logics.
		//void
	}
}

void CT_Actions::addAction_ActionOperate_Activate(
	int cursorIndex,
	std::shared_ptr<Vector3> worldPos, std::shared_ptr<Vector3> worldDir, sp<Vector2> worldPosXY,
	bool isEnabledPreTurning)
{
	actions->addAction(std::make_shared<ActionOperate>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		cursorIndex,
		worldPos, worldDir, worldPosXY,
		isEnabledPreTurning, ActionOperate::SourceFilterType::Activate
	));
}

void CT_Actions::addAction_ActionOperate_Update(
	int cursorIndex,
	std::shared_ptr<Vector3> worldPos, std::shared_ptr<Vector3> worldDir, sp<Vector2> worldPosXY,
	bool isEnabledPreTurning)
{
	actions->addAction(std::make_shared<ActionOperate>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		cursorIndex,
		worldPos, worldDir, worldPosXY,
		isEnabledPreTurning, ActionOperate::SourceFilterType::Update
	));
}

void CT_Actions::addAction_ActionStopOperation(int cursorIndex) {
	actions->addAction(std::make_shared<ActionStopOperation>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		cursorIndex
		));
}

void CT_Actions::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if (action.getType() == ActionOperate::TYPE) {
		onEntityAction_ActionOperate((ActionOperate*)action.getRawSubclassPointer(ActionOperate::TYPE));
	} else if (action.getType() == ActionStopOperation::TYPE) {
		onEntityAction_ActionStopOperation((ActionStopOperation*)action.getRawSubclassPointer(ActionStopOperation::TYPE));
	} else {
		throw LogicException(LOC);
	}
}

void CT_Actions::onEntityAction_ActionOperate(ActionOperate* action) {
	if (action->getIsReplaced() || action->getIsFinished() || !action->getIsPreRequirementsMet()) {
		// This is the last time this action is called.
		// Do action finish,cleanup logic (eg. remove active status effects from the list).

		//void
	}
	if (action->getIsReplaced() || action->getIsOverridden() || !action->getIsPreRequirementsMet()) {
		// Action replaced. This is the last time this action is called.
		// Or action is overridden. Should not do any active logic now excet for the above one in certain cases (eg. to remove active status effects from the list).

		return;
	}

	if (action->sourceFilterType == ActionOperate::SourceFilterType::Update) {
		if (!getIsOperatingAny()) {
			// This is a update_action but the operation_is_not_active.

			action->persistentActionFinished();

			return;
		}
	}

	if (getIsOperatingAny() && !getIsOperating(action->cursorIndex)) {
		// Already operating by a different cursor, cancel the action->
		action->persistentActionFinished();

		return;
	}

	// Note. Use == comparisson so that the counter can be reset.
	if (toolsHandler->getIsTurnBasedToolsModeActive() && getLastTrigger_turnBasedActionStateCounter() == toolsHandler->getTurnBasedActionStateCounter()) {
		// Turn based mode used and already did the triggering this turn. This is used to detect low cooldown on bow tools (multiple triggers before the first projectile finishes).
		action->persistentActionFinished();

		return;
	}

	// Only do this on the first action tick because the update will do it also if cursor/camera/... changes.
	if (action->isEnabledPreTurning && !action->isTurnPreInjected) {
		action->isTurnPreInjected = true;

		float moveYawDeg;
		if (action->worldPos != nullptr) {
			moveYawDeg = Math::atan2Deg(action->worldPos->y - entity->getY(), action->worldPos->x - entity->getX());
		} else if (action->worldDir != nullptr) {
			moveYawDeg = Math::atan2Deg(action->worldDir->y, action->worldDir->x);
		} else if (action->worldPosXY != nullptr) {
			moveYawDeg = Math::atan2Deg(action->worldPosXY->y - entity->getY(), action->worldPosXY->x - entity->getX());
		} else {
			throw LogicException(LOC);
		}

		std::shared_ptr<worldGame3D::EntityAction> depAction = movingEntity->buildAction_ActionToTargetTurning(
			800/*persistenceDurationMS*/,
			worldGame3D::EntityAction::Priorities::Normal/*priority*/,
			ActionGroup_ToolTurning/*extraGroupsBitMap*/,
			0/*extraPreRequirementsGroupsBitMask*/,
			ActionGroup_ToolTurning/*extraReplaceGroupsBitMask*/,
			rpg3D::ITool::ActionGroup_MovementTurningIdle/*extraOverridesGroupsBitMask*/,
			moveYawDeg, nullptr/*targetEntity*/,
			false/*isStopAtDestination*/,
			true/*isInstantTurn*/
		);
		action->childDependencyActionsToPreInjectList.append_emplace(depAction, true/*addAsDependency*/, false/*addAsDependant*/, false/*triggerDependencyFailOnFinishSuccess*/, true/*triggerDependencyFailOnFail*/);

		return;
	}

	if (config->channelingDurationS != 0) {
		//Not implemented yet. Implement when needed. This variable currently exists for the below ussage start.
		//Need to implement a timer that stops the update after channeling finishes.
		//When implementing this, on channeling finish and in the update phase, handle config->supportsContinuousReactivation and getLogicBlockOverrideDisableContinuousReactivation to disable the update phase if reactivation is disabled.
		throw NotImplementedException();
	}

	if (action->sourceFilterType == ActionOperate::SourceFilterType::Activate && !action->isOperationStarted && !getIsOperatingAny()) {
		// The tool can be activated even if continuous activation is disabled by the tools handler (when turn based mode is active).
		action->isOperationStarted = true;

		setIsOperating(true, action->cursorIndex);
	}

	if (!getIsOperatingAny() || !getIsOperating(action->cursorIndex)) {
		// Operation not active or this is not the operation handler.
		action->persistentActionFinished();

		return;
	}

	operating_worldPos = action->worldPos;
	operating_worldDir = action->worldDir;
	operating_worldPosXY = action->worldPosXY;

	if (canStartTriggering()) {
		startNewTriggering();
	}
}

void CT_Actions::onEntityAction_ActionStopOperation(ActionStopOperation* action) {
	if (action->getIsReplaced() || action->getIsFinished() || !action->getIsPreRequirementsMet()) {
		// This is the last time this action is called.
		// Do action finish,cleanup logic (eg. remove active status effects from the list).

		//void
	}
	if (action->getIsReplaced() || action->getIsOverridden() || !action->getIsPreRequirementsMet()) {
		// Action replaced. This is the last time this action is called.
		// Or action is overridden. Should not do any active logic now excet for the above one in certain cases (eg. to remove active status effects from the list).

		return;
	}

	if (getIsOperating(action->cursorIndex)) {
		setIsOperating(false, action->cursorIndex);
	}
}

CT_Actions::~CT_Actions() {
	//void
}
