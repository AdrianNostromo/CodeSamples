#include "IMovingEntityModule.h"
#include <worldGame3D/gw/entity/module/actions/action/EntityAction.h>

int IMovingEntityModule::TYPE = GetNew_TYPE();
DEF_componentBasics(IMovingEntityModule);

const int IMovingEntityModule::ControlType::Global_Dir = 1;
const int IMovingEntityModule::ControlType::LocalEntityDirection_Strafe = 2;
// This is the same as strafe but the entity can move only back or front in the direction it is facing.
//asdA2;// this is currently not implemented, it just global_strafe-s to the destination and instant turns. Implement when this type of movement is really needed (eg: planes, rockets, innertial stuff);
//asdA2;// The min_move_speed_s and slow down when turning is handled by the logic block, not this.
//asdA2;// Maybe remove this and make the logickBlock with some custom code and LocalEntityDirection_Strafe.
const int IMovingEntityModule::ControlType::LocalEntityDirection_BackForwardOnly = 3;

int IMovingEntityModule::ActionGroup_Move = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();
int IMovingEntityModule::ActionGroup_Turn = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

int IMovingEntityModule::ActionGroup_DirectionalTurnZ = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

base::EntityEvent* IMovingEntityModule::EVENT_move_Start = new base::EntityEvent(false, "EVENT_move_Start");
base::EntityEvent* IMovingEntityModule::EVENT_move_Stop = new base::EntityEvent(false, "EVENT_move_Stop");
base::EntityEvent* IMovingEntityModule::EVENT_move_destinationReached = new base::EntityEvent(false, "EVENT_move_destinationReached");

base::EntityEvent* IMovingEntityModule::EVENT_teleport_Start = new base::EntityEvent(false, "EVENT_teleport_Start");

base::EntityEvent* IMovingEntityModule::EVENT_turn_Start = new base::EntityEvent(false, "EVENT_turn_Start");
base::EntityEvent* IMovingEntityModule::EVENT_turn_Stop = new base::EntityEvent(false, "EVENT_turn_Stop");
