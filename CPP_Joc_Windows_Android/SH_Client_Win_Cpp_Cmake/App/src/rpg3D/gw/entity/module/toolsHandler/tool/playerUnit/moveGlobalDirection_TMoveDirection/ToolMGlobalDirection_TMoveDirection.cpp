#include "ToolMGlobalDirection_TMoveDirection.h"
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

ToolMGlobalDirection_TMoveDirection::ToolMGlobalDirection_TMoveDirection()
	: super()
{
	// This works if the camera is following the player unit directly, that is the reason why GlobalFocusDirection2DRelativeToUnit is not used.
	// This is also used to allow for the idle_auto_turn logic to work, previously was using GlobalFocusDirection2DRelativeToUnit but that dispatches also on unit movements and that interferes with the idle_auto_turn logic.
	activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::GlobalMoveDirection2D);
}

void ToolMGlobalDirection_TMoveDirection::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		userControlledModule = nullptr;
		movingEntity = nullptr;
	} else {
		userControlledModule = entity->getComponentAs<rpg3D::IUserControlledModule*>(true/*mustExist*/);
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	}
}

bool ToolMGlobalDirection_TMoveDirection::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::alwaysUpdate) {
		if(interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::GlobalMoveDirection2D) {
			bool isInstantTurn = true;
			int actionsGroups = rpg3D::ITool::ActionGroup_MovementTurningAlways;
			int replacesGroupsBitMask = IMovingEntityModule::ActionGroup_Turn | rpg3D::ITool::ActionGroup_MovementTurningIdle | rpg3D::ITool::ActionGroup_MovementTurningAlways;
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
		}else {
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

ToolMGlobalDirection_TMoveDirection::~ToolMGlobalDirection_TMoveDirection() {
	//void
}
