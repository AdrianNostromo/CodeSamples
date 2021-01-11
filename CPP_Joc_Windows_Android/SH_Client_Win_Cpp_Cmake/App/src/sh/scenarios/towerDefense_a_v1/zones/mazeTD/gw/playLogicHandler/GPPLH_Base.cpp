#include "GPPLH_Base.h"
#include <worldGame3D/gw/entity/util/EntityRemoveReason.h>
#include <rpg3D/gw/ui/IGamePartUserInterface.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <sh/scenarios/towerDefense_a_v1/zones/mazeTD/gw/views/mainui/IMainInGameUIView.h>
#include <rpg3D/gw/ui/GPUIMainUIView.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <memory>
#include <worldGame3D/gw/handler/GWEventDataBase.h>
#include <worldGame3D/scripts/ScriptCommands.h>

using namespace towerDefense_a_v1;
using namespace towerDefense_a_v1::MazeTD;

EntityRemoveReason* const GPPLH_Base::ENT_REM_REASON_PURPOSE_FINISHED = new EntityRemoveReason("ENT_REM_REASON_PURPOSE_FINISHED", true, true);
EntityRemoveReason* const GPPLH_Base::ERR_DESTINATION_REACHED = new EntityRemoveReason("ERR_DESTINATION_REACHED", true, true);

GPPLH_Base::GPPLH_Base(base::IGameWorld* gw)
	: super(gw)
{
	//void
}

void GPPLH_Base::createBMain() {
	super::createBMain();

	rpg3D::IGamePartUserInterface* gpUI = gw->getComponentAs<rpg3D::IGamePartUserInterface*>(true/*mustExist*/);
	mainUIVIew = gpUI->getMainUiViewAs<IMainInGameUIView*>();
	if (mainUIVIew == nullptr) {
		throw LogicException(LOC);
	}
}

int GPPLH_Base::getCurrentLives() {
	return currentLives;
}

void GPPLH_Base::setCurrentLives(int currentLives) {
	if (this->currentLives == currentLives) {
		return;
	}
	this->currentLives = currentLives;

	mainUIVIew->setCurrentLives(this->currentLives);

	if (this->currentLives <= 0) {
		// Level loss occured.
		std::shared_ptr<std::string> command = std::make_shared<std::string>(
			worldGame3D::ScriptCommands::cmd_setScenarioAndZone + worldGame3D::ScriptCommands::commandPartsDelimiter
			+ startarea_a_v1::ScenarioFactory::scenarioTemplate->id
		);

		std::shared_ptr<base::GWEventDataBase> data = std::make_shared<base::GWEventDataBase>();
		data->paramSPS = command;

		gw->getHandler()->onGWEvent(
			base::IGameWorld::GWE_RequestRunScript,
			data
		);
	}
}

int GPPLH_Base::getCurrentGold() {
	return currentGold;
}

void GPPLH_Base::setCurrentGold(int currentGold) {
	if (this->currentGold == currentGold) {
		return ;
	}
	this->currentGold = currentGold;

	mainUIVIew->setCurrentGold(this->currentGold);
}

GPPLH_Base::~GPPLH_Base() {
	//void
}
