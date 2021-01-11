#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include <base/component/IComponent_AppComponent.h>
#include <base/net/tcp/GeneralTcpConnection.h>
#include "IListener_NetConHandler.h"

class IApp;
class IEventListener;
namespace base {
    class LoopEvent;
};

namespace base {
namespace Net {
class BasicNetConHandler : public base::ManagedObject, public virtual IComponent_AppComponent {priv typedef base::ManagedObject super;pub dCtor(BasicNetConHandler);
    priv class NetCodes {
        pub static const int SC_C_PING = 10010;
        pub static const int CS_R_PING_OK = 10011;
    };

    priv IApp* app;

    priv std::shared_ptr<GeneralTcpConnection> con;
    priv float idleTimeS = 0.0f;

    priv float idleThresholdS;

    priv IListener_NetConHandler* listener;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit BasicNetConHandler(IApp* app, std::shared_ptr<GeneralTcpConnection> con, float idleThresholdS, IListener_NetConHandler* listener);
    prot void create() override;
    prot void createPost() override;

    pub std::shared_ptr<GeneralTcpConnection> extractNetCon();
    pub std::shared_ptr<GeneralTcpConnection> getNetCon();

    prot void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event);

    priv void processConData(IGeneralTcpCon& con);
    priv bool onProcessMsg(IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer);
    priv bool processMsg_local(IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer);
    priv void processMsg_SC_C_PING(IGeneralTcpCon& con, int msgLen, int msgType, base::StreamBuffer& msgNetBuffer);
    priv void sendMsg_CS_R_PING_OK(IGeneralTcpCon& con);

    priv void onNetCon_logicError(BasicTcpConnection* target);
    priv void onNetCon_netError(BasicTcpConnection* target);

    prot void disposeMain() override;
    pub ~BasicNetConHandler() override;
    
};
};
};
