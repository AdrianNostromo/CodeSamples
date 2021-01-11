#pragma once

#include "ServerNetConsManager.h"

namespace worldGame3D {
class ServerDevice : public ServerNetConsManager {
private: typedef ServerNetConsManager super;
protected:
    static const bool IS_DEBUG_ACCOUNT;

protected:
    ConnectedDeviceInformation* deviceInformation = nullptr;

public:
    ServerDevice(ServerDevice const&) = delete;
    ServerDevice(ServerDevice &&) = default;
    ServerDevice& operator=(ServerDevice const&) = delete;
    ServerDevice& operator=(ServerDevice &&) = default;

    explicit ServerDevice(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app);

    ConnectedDeviceInformation* getDeviceInformation() override;

    bool isDeviceConnected();

    void deviceConnectOccured(int deviceId, std::shared_ptr<std::string> deviceKey);

    virtual void userDisconnectOccuredDetected();

    ~ServerDevice() override;
protected:
    void createPost() override;

    virtual void onAccountInteractionFailed();

private:
    void clearDeviceInformation();

};
};
