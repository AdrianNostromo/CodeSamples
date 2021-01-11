#include "IInventoryModule.h"
#include <worldGame3D/gw/entity/event/EntityEvent.h>

using namespace rpg3D;

int IInventoryModule::TYPE = GetNew_TYPE();
DEF_componentBasics(IInventoryModule);

base::EntityEvent* IInventoryModule::EVENT_inventory_goldAmountChanged = new base::EntityEvent(false, "EVENT_inventory_goldAmountChanged");
