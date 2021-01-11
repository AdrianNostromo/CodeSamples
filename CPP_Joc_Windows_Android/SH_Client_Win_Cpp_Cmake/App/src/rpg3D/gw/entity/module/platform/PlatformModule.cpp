#include "PlatformModule.h"
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <rpg3D/gw/entity/module/physics3D/util/PhysicsEntityCallbackFilter.h>
#include <rpg3D/gw/entity/module/platformInteractor/IPlatformInteractorModule.h>
#include <rpg3D/gw/config/physics/PhysicsCollissionConfig.h>

using namespace rpg3D;

base::EntityEvent* PlatformModule::EVENT_onPlatformContactAdded = new base::EntityEvent(true, "EVENT_onPlatformContactAdded");
base::EntityEvent* PlatformModule::EVENT_onPlatformContactRemoved = new base::EntityEvent(true, "EVENT_onPlatformContactRemoved");
base::EntityEvent* PlatformModule::EVENT_Platform_activeStateChanged = new base::EntityEvent(false, "EVENT_Platform_activeStateChanged");

PlatformModule::PlatformModule(
    IWorldEntity* entity,
    PlatformTemplate* t)
    :super(t->moduleName, entity),
	t(t)
{
    //void
}

void PlatformModule::createBMain() {
	super::createBMain();

	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(true/*mustExist*/);
	activeFilter = physics3DModule->addEntityContactFilter(std::bind(&PlatformModule::onPlatformInteractorContactChange, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	// Only test for units. This is a fast test and happens before the unit_speciffic_tests.
	activeFilter->setCollissionMaskBitMapOneWay(rpg3D::PhysicsCollissionConfig::Unit->groups)
		//->addFlagsBitMap(BoolUtil::setBitAtIndexDirect(0, rpg3D::WorldOrEntityInteractionFilter::Flags::NotSelf, rpg3D::WorldOrEntityInteractionFilter::Flags::Team_Same_Or_NoTeamModule))
		//->addEntityTag(rpg3D::EntityTags::UNIT, true)
		->addEntityModuleRequirement(rpg3D::IPlatformInteractorModule::TYPE);
}

void PlatformModule::onPlatformInteractorContactChange(IPhysicsEntityCallbackFilter* filter, IWorldEntity* otherEntity, bool isContact) {
	if (!isContact) {
		if (activeFilter->getActiveContactsCount() == 0) {
			e->dispatchEvent(*EVENT_Platform_activeStateChanged);
		}

		e->dispatchEvent(*EVENT_onPlatformContactRemoved->setEntityData(e, otherEntity));
	} else {
		e->dispatchEvent(*EVENT_onPlatformContactAdded->setEntityData(e, otherEntity));

		if (activeFilter->getActiveContactsCount() == 1) {
			e->dispatchEvent(*EVENT_Platform_activeStateChanged);
		}
	}
}

bool PlatformModule::getIsActive() {
	if(activeFilter->getActiveContactsCount() > 0) {
        return true;
    }

    return false;
}

std::shared_ptr<std::string> PlatformModule::getActivation_script() {
	return activation_script;
}

void PlatformModule::setActivation_script(std::shared_ptr<std::string> activation_script) {
	this->activation_script = activation_script;
}

PlatformModule::~PlatformModule() {
    //void
}
