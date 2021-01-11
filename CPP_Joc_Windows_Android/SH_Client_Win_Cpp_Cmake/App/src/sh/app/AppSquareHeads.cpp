#include <sh/scenariohandler/ScenariosHandler.h>
#include <rpg3D/_dev/TestVoxelStructureComponents.h>
#include "AppSquareHeads.h"
#include <base/util/DeviceInfo.h>
#include <sh/scenarios/startarea_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/battlezones_a_v1/gw/config/ScenarioFactory.h>
#include <sh/scenarios/gridAdventure_a_v1/ScenarioFactory.h>
#include <sh/scenarios/roomgeons_a_v1/ScenarioFactory.h>
#include <sh/scenarios/zoneAlert_a_v1/ScenarioFactory.h>
#include <sh/scenarios/adventLandia/ScenarioFactory.h>
#include <sh/scenarios/customRoomgeons/ScenarioFactory.h>
#include <base/app/config/AppConfig.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>
#include <sh/viewsHandler/ViewsHandler.h>
#include <sh/view/mainsidebar/IMainSidebarView.h>
#include <sh/view/status/IStatusView.h>

using namespace sh;

AppSquareHeads::AppSquareHeads(
    int deviceType,
    INativeInput* nativeInputConnection,
    base::IProfileStorage* profileStorage,
    base::Net::INativeHttpHandler* nativeHttpHandler,
    base::Net::INativeSocketsHandler* nativeSocketsHandler,
	base::IFileSystem* fileSystem, IGL* gl,
    base::INativeConsole* nativeConsole,
    base::audio::INativeAudio* nativeAudio,
	base::ThreadsManager* threadsManager)
    : super(
		deviceType,
		nativeInputConnection,
        profileStorage,
        nativeHttpHandler,
        nativeSocketsHandler,
        fileSystem, gl,
        nativeConsole,
        nativeAudio,
		threadsManager)
{
    //void
}

void AppSquareHeads::create(void) {
    super::create();
    
    addComponent(tester = new Tester(this), true/*doCreate*/);
}

worldGame3D::ScenariosHandler* AppSquareHeads::newUninitialisedScenarioHandler() {
    ScenariosHandler* ret = new ScenariosHandler(
        getRenderingHandler(), this, this, this, layer2DGameWorld, this
    );

    return ret;
}

void AppSquareHeads::onInitialPreloadingFinished() {
    super::onInitialPreloadingFinished();

	// Make sure scenarios are registered for the start_by_string. This is needed in case the scenario template is not included and used somewhere.
	if (roomgeons_a_v1::ScenarioFactory::scenarioTemplate == nullptr
		|| zoneAlert_a_v1::ScenarioFactory::scenarioTemplate == nullptr
		|| adventLandia::ScenarioFactory::scenarioTemplate == nullptr
		|| customRoomgeons::ScenarioFactory::scenarioTemplate == nullptr)
	{
		throw LogicException(LOC);
	}

	std::string initialScenario;
	std::string optionalZoneName = "";
	
	//initialScenario = "startarea_a_v1";
	//optionalZoneName = "Main";

	//initialScenario = "towerDefense_a_v1";

	//initialScenario = "randDungeons_a_v1";
	//optionalZoneName = "Main";

	//initialScenario = "islandSurvival_a_v1";

	//initialScenario = "gridAdventure_a_v1";
	//optionalZoneName = "Main";
	
	initialScenario = "squareTactics_a_v1";
	optionalZoneName = "TownA";
	
	/*initialScenario = "adventLandia";
	optionalZoneName = "TownA";*/
	
	/*initialScenario = "customRoomgeons";
	optionalZoneName = "TownA";*/
	
	//initialScenario = "zoneAlert_a_v1";
	//optionalZoneName = "TownA";

	//initialScenario = "roomgeons_a_v1";
	//optionalZoneName = "Town";

	int r = getScenariosHandler()->setActiveScenario(
		initialScenario, new ScenarioConfig(nullptr/*serverConnectConfig*/), false/*forceScenarioRestartIfSame*/,
		optionalZoneName/*optionalZoneName*/, false/*forceZoneRestartIfSame*/);
    if (r != 0) {
        assert(false);
    }
}

base::ViewsHandler* AppSquareHeads::createInstance_ViewsHandler() {
	//return super::::createInstance_ViewsHandler();

	return newt sh::ViewsHandler(this);
}

void AppSquareHeads::createPost(void) {
	super::createPost();
	
	//void
}

void AppSquareHeads::onLoopEvent_appLoop_General(IEventListener& eventListener, base::LoopEvent& event) {
	super::onLoopEvent_appLoop_General(eventListener, event);

	//void
}

bool AppSquareHeads::tickAndGetIsInitialPreloadFinished(float deltaS) {
	bool isFinished = super::tickAndGetIsInitialPreloadFinished(deltaS);

	// The app start in offline mode currently.
//    if(!isInitialServerSessionConnected) {
//        if(server->isSessionConnected()) {
//            isInitialServerSessionConnected = true;
//
//            if(AppConfig::IS_DEBUG && IS_DEBUG) {
//                logger->postInfo("Preload(Initial phase) - Server session connect finished.");
//            }
//        }
//    }
//
//    if(isFinished) {
//        if(!isInitialServerSessionConnected) {
//            isFinished = false;
//        }
//    }

	return isFinished;
}

void AppSquareHeads::disposeMain() {
    //void

    super::disposeMain();
}

AppSquareHeads::~AppSquareHeads() {
    //void
}
