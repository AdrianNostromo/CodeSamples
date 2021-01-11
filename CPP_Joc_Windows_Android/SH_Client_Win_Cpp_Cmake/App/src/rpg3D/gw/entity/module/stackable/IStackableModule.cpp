#include "IStackableModule.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>

using namespace rpg3D;

int IStackableModule::TYPE = GetNew_TYPE();
DEF_componentBasics(IStackableModule);

int IStackableModule::ActionGroup_StackablePickupOrThrow = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();
int IStackableModule::ActionGroup_StackableTurning = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

base::EntityEvent* IStackableModule::EVENT_haulerChanged = new base::EntityEvent(false, "EVENT_haulerChanged");
base::EntityEvent* IStackableModule::EVENT_carryingChanged = new base::EntityEvent(false, "EVENT_carryingChanged");

base::EntityEvent* IStackableModule::EVENT_stackablePickup_Start = new base::EntityEvent(false, "EVENT_stackablePickup_Start");

base::EntityEvent* IStackableModule::EVENT_stackableThrow_Start = new base::EntityEvent(false, "EVENT_stackableThrow_Start");
