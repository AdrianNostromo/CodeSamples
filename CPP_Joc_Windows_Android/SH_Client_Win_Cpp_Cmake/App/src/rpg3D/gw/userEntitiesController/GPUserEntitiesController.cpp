#include "GPUserEntitiesController.h"
#include <rpg3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

GPUserEntitiesController::GPUserEntitiesController(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		IGPContent::GWEvent_onSelectedPlayerEntityChanged->type, base::IGameWorld::GWEvent_isPlayEnabled_changed->type,
		std::bind(&GPUserEntitiesController::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_localInput->type,
		std::bind(&GPUserEntitiesController::onGWEvent_gameLoop_localInput, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPUserEntitiesController::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == IGPContent::GWEvent_onSelectedPlayerEntityChanged->type) {
		onPlayerEntityChanged();
	}else if(event.type == base::IGameWorld::GWEvent_isPlayEnabled_changed->type) {
		if (gw->getIsPlayEnabled()) {
			clearInput();
		}
	}else {
		throw LogicException(LOC);
	}
}

void GPUserEntitiesController::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_localInput(deltaS, deltaMS, gameTimeS);

	if (isDirtyControlType) {
		isDirtyControlType = false;
	}
}

void GPUserEntitiesController::createBMain() {
	super::createBMain();

	if(gw->getGPContent()->getSelectedPlayerEntity(false/*mustExist*/) != nullptr) {
		setControlledEntity(gw->getGPContent()->getSelectedPlayerEntity(true/*mustExist*/));
	}
}

void GPUserEntitiesController::onPlayerEntityChanged() {
	setControlledEntity(gw->getGPContent()->getSelectedPlayerEntity(false/*mustExist*/));
}

void GPUserEntitiesController::onGWEvent_gameLoop_localInput(IEventListener& eventListener, base::GWEvent& event) {
	onGameLoop_localInput(event.deltaS, event.deltaMS, event.gameTimeS);//asdAA;// this will check the camera direction change also.
}

GPUserEntitiesController::~GPUserEntitiesController() {
	//void
}
