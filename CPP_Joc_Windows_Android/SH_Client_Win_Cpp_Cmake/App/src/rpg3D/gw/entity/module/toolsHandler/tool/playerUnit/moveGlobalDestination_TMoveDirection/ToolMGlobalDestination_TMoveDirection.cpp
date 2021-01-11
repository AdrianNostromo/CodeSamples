#include "ToolMGlobalDestination_TMoveDirection.h"
#include <base/math/util/BoolUtil.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <rpg3D/gw/entity/template/userControlled/UserControlledTemplate.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <base/math/Math.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <rpg3D/gw/entity/module/movingEntity/MovingEntityModule.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/entity/module/platform/IPlatformModule.h>
#include <worldGame3D/gw/IGameWorldGameParts.h>
#include <rpg3D/gw/grid2DContent/IGPGrid2DContent.h>

using namespace rpg3D;
using namespace rpg3D::playerUnit;

ToolMGlobalDestination_TMoveDirection::ToolMGlobalDestination_TMoveDirection(int movementTypeOverride, bool usesGridMovement, DestinationType destinationType)
	: super(movementTypeOverride, usesGridMovement, destinationType)
{
	if (destinationType == GroundAndSomeEntities) {
		if (usesGridMovement) {
			// This is not implemented and should not be used.
			throw LogicException(LOC);
		}

		// This works if the camera is following the player unit directly, that is the reason why GlobalFocusDirection2DRelativeToUnit is not used.
		// This is also used to allow for the idle_auto_turn logic to work, previously was using GlobalFocusDirection2DRelativeToUnit but that dispatches also on unit movements and that interferes with the idle_auto_turn logic.
		activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::GroundDown);
		activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::EntityDown)
			->addFlagsBitMap(BoolUtil::setBitAtIndexDirect(0, rpg3D::WorldOrEntityInteractionFilter::Flags::NotSelf))
			->addEntityTag(rpg3D::EntityTags::PLATFORM, true);
	} else if (destinationType == GroundOnly) {
		if (usesGridMovement) {
			activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::GroundDown)
				->setWorldPosCheck(std::bind(&ToolMGlobalDestination_TMoveDirection::customFilterCheck_worldPos, this, std::placeholders::_1));
		} else {
			activationFiltersList_alwaysUpdate->append_emplace((int)rpg3D::WorldOrEntityInteractionFilter::Type::GroundDown);
		}
	} else {
		throw LogicException(LOC);
	}
}

bool ToolMGlobalDestination_TMoveDirection::customFilterCheck_worldPos(Vector3& worldPos) {
	if (gpGrid2DContent == nullptr) {
		// This may occur on some single transition frames;
		return false;
	}

	Vector2Int gPos{};
	gpGrid2DContent->worldPosToGPos(worldPos, gPos);

	if (!gpGrid2DContent->isGPosInTrackingRegion(gPos)) {
		return false;
	}
	
	if (gpGrid2DContent->getIsGCellOccupied(gPos.x, gPos.y)) {
		return false;
	}

	return true;
}

void ToolMGlobalDestination_TMoveDirection::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		userControlledModule = nullptr;
		movingEntity = nullptr;
	} else {
		userControlledModule = entity->getComponentAs<rpg3D::IUserControlledModule*>(true/*mustExist*/);
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	}
}

void ToolMGlobalDestination_TMoveDirection::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		//void
	}else {
		if (gwGamePartsOptional != nullptr && usesGridMovement && gpGrid2DContent == nullptr) {
			gpGrid2DContent = gwGamePartsOptional->getComponentAs<rpg3D::IGPGrid2DContent*>(true/*mustExist*/);
		}
	}
}

bool ToolMGlobalDestination_TMoveDirection::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if (filterSource == FilterSource::alwaysUpdate) {
		if(interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::GroundDown) {
			// Add a moveToDestination command to the destination location.
			Vector2 p{ interaction->worldPos.x, interaction->worldPos.y };

			if (usesGridMovement) {
				if (gpGrid2DContent == nullptr) {
					throw LogicException(LOC);
				}

				Vector2Int gPos{};
				gpGrid2DContent->worldPosToGPos(p, gPos);

				if (!gpGrid2DContent->isGPosInTrackingRegion(gPos)) {
					// Ignore movement requests to outside of the grid area.

					// Return true to mark the filter as processed even if not used.
					return true;
				}

				// Convert the destination pos to a gCell center.
				gpGrid2DContent->gPosCenterToWorldPos(gPos, p);
			}

			// The movement always occurs to handle the case where:
			/// while the unit is moving, the user taps on the unit exact position and the unit should stop.
			/*if (entity->getPos()->equals(interaction->worldPos)) {
				goto end;
			}*/

			movingEntity->addAction_ActionToTargetMovementContinuous(
				IMovingEntityModule::ControlType::LocalEntityDirection_BackForwardOnly,
				p, nullptr,
				0.0f, true,
				true/*isAutoTurn*/, true/*isInstantTurn*/,
				StringUtil::empty/*initiatorIdentifier*/, movementTypeOverride/*movementTypeOverride*/
			);
		}else if(interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::EntityDown) {
			if(interaction->targetEntity == entity || usesGridMovement) {
				// The player entity is filtered out of from touch events in the current logic.
				// Or grid movement is not allowed with this filter type, the constructor didn't add this filter and this should never occur.
				throw LogicException(LOC);
			}

			if(entity->getPos()->equals(*interaction->targetEntity->getPos())) {
				movingEntity->addAction_stopMovement(true/*stopPhysicsLinearVelocitiesAlso*/);

				float angDZ = Math::normalizeAngleDeg(entity->getRot()->getAngleAround(Vector3::Z));

				// This version jumps by -45 degrees, no matter what.
				// The input is rounded to the nearest of 12 quadrants.
				// Looks best out of the other variants.
				float quadrantAngle = 360.0f / 12.0f;
				angDZ = angDZ / quadrantAngle;
				angDZ = Math::round(angDZ);
				angDZ *= quadrantAngle;
				movingEntity->addAction_ActionDirectRotateZ(worldGame3D::EntityAction::Priorities::Normal, angDZ - 45.0f);

				// This version jumps by -45 degrees, no matter what.
				// It is hard to align to certain fixed angles (eg: front, 45 degrees, ...);
				//doAction_player_setRotDZ(angDZ - 45.0f);

				// This version snaps to the next -45 degrees increment.
				// The initial jump looks bad (most of the times it is too small).
				//if(angDZ <= -180.0f + 45.0f * 1.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 0.0f);
				//}else if(angDZ <= -180.0f + 45.0f * 2.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 1.0f);
				//}else if(angDZ <= -180.0f + 45.0f * 3.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 2.0f);
				//}else if(angDZ <= -180.0f + 45.0f * 4.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 3.0f);
				//}else if(angDZ <= -180.0f + 45.0f * 5.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 4.0f);
				//}else if(angDZ <= -180.0f + 45.0f * 6.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 5.0f);
				//}else if(angDZ <= -180.0f + 45.0f * 7.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 6.0f);
				//}else if(angDZ <= -180.0f + 45.0f * 8.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 7.0f);
				//}else if(angDZ <= -180.0f + 45.0f * 9.0f) {
				//    doAction_player_setRotDZ(-180.0f + 45.0f * 8.0f);
				//}else {
				//    throw LogicException(LOC);
				//}

				// This version snaps to -90 degrees the first time and after that it jumps by -45 degrees.
				// The initial jump looks bad (it is too big most of the times).
				//if(Math::isEqual(angDZ, -135.0f, 0.001f)) {
				//    doAction_player_setRotDZ(-135.0f - 45.0f);
				//}else if(Math::isEqual(angDZ, -90.0f, 0.001f)) {
				//    doAction_player_setRotDZ(-90.0f - 45.0f);
				//}else if(Math::isEqual(angDZ, -45.0f, 0.001f)) {
				//    doAction_player_setRotDZ(-45.0f - 45.0f);
				//}else if(Math::isEqual(angDZ, 0.0f, 0.001f)) {
				//    doAction_player_setRotDZ(0.0f - 45.0f);
				//}else if(Math::isEqual(angDZ, 45.0f, 0.001f)) {
				//    doAction_player_setRotDZ(45.0f - 45.0f);
				//}else if(Math::isEqual(angDZ, 90.0f, 0.001f)) {
				//    doAction_player_setRotDZ(90.0f - 45.0f);
				//}else if(Math::isEqual(angDZ, 135.0f, 0.001f)) {
				//    doAction_player_setRotDZ(135.0f - 45.0f);
				//}else if(Math::isEqual(angDZ, 180.0f, 0.001f)) {
				//    doAction_player_setRotDZ(180.0f - 45.0f);
				//}else {
				//    doAction_player_setRotDZ(-90.0f);
				//}
			}else {
				float targetRange;
				if(interaction->targetEntity->hasComponent(rpg3D::IPlatformModule::TYPE) || interaction->targetEntity->getTemplate()->hasTag(rpg3D::EntityTags::PLATFORM)) {
					targetRange = 0.0f;
				}else {
					targetRange = 0.3f;
				}
				bool isStopAtTarget;
				if(interaction->targetEntity->hasModule(IMovingEntityModule::TYPE)) {
					// If the target can move, follow it until further notice.
					isStopAtTarget = false;
				}else {
					isStopAtTarget = true;
				}

				// Dispatch a moveToDestination to command.
				Vector2 p{};
				movingEntity->addAction_ActionToTargetMovementContinuous(
					IMovingEntityModule::ControlType::LocalEntityDirection_BackForwardOnly,
					p, interaction->targetEntity->getWrapper(),
					targetRange, isStopAtTarget,
					true/*isAutoTurn*/, true/*isInstantTurn*/,
					StringUtil::empty/*initiatorIdentifier*/, movementTypeOverride/*movementTypeOverride*/
				);
			}
		}else {
			localIsProcessed = false;
		}
	} else {
		localIsProcessed = false;
	}
//end:

	if (isProcessed && localIsProcessed) {
		// Filter used in multiple layers. That should never occur. Filters code is not made for that.
		throw LogicException(LOC);
	}

	return isProcessed || localIsProcessed;
}

void ToolMGlobalDestination_TMoveDirection::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	if (usesGridMovement && gpGrid2DContent == nullptr) {
		gpGrid2DContent = gwGameParts->getComponentAs<rpg3D::IGPGrid2DContent*>(true/*mustExist*/);
	}
}

void ToolMGlobalDestination_TMoveDirection::disconnectFromGamePartsPre() {
	gpGrid2DContent = nullptr;
}

ToolMGlobalDestination_TMoveDirection::~ToolMGlobalDestination_TMoveDirection() {
	//void
}
