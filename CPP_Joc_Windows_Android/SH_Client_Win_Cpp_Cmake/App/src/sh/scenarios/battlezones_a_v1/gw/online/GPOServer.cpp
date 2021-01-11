#include <sh/scenarios/battlezones_a_v1/gw/online/server/test/TestServer.h>
#include "GPOServer.h"

using namespace battlezones_a_v1;

GPOServer::GPOServer(base::IGameWorld* gw)
    : super(gw)
{
	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_netInput->type,
		std::bind(&GPOServer::onGWEvent_gameLoop_netInput, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
};

void GPOServer::onGWEvent_gameLoop_netInput(IEventListener& eventListener, base::GWEvent& event) {
    if(server != nullptr) {
        server->onGameLoop_netInput(event.deltaS, event.deltaMS, event.gameTimeS);
    }
}

base::ServerExtendable* GPOServer::newServer() {
    TestServer* ret = new TestServer(gw->getScenarioConfig()->serverConnectConfig);

    server = ret;

    return ret;
}

GPOServer::~GPOServer() {
    //void
};
