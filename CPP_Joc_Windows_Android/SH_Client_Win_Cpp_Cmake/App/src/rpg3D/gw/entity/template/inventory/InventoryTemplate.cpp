#include "InventoryTemplate.h"
#include <rpg3D/gw/entity/module/inventory/InventoryModule.h>

using namespace rpg3D;

InventoryTemplate::InventoryTemplate()
    : super("inventory", IInventoryModule::TYPE)
{
    //void
}

IEntityModule* InventoryTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    InventoryTemplate* t = dynamic_cast<InventoryTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    InventoryModule* mod = new InventoryModule(entity, t);

    return mod;
}

InventoryTemplate::~InventoryTemplate() {
    //void
}
