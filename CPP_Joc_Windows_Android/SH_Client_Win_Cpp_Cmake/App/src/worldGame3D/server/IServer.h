#pragma once

#include <worldGame3D/server/util/ClientSessionInformation.h>
#include <worldGame3D/server/componentipport/IComponentIpPort.h>

namespace worldGame3D {
class INetConsManager;
class ConnectedDeviceInformation;

class IServer {
public:
    virtual bool getIsAccountConnectPrefferedState() = 0;
    virtual void setIsAccountConnectPrefferedState(bool isAccountConnectPrefferedState) = 0;

    virtual ConnectedDeviceInformation* getDeviceInformation() = 0;

    virtual bool isSessionConnected() = 0;
    virtual bool isSessionConnectedOrConnecting() = 0;
    virtual ClientSessionInformation* getSessionInformation() = 0;

    virtual INetConsManager* getNetConsManager() = 0;

    virtual IComponentIpPort* getComponentIpPort() = 0;

    virtual ~IServer() = default;
};
};
