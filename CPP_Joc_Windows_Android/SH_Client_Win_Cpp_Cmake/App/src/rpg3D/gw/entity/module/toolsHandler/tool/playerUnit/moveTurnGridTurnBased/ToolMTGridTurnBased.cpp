#include "ToolMTGridTurnBased.h"
#include <base/math/util/BoolUtil.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <base/math/Math.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/grid2DContent/IGPGrid2DContent.h>
#include <rpg3D/gw/entity/module/grid2DContent/IGrid2DContentModule.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigDirect.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <rpg3D/gw/entity/module/userControlled/filters/custom/WIF_Touch_FeetPlane.h>
#include <rpg3D/gw/entity/module/toolsHandler/IToolsHandlerModule.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

std::string ToolMTGridTurnBased::MOVEMENT_INITIATOR_IDENTIFIER = "LBMTGridTurnBased";

ToolMTGridTurnBased::ToolMTGridTurnBased()
	: super()
{
	// This works if the camera is following the player unit directly, that is the reason why GlobalFocusDirection2DRelativeToUnit is not used.
	// This is also used to allow for the idle_auto_turn logic to work, previously was using GlobalFocusDirection2DRelativeToUnit but that dispatches also on unit movements and that interferes with the idle_auto_turn logic.
	customFiltersList_innactiveForActivation.addFilter(std::make_shared<rpg3D::WIF_Touch_FeetPlane>(
		base::Touch::CursorMode::Normal->bitGroup | base::Touch::CursorMode::Locked->bitGroup, base::Touch::ButtonCode::LeftOrTouch->bitGroup/*buttonId*/, TouchEventType::CURSOR_DOWN->bitGroup/*touchType*/,
		IWorldInteractionFilter::InputPriorities::Normal_Plus1/*inputPriority*/,
		std::bind(&ToolMTGridTurnBased::onFilterInteraction_FeetPlane_Down_BtnMain, this, std::placeholders::_1, std::placeholders::_2)
	));
}

void ToolMTGridTurnBased::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		userControlledModule = nullptr;
		movingEntity = nullptr;
		grid2DContent = nullptr;
	} else {
		userControlledModule = entity->getComponentAs<rpg3D::IUserControlledModule*>(true/*mustExist*/);
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
		grid2DContent = entity->getComponentAs<IGrid2DContentModule*>(true/*mustExist*/);
	}
}

void ToolMTGridTurnBased::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		//void
	} else {
		if (gpGrid2DContent == nullptr && gwGamePartsOptional != nullptr) {
			gpGrid2DContent = gwGamePartsOptional->getComponentAs<rpg3D::IGPGrid2DContent*>(false/*mustExist*/);
		}
	}
}

bool ToolMTGridTurnBased::onFilterInteraction_FeetPlane_Down_BtnMain(Vector3& localFeetPlanePos, int cursorIndex) {
	Vector2 targetPos{ localFeetPlanePos.x, localFeetPlanePos.y };

	Vector2Int moveDestGPos{};
	gpGrid2DContent->worldPosToGPos(targetPos, moveDestGPos);
		
	Vector2Int& currentGPos = grid2DContent->getGPosRef();

	if (toolsHandler->getIsTurnBasedToolsModeActive() && lastMove_turnBasedActionStateCounter == toolsHandler->getTurnBasedActionStateCounter()) {
		// Already did an action this turn_state.

		return true;
	}

	if (moveDestGPos.equals(currentGPos)) {
		// Move skipped.
		if (toolsHandler->getIsTurnBasedToolsModeActive()) {
			lastMove_turnBasedActionStateCounter = toolsHandler->getTurnBasedActionStateCounter();
		}

		entity->dispatchEvent(*EVENT_gridMovement_skipped);

		return true;
	}else if (((currentGPos.x == moveDestGPos.x && Math::abs(moveDestGPos.y - currentGPos.y) == 1)
		|| (currentGPos.y == moveDestGPos.y && Math::abs(moveDestGPos.x - currentGPos.x) == 1))
		&& gpGrid2DContent->isGPosInTrackingRegion(moveDestGPos.x, moveDestGPos.y) && !gpGrid2DContent->getIsGCellOccupied(moveDestGPos.x, moveDestGPos.y))
	{
		gpGrid2DContent->gPosCenterToWorldPos(moveDestGPos, targetPos);
	} else {
		// Out of range or gCell occupied or outside of the grid play area.
		return false;
	}

	// Add a moveToDestination command to the destination location.
	entity->addEventListener(
		IMovingEntityModule::EVENT_move_destinationReached->type,
		std::bind(&ToolMTGridTurnBased::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		movementAutoListenersList
	);

	movingEntity->addAction_ActionToTargetMovementContinuous(
		IMovingEntityModule::ControlType::LocalEntityDirection_BackForwardOnly,
		targetPos, nullptr/*targetEntity*/,
		0.0f/*maxRange*/, true/*isStopAtDestination*/,
		true/*isAutoTurn*/, true/*isInstantTurn*/,
		MOVEMENT_INITIATOR_IDENTIFIER/*initiatorIdentifier*/,
		rpg3D::MovementConfigDirect::TYPE/*movementTypeOverride*/
	);

	if (toolsHandler->getIsTurnBasedToolsModeActive()) {
		lastMove_turnBasedActionStateCounter = toolsHandler->getTurnBasedActionStateCounter();
	}
	entity->dispatchEvent(*EVENT_gridMovement_started);

	return true;
}

void ToolMTGridTurnBased::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == MovingEntityModule::EVENT_move_destinationReached->type) {
		if (event.initiatorIdentifier != MOVEMENT_INITIATOR_IDENTIFIER) {
			// This is not the movement started from here.
			throw LogicException(LOC);
		}

		movementAutoListenersList.clear();

		entity->dispatchEvent(*EVENT_gridMovement_finished);
	}

	else {
		throw LogicException(LOC);
	}
}

void ToolMTGridTurnBased::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	if (gpGrid2DContent == nullptr) {
		gpGrid2DContent = gwGameParts->getComponentAs<rpg3D::IGPGrid2DContent*>(false/*mustExist*/);
	}
}

void ToolMTGridTurnBased::disconnectFromGamePartsPre() {
	gpGrid2DContent = nullptr;
}

ToolMTGridTurnBased::~ToolMTGridTurnBased() {
	//void
}
