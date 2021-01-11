#include <worldGame3D/gw/sim/GamePartSimExtendable.h>
#include <sh/scenarios/startarea_a_v1/gw/content/IGPContentLocal.h>
#include "GPManager.h"

using namespace startarea_a_v1;

GPManager::GPManager(base::IGameWorld* gw)
    : super(gw)
{
	gw->addEventListener(
		IGPContentLocal::GWEvent_onSelectedPlayerEntityChanged->type,
		std::bind(&GPManager::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPManager::createBMain() {
	super::createBMain();

	//void
}

void GPManager::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == IGPContentLocal::GWEvent_onSelectedPlayerEntityChanged->type) {
		onPlayerEntityChanged();
	}else {
		throw LogicException(LOC);
	}
}

void GPManager::onPlayerEntityChanged() {
	gw->getSimAs<base::GamePartSimExtendable*>()->setCameraFollowTarget(gw->getContent()->getSelectedPlayerEntity(true/*mustExist*/));
}

GPManager::~GPManager() {
    //void
}
