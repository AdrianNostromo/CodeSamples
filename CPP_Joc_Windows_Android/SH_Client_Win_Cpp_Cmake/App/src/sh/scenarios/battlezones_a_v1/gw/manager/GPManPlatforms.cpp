#include "GPManPlatforms.h"
#include <rpg3D/gw/entity/module/platformInteractor/PlatformInteractorModule.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityIds.h>
#include <sh/scenarios/battlezones_a_v1/gw/zone/ZoneTemplatesList.h>

using namespace battlezones_a_v1;

std::string GPManPlatforms::GWE_RequestStartScenario_StartArea = "GWE_RequestStartScenario_StartArea";

GPManPlatforms::GPManPlatforms(base::IGameWorld* gw)
    : super(gw)
{
	//void
}

GPManPlatforms::~GPManPlatforms() {
    //void
}
