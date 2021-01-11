#include "ToolTGlobalFocusDirectionYaw.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
#include <rpg3D/gw/entity/module/controller/logicBlock/ILogicBlock.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolTGlobalFocusDirectionYaw::ToolTGlobalFocusDirectionYaw()
	: super()
{
	// This works if the camera is following the player unit directly, that is the reason why GlobalFocusDirection2DRelativeToUnit is not used.
	// This is also used to allow for the idle_auto_turn logic to work, previously was using GlobalFocusDirection2DRelativeToUnit but that dispatches also on unit movements and that interferes with the idle_auto_turn logic.
	activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::GlobalFocusDirection2D);
}

void ToolTGlobalFocusDirectionYaw::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		userControlledModule = nullptr;
		movingEntity = nullptr;
	} else {
		userControlledModule = entity->getComponentAs<rpg3D::IUserControlledModule*>(true/*mustExist*/);
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	}
}

bool ToolTGlobalFocusDirectionYaw::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::alwaysUpdate) {
		if (interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::GlobalFocusDirection2D) {
			if (interaction->isSource_cursor) {
				// This uses persistenceDurationMS as a iddle mouse check to override the idle_movement_turning on mouse_input for 4s.
				// The persistenceDurationMS is used to enable auto turn to move direction when the mouse is iddle for ~4seconds.
				movingEntity->addAction_ActionToTargetTurning(
					4000/*persistenceDurationMS*/,
					worldGame3D::EntityAction::Priorities::Normal/*priority*/,
					rpg3D::ITool::ActionGroup_MouseTurningDirect/*extraGroupsBitMap*/,
					0/*extraPreRequirementsGroupsBitMask*/,
					rpg3D::ITool::ActionGroup_MouseTurningDirect/*extraReplaceGroupsBitMask*/,
					rpg3D::ITool::ActionGroup_MovementTurningIdle | rpg3D::ITool::ActionGroup_ToolTurning/*extraOverridesGroupsBitMask*/,
					interaction->globalRotZDeg, nullptr/*targetEntity*/,
					false/*isStopAtDestination*/,
					true/*isInstantTurn*/
				);
			} else {
				// This is used if the camera position is separate from the player (eg. fixed room camera) so the player always turns on movement even if no mouse input.
				// This relies on the above ActionGroup_MouseTurningDirect to keep ActionGroup_MovementTurningIdle overridden because this is a instant action.
				movingEntity->addAction_ActionToTargetTurning(
					0/*persistenceDurationMS*/,
					worldGame3D::EntityAction::Priorities::Normal/*priority*/,
					rpg3D::ITool::ActionGroup_MouseTurningIndirect/*extraGroupsBitMap*/,
					rpg3D::ITool::ActionGroup_MouseTurningDirect/*extraPreRequirementsGroupsBitMask*/,
					rpg3D::ITool::ActionGroup_MouseTurningIndirect/*extraReplaceGroupsBitMask*/,
					rpg3D::ITool::ActionGroup_MouseTurningDirect/*extraOverridesGroupsBitMask*/,
					interaction->globalRotZDeg, nullptr/*targetEntity*/,
					true/*isStopAtDestination*/,
					true/*isInstantTurn*/
				);
			}
		} else {
			localIsProcessed = false;
		}
	}else {
		localIsProcessed = false;
	}

	if (isProcessed && localIsProcessed) {
		// Filter used in multiple layers. That should never occur. Filters code is not made for that.
		throw LogicException(LOC);
	}

	return isProcessed || localIsProcessed;
}

ToolTGlobalFocusDirectionYaw::~ToolTGlobalFocusDirectionYaw() {
	//void
}
