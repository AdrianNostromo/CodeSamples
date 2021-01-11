#pragma once

#include <worldGame3D/server/netconsmanager/NetConsManager.h>
#include "ServerComponentIpPort.h"

namespace worldGame3D {
class ServerNetConsManager : public ServerComponentIpPort {
private: typedef ServerComponentIpPort super;
private:
    NetConsManager* netConsManager = nullptr;

public:
    ServerNetConsManager(ServerNetConsManager const&) = delete;
    ServerNetConsManager(ServerNetConsManager &&) = default;
    ServerNetConsManager& operator=(ServerNetConsManager const&) = delete;
    ServerNetConsManager& operator=(ServerNetConsManager &&) = default;

    explicit ServerNetConsManager(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app);

    INetConsManager* getNetConsManager() override;

    ~ServerNetConsManager() override;
protected:
    void create() override;

};
};
