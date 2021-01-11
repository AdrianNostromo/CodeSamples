#include "ICustomCubesCannonTurretModule.h"

using namespace towerDefense_a_v1;

int ICustomCubesCannonTurretModule::TYPE = GetNew_TYPE();
DEF_componentBasics(ICustomCubesCannonTurretModule);

base::EntityEvent* ICustomCubesCannonTurretModule::EVENT_ReloadFinished = new base::EntityEvent(false, "EVENT_ReloadFinished");

base::EntityEvent* ICustomCubesCannonTurretModule::EVENT_ProjectileLaunched = new base::EntityEvent(false, "EVENT_ProjectileLaunched");
base::EntityEvent* ICustomCubesCannonTurretModule::EVENT_ProjectileLaunchCancelled_TargetLost = new base::EntityEvent(false, "EVENT_ProjectileLaunchCancelled_TargetLost");
