#include "ToolStackablesHauler.h"
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <base/math/util/BoolUtil.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/stackablesHauler/config/ToolConfigStackablesHauler.h>
#include <base/math/Math.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionEvent.h>

using namespace rpg3D;

ToolStackablesHauler::ToolStackablesHauler(ToolConfigStackablesHauler* config)
	: super(config)
{
	activationFiltersList_innactiveForActivation->append_emplace((int)IWorldOrEntityInteractionFilter::Type::EntityDown)
		->addFlagsBitMap(BoolUtil::setBitAtIndexDirect(0, rpg3D::WorldOrEntityInteractionFilter::Flags::NotSelf, rpg3D::WorldOrEntityInteractionFilter::Flags::Team_Same_Or_NoTeamModule))
		//->addEntityTag(rpg3D::EntityTags::UNIT, true)
		->addEntityModuleRequirement(IStackableModule::TYPE);
}

void ToolStackablesHauler::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		stackable = nullptr;
		movingEntity = nullptr;
	} else {
		stackable = entity->getComponentAs<rpg3D::IStackableModule*>(true/*mustExist*/);
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(true/*mustExist*/);
	}
}

bool ToolStackablesHauler::onFilterInteraction(int filterSource, std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>& interaction) {
	bool isProcessed = super::onFilterInteraction(filterSource, interaction);

	// Also check locally to detect error where multiple layers use the same filters (The filters lists are not set up for that and it should never occur).
	bool localIsProcessed = true;
	if(filterSource == FilterSource::innactiveForActivation) {
		if(interaction->type == rpg3D::WorldOrEntityInteractionFilter::Type::EntityDown) {
			if (!canTrigger()) {
				goto end;
			}

			//GlobalAccessLogHandler::devLogHandler->post_debug((std::string("activate_EntityDown: ") + *targetEntity->getInstanceId()).c_str());

			IStackableModule* targetEntity_stackable = interaction->targetEntity->getComponentAs< IStackableModule*>(true/*mustExist*/);

			if (stackable->getIsCarryingIndirect(targetEntity_stackable)) {
				// Target entity is from the carried local stack.
				if (config->getCanThrow() && doThrowCarriedOnCarriedEntityTouchDown) {
					// Throw the carried entity in front of this entity.
					// The carried entity may be more up the stack, break the stack as needed.

					float entityRotZDeg = entity->getRot()->getAngleAround(Vector3::Z);

					Vector3 throwPos{
						entity->getX() + Math::cosDegToRad(entityRotZDeg) * config->throwRangeM,
						entity->getY() + Math::sinDegToRad(entityRotZDeg) * config->throwRangeM,
						entity->getZ()
					};
					
					if (config->throwPosXYGridSize > 0) {
						// Round throw pos to a 5cm grid and a 15deg angle so it is easier for the editor.
						throwPos.x = Math::round(throwPos.x / config->throwPosXYGridSize) * config->throwPosXYGridSize;
						throwPos.y = Math::round(throwPos.y / config->throwPosXYGridSize) * config->throwPosXYGridSize;
					}

					std::shared_ptr<float> throwRotZDeg = nullptr;
					if (config->throwAngDegZGridSize > 0) {
						throwRotZDeg = std::make_shared<float>(Math::round(entityRotZDeg / config->throwAngDegZGridSize) * config->throwAngDegZGridSize);
					}

					stackable->addAction_StackableThrowActionContinuous(
						interaction->targetEntity->getWrapper(),
						nullptr, std::make_shared<Vector3>(throwPos),
						config->throwRangeM,
						throwRotZDeg
					);
				}
			} else if(!targetEntity_stackable->getIsCarryingIndirect(stackable)) {
				// Target entity is from a remote stack.
				if (!stackable->getIsCarrying()) {
					// Not carrying anything, try to pickup.
					if (config->getCanPickup() && stackable->getT()->canHaul && targetEntity_stackable->getT()->canBeCarried) {
						// Pickup the target from the remote stack.
						// Break the other stack as needed.
						chargeCooldown();

						stackable->addAction_StackablePickupActionContinuous(interaction->targetEntity->getWrapper(), config->pickupRangeM);
					}
				} else {
					// This is carrying some. Try to throw some on top of the remote stack.
					if (config->getCanThrow() && targetEntity_stackable->getT()->canHaul && targetEntity_stackable->getT()->allowPlaceOnThis && stackable->getCarrying()->getT()->canBeCarried) {
						// Throw target on top of the remote stack.
						// The entire local stack is thrown on top of the remove stack.
						stackable->addAction_StackableThrowActionContinuous(
							stackable->getCarrying()->getEntity()->getWrapper(),
							targetEntity_stackable->getEntity()->getWrapper(), nullptr,
							config->throwRangeM,
							nullptr/*throwRotZDeg*/
						);
					}
				}
			}
		}else {
			localIsProcessed = false;
		}
	}else {
		localIsProcessed = false;
	}
end:

	if (isProcessed && localIsProcessed) {
		// Filter used in multiple layers. That should never occur. Filters code is not made for that.
		throw LogicException(LOC);
	}

	return isProcessed || localIsProcessed;
}

ToolStackablesHauler::~ToolStackablesHauler() {
	//void
}
