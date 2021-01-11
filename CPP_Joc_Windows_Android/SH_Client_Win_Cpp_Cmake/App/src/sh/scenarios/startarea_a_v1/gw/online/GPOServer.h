#pragma once

#include <base/gh.h>
#include <sh/scenarios/startarea_a_v1/gw/online/server/IServer.h>
#include "GPOBase.h"
#include <sh/scenarios/startarea_a_v1/gw/online/server/test/TestServer.h>

namespace startarea_a_v1 {
class GPOServer : public GPOBase {priv typedef GPOBase super;pub dCtor(GPOServer);
	prot TestServer* server = nullptr;

    pub explicit GPOServer(base::IGameWorld* gw);

	priv void onGWEvent_gameLoop_netInput(IEventListener& eventListener, base::GWEvent& event);

	prot base::ServerExtendable* newServer() override;

    pub ~GPOServer() override;
};
};