#include "ToolTMouseOrSensorPitchYaw.h"
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

ToolTMouseOrSensorPitchYaw::ToolTMouseOrSensorPitchYaw()
	: super()
{
	// This works if the camera is following the player unit directly, that is the reason why GlobalFocusDirection2DRelativeToUnit is not used.
	// This is also used to allow for the idle_auto_turn logic to work, previously was using GlobalFocusDirection2DRelativeToUnit but that dispatches also on unit movements and that interferes with the idle_auto_turn logic.
	activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::ContinuousFocusView3DElevation_Absolute);
	activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::ContinuousFocusView3D_Delta);
}

void ToolTMouseOrSensorPitchYaw::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		userControlledModule = nullptr;
		movingEntity = nullptr;
	} else {
		userControlledModule = entity->getComponentAs<rpg3D::IUserControlledModule*>(true/*mustExist*/);
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	}
}

bool ToolTMouseOrSensorPitchYaw::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::alwaysUpdate) {
		if(interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ContinuousFocusView3DElevation_Absolute) {
			movingEntity->addAction_ActionDirectRotateElevation(
				// Negate the x direction to convert from screen horizontal to z rotation.
				interaction->angElevationDeg
			);
		}else if(interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::ContinuousFocusView3D_Delta) {
			movingEntity->addAction_ActionDirectRotateZElevationDelta(
				// Negate the x and y direction to convert from screen horizontal to z rotation.
				-interaction->sensitisedViewPanDelta.x,
				-interaction->sensitisedViewPanDelta.y
			);
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

ToolTMouseOrSensorPitchYaw::~ToolTMouseOrSensorPitchYaw() {
	//void
}