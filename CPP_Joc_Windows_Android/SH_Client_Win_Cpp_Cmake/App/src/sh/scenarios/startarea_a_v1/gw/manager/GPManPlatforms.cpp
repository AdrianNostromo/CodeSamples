#include "GPManPlatforms.h"
#include <rpg3D/gw/entity/module/platformInteractor/PlatformInteractorModule.h>
#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityIds.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityIds.h>
#include <rpg3D/gw/entity/module/stackable/IStackableModule.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <rpg3D/gw/config/entity/EntityTags.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace startarea_a_v1;

GPManPlatforms::GPManPlatforms(base::IGameWorld* gw)
    : super(gw)
{
    gw->addEventListener(
	    GWEvent_onPlayerActivatedPlatformNonSpawn->type,
	    std::bind(&GPManPlatforms::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
    );
}

void GPManPlatforms::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == GWEvent_onPlayerActivatedPlatformNonSpawn->type) {
		IWorldEntity* platformEntity = event.getEntityData();

		if (platformEntity->getInstanceId() == EntityIds::platformCreatureCreator) {
			// This has a customLogic module that handles this.

			// void
		}
	}else {
		throw LogicException(LOC);
	}
}

GPManPlatforms::~GPManPlatforms() {
    //void
}
