#pragma once

#include <worldGame3D/server/componentipport/ComponentIpPort.h>
#include "ServerBase.h"

namespace worldGame3D {
class ServerComponentIpPort : public ServerBase {
private: typedef ServerBase super;
public:
    ComponentIpPort* componentIpPort = nullptr;

public:
    ServerComponentIpPort(ServerComponentIpPort const&) = delete;
    ServerComponentIpPort(ServerComponentIpPort &&) = default;
    ServerComponentIpPort& operator=(ServerComponentIpPort const&) = delete;
    ServerComponentIpPort& operator=(ServerComponentIpPort &&) = default;

    explicit ServerComponentIpPort(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app);

    IComponentIpPort* getComponentIpPort() override;

    ~ServerComponentIpPort() override;
protected:
    void create() override;

};
};
