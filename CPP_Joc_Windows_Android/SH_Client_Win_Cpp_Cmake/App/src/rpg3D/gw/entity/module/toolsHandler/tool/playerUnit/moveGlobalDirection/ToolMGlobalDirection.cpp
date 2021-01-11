#include "ToolMGlobalDirection.h"
#include <base/math/util/BoolUtil.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <base/math/Math.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolMGlobalDirection::ToolMGlobalDirection()
	: super()
{
	activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::GlobalMoveDirection2D);
}

void ToolMGlobalDirection::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		userControlledModule = nullptr;
		movingEntity = nullptr;
	} else {
		userControlledModule = entity->getComponentAs<rpg3D::IUserControlledModule*>(true/*mustExist*/);
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	}
}

void ToolMGlobalDirection::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		Vector2 moveDirVector{ 0.0f, 0.0f };
		movingEntity->addAction_ActionSetDirectionalMovementContinuous(
			IMovingEntityModule::ControlType::Global_Dir,
			moveDirVector,
			false/*isAutoTurn*/, true/*isInstantTurn*/
		);
	} else {
		//void
	}
}

bool ToolMGlobalDirection::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	if (!getIsToolSelected()) {
		return false;
	}

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::alwaysUpdate) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::GlobalMoveDirection2D) {
			bool isInstantTurn = false;
			int actionsGroups = rpg3D::ITool::ActionGroup_MovementTurningIdle;
			int replacesGroupsBitMask = rpg3D::ITool::ActionGroup_MovementTurningIdle | rpg3D::ITool::ActionGroup_MovementTurningAlways;
			int persistenceDurationMS = -1;

			movingEntity->addAction_ActionSetDirectionalMovementContinuous(
				IMovingEntityModule::ControlType::Global_Dir,
				interaction->moveDirVector,
				false/*isAutoTurn*/, true/*isInstantTurn*/
			);

			if (interaction->moveDirVector.x != 0.0f || interaction->moveDirVector.y != 0.0f) {
				float moveYawDeg = Math::atan2Deg(interaction->moveDirVector.y, interaction->moveDirVector.x);
				// Make this persistent so it persists on dPad movements if the turn was overridden.
				movingEntity->addAction_ActionToTargetTurning(
					persistenceDurationMS/*persistenceDurationMS*/,
					worldGame3D::EntityAction::Priorities::Normal/*priority*/,
					actionsGroups/*extraGroupsBitMap*/,
					0/*extraPreRequirementsGroupsBitMask*/,
					replacesGroupsBitMask/*extraReplaceGroupsBitMask*/,
					0/*extraOverridesGroupsBitMask*/,
					moveYawDeg, nullptr/*targetEntity*/,
					false/*isStopAtDestination*/,
					isInstantTurn/*isInstantTurn*/
					);
			} else {
				// The above turn action never stops, dispatch this one to make it stop.
				movingEntity->addAction_ActionCustomStop(
					worldGame3D::EntityAction::Priorities::Normal/*priority*/,
					replacesGroupsBitMask/*extraReplaceGroupsBitMask*/
				);
			}
		} else {
			localIsProcessed = false;
		}
	} else {
		localIsProcessed = false;
	}

	if (isProcessed && localIsProcessed) {
		// Filter used in multiple layers. That should never occur. Filters code is not made for that.
		throw LogicException(LOC);
	}

	return isProcessed || localIsProcessed;
}

ToolMGlobalDirection::~ToolMGlobalDirection() {
	//void
}
