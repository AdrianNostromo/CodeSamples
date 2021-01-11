#include "IOGPMobsSpawnSystem.h"
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

int IOGPMobsSpawnSystem::TYPE = GetNew_TYPE();

DEF_componentBasics(IOGPMobsSpawnSystem);

std::string IOGPMobsSpawnSystem::FLAG_isZoneCleared = "isZoneCleared";
