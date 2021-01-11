#include "SH_ActiveScenario.h"
#include <iostream>
#include <base/exceptions/UnhandledCaseException.h>
#include <base/exceptions/LogicException.h>
#include <base/app/IApp.h>
#include <base/app/event/AppEvent.h>
#include <worldGame3D/gw/contentZones/IGamePartContentZones.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace worldGame3D;

SH_ActiveScenario::SH_ActiveScenario(
    IRenderingHandler* _renderingHandler,
    IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
    IContainer2D* layer2DGameWorld,
	IApp* app)
	: super(appAssets, appMetrics, appUtils, layer2DGameWorld, app),
	renderingHandler(_renderingHandler)
{
	app->addEventListener(
		IApp::Event_isAppActiveChangePre->type,
		std::bind(&SH_ActiveScenario::onAppEvent, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
}

void SH_ActiveScenario::onAppEvent(IEventListener& eventListener, base::AppEvent& event) {
	if (event.type == IApp::Event_isAppActiveChangePre->type) {
		if (app->getIsAppActive()) {
			// The app is active and will become innactive, do a data save.
			if (gameWorld != nullptr) {
				gameWorld->onAppBecomingInnactive();
			}
		}
	} else {
		throw LogicException(LOC);
	}
}

void SH_ActiveScenario::create() {
	super::create();

	app->getLoopDistribution()->addEventListener(
		base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
		std::bind(&SH_ActiveScenario::onLoopEvent_appLoop_general, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
}

int SH_ActiveScenario::setActiveScenario(
	std::string scenarioId, ScenarioConfig* scenarioConfig, bool forceScenarioRestartIfSame,
	std::string optionalZoneName, bool forceZoneRestartIfSame, std::shared_ptr<worldGame3D::ZoneStartExtraData> zoneStartExtraData)
{
	this->targetScenario_id = scenarioId;
	this->targetScenario_config = scenarioConfig;
	this->forceScenarioRestartIfSame = forceScenarioRestartIfSame;

	this->optionalZoneName = optionalZoneName;
	this->forceZoneRestartIfSame = forceZoneRestartIfSame;
	this->zoneStartExtraData = zoneStartExtraData;

	this->isDirty_scenarioTarget = true;

	return 0;
}

void SH_ActiveScenario::disposeScenario() {
	if (scenarioTemplate != nullptr) {
		std::cout << "Game scenario disposed with id: " << scenarioTemplate->id << std::endl;

		gpContentZones->setActiveZone(
			"", nullptr/*zoneStartExtraData*/, false/*forceZoneRestartIfSame*/,
			false/*useDelayedOnLoopActivation*/
		);

		gameWorld->setIsPlayEnabled(false);

        gameWorld->setIsEnabledIncremental(false);

		onActiveScenarioDeactivatePre(gameWorld);
		
		gameWorld->prepareForDisposal();

        if(gameWorld->getGameWorldContainer2D()->getParent() != nullptr) {
            gameWorld->getGameWorldContainer2D()->getParent()->removeChild(gameWorld->getGameWorldContainer2D());
        }

        removeComponent(gameWorld, true/*doDispose*/);

		gameWorld = nullptr;
		gpContentZones = nullptr;
		scenarioTemplate = nullptr;
	}
}

void SH_ActiveScenario::onActiveScenarioDeactivatePre(base::IGameWorld* gameWorld) {
	//void
}

void SH_ActiveScenario::onActiveScenarioActivated(base::IGameWorld* gameWorld) {
	//void
}

void SH_ActiveScenario::loopFinished() {
	// This was used to change scenarios when a scenario was active. No longer use.
	// Is called after the game loops finish.
}

ScenarioTemplate* SH_ActiveScenario::getActiveScenarioTemplate() {
    return scenarioTemplate;
}

base::IGameWorld* SH_ActiveScenario::getActiveScenatioGameWorld() {
	return gameWorld;
}

void SH_ActiveScenario::onGWEvent(std::string id, std::shared_ptr<base::GWEventDataBase> data) {
    throw UnhandledCaseException();
}

void SH_ActiveScenario::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
	if(isDirty_scenarioTarget) {
		isDirty_scenarioTarget = false;

		// Only update if different.
		if (gameWorld == nullptr && targetScenario_id.empty()) {
			// Same nullptr scenario.
			return ;
		}else if (gameWorld != nullptr && gameWorld->getScenarioTemplate()->id == targetScenario_id) {
			//Same scenario.
			if (!forceScenarioRestartIfSame) {
				std::string targetZoneName;
				if (optionalZoneName.empty() || optionalZoneName == "default") {
					// Zones are currenntly optional. Must have no zones registered tough.
					if (scenarioTemplate->defaultZoneName.empty() && scenarioTemplate->zonesFactoriesMap.count() > 0) {
						// The scenario needs to have a default_zone if it has any zones registered (it uses zones).
						throw LogicException(LOC);
					}

					targetZoneName = scenarioTemplate->defaultZoneName;
				} else {
					if (optionalZoneName == "same") {
						if (gpContentZones->getActiveZoneName() == "") {
							// Same zone requested but there is no zone active.
							throw LogicException(LOC);
						}
						targetZoneName = gpContentZones->getActiveZoneName();
					} else {
						targetZoneName = optionalZoneName;
					}
				}
				if (!targetZoneName.empty()) {
					// If zone changed, update it or force update it.
					if (!targetZoneName.empty() && (gpContentZones->getActiveZoneName() != targetZoneName || forceZoneRestartIfSame)) {
						gpContentZones->setActiveZone(
							targetZoneName, zoneStartExtraData/*zoneStartExtraData*/, forceZoneRestartIfSame,
							false/*useDelayedOnLoopActivation*/
						);
					}
				}

				return;
			}
		}
		
		disposeScenario();

		if (!targetScenario_id.empty()) {
			std::cout << "Game scenario created with id: " << targetScenario_id << std::endl;

			if(targetScenario_config == nullptr) {
				throw LogicException(LOC);
			}

			scenarioTemplate = ScenarioTemplate::entriesMap.getDirect(targetScenario_id, nullptr);
			if (scenarioTemplate == nullptr) {
				throw LogicException(LOC);
			}

			gameWorld = scenarioTemplate->worldFactory->newInstance(
				this,
				scenarioTemplate,
				targetScenario_config
			);
			gameWorld->setCb_onGamePartsCreated(std::bind(&SH_ActiveScenario::gameWorld_onGamePartsCreated, this, std::placeholders::_1));
			
			addComponent(gameWorld, true/*doCreate*/);

			// Activate the override or the default zone.
			gpContentZones = gameWorld->getComponentAs<worldGame3D::IGamePartContentZones*>(true/*mustExist*/);
			if (!gpContentZones->getActiveZoneName().empty()) {
				// There must not be a default zone with the new system. The logic below will handle the default zone.
				throw LogicException(LOC);
			}

			std::string targetZoneName;
			if (!optionalZoneName.empty()) {
				targetZoneName = optionalZoneName;
			} else {
				// Zones are currenntly optional. Must have no zones registered tough.
				if (scenarioTemplate->defaultZoneName.empty() && scenarioTemplate->zonesFactoriesMap.count() > 0) {
					// The scenario needs to have a default_zone if it has any zones registered (it uses zones).
					throw LogicException(LOC);
				}

				targetZoneName = scenarioTemplate->defaultZoneName;
			}
			if (!targetZoneName.empty()) {
				gpContentZones->setActiveZone(
					targetZoneName, zoneStartExtraData/*zoneStartExtraData*/, false/*restartZoneIfActive*/,
					false/*useDelayedOnLoopActivation*/
				);
			}
			
			getLayer2DGameWorld()->addChild(gameWorld->getGameWorldContainer2D());

			gameWorld->setIsEnabledIncremental(true);

			gameWorld->startLevelPlay();
		}
	}
}

void SH_ActiveScenario::gameWorld_onGamePartsCreated(base::IGameWorld* gameWorld) {
	if (this->gameWorld != gameWorld) {
		throw LogicException(LOC);
	}

	onActiveScenarioActivated(gameWorld);
}

void SH_ActiveScenario::disposeMainPre() {
	disposeScenario();

	super::disposeMainPre();
}

SH_ActiveScenario::~SH_ActiveScenario() {
	//void
}
