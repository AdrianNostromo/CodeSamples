#include "TestServer.h"

using namespace startarea_a_v1;

TestServer::TestServer(std::shared_ptr<base::ServerConnectConfig> serverConnectConfig)
    : super(serverConnectConfig)
{
    //void
};

void TestServer::onGameLoop_netInput(float deltaS, int deltaMS, float gameTimeS) {
    if(!isA) {
        isA = true;

        //asdA9;// The con is single and has a cb for messages;
        TestNetCon con{};
        base::StreamBuffer buff{4};

        int SC_C_SetSelfStatus = 200100;
        processMsg(con, 4, SC_C_SetSelfStatus, buff);
    }
}

TestServer::~TestServer() {
    //void
};
