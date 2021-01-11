#include <sh/scenarios/battlezones_a_v1/gw/sim/GamePartSim.h>
#include <sh/scenarios/battlezones_a_v1/gw/content/IGPContent.h>
#include "GPManager.h"

using namespace battlezones_a_v1;

GPManager::GPManager(base::IGameWorld* gw)
    : super(gw)
{
	gw->addEventListener(
		IGPContent::GWEvent_onSelectedPlayerEntityChanged->type,
		std::bind(&GPManager::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPManager::createBMain() {
	super::createBMain();

	//void
}

void GPManager::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == IGPContent::GWEvent_onSelectedPlayerEntityChanged->type) {
		onPlayerEntityChanged();
	}else {
		throw LogicException(LOC);
	}
}

void GPManager::onPlayerEntityChanged() {
	gw->getSimAs<GamePartSim*>()->setCameraFollowTarget(gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/));
}

GPManager::~GPManager() {
    //void
}
