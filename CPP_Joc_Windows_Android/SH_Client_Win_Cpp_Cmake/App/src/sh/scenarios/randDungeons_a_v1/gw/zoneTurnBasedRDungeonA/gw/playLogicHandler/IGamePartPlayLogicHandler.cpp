#include "IGamePartPlayLogicHandler.h"
#include <base/exceptions/LogicException.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

int IGamePartPlayLogicHandler::TYPE = GetNew_TYPE();

DEF_componentBasics(IGamePartPlayLogicHandler);
