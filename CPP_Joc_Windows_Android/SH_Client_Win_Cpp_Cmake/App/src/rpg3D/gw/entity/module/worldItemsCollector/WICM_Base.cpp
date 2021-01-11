#include "WICM_Base.h"
#include "../../template/worldItemsCollector/WorldItemsCollectorTemplate.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <base/math/util/BoolUtil.h>
#include <rpg3D/gw/entity/module/userControlled/util/WorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>
#include <rpg3D/gw/entity/module/worldItem/IWorldItemModule.h>
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include <rpg3D/gw/projectile/EffectPayload.h>

using namespace rpg3D;

WICM_Base::WICM_Base(
	IWorldEntity* entity,
	WorldItemsCollectorTemplate* t)
	: super(entity),
	t(t)
{
	//void
}

void WICM_Base::createBMain() {
	super::createBMain();

	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(true/*mustExist*/);

	activeWorldItemFilter = physics3DModule->addEntityContactFilter(std::bind(&WICM_Base::onWorldItemContactChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	// Only test for units. This is a fast test and happens before the unit_speciffic_tests.
	activeWorldItemFilter->setCollissionMaskBitMapOneWay(rpg3D::PhysicsCollissionConfig::Unit->groups)
		->addFlagsBitMap(BoolUtil::setBitAtIndexDirect(0,
			rpg3D::WorldOrEntityInteractionFilter::Flags::NotSelf
		))
		//->addEntityTag(rpg3D::EntityTags::UNIT, true)
		->addEntityModuleRequirement(IWorldItemModule::TYPE);
}

void WICM_Base::onWorldItemContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact) {
	if (!isContact) {
		//void
	} else {
		if (e->getIsMarkedForRemoval() || otherEntity->getIsMarkedForRemoval()) {
			return;
		}

		// Give item to the player.
		IWorldItemModule* otherEntity_worldItem = otherEntity->getComponentAs<rpg3D::IWorldItemModule*>(true/*mustExist*/);

		ArrayList<std::shared_ptr<rpg3D::EffectPayload>>* payloadsList = otherEntity_worldItem->peekPayloadsList();
		for (int i = 0; i < payloadsList->count(); i++) {
			std::shared_ptr<rpg3D::EffectPayload>& payloadEntry = payloadsList->getReference(i);

			payloadEntry->transferToEntity(otherEntity/*selfEntity*/, e/*otherEntity*/);
		}

		otherEntity_worldItem->onItemCollected();
	}
}

WICM_Base::~WICM_Base() {
	//void
}
