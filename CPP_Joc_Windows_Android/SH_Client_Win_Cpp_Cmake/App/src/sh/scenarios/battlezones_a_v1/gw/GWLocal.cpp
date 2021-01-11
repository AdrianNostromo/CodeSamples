#include "GWLocal.h"
#include <sh/scenarios/battlezones_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <base/map/Map1D.h>
#include <rpg3D/gw/config/entity/unit/UnitTemplates_PlayerA.h>
#include <worldGame3D/gw/entity/template/referenceFrame/ReferenceFrameTemplate.h>
#include <worldGame3D/gw/entity/module/referenceFrame/ReferenceFrameModule.h>
#include <worldGame3D/gw/entity/template/voxelisedStructure/VoxelisedStructureTemplate.h>
#include <worldGame3D/gw/entity/module/voxelisedStructure/VoxelisedStructureModule.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include <sh/scenarios/battlezones_a_v1/gw/main/GamePartMain.h>
#include <worldGame3D/gw/capabilities/ICapabilities.h>

using namespace battlezones_a_v1;

GWLocal::GWLocal(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
	: super(handler, scenarioTemplate, scenarioConfig)
{
	//void
}

void GWLocal::startLevelPlay() {
	super::startLevelPlay();

	setIsPlayEnabled(true);
}

int GWLocal::createComponents() {
    super::createComponents();

	getCapabilities().enableAllCapabilities();

    //void

    return 0;
}

void GWLocal::createPost() {
    super::createPost();

    getManagerAs<GPManager*>()->gwInitFinished();
}

GWLocal::~GWLocal() {
	//void
}
