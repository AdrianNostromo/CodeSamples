#pragma once

#include "PlayerProfileDataLocalPlayerGems.h"

namespace base {
class PlayerProfileDataLocalAccount : public PlayerProfileDataLocalPlayerGems {
private: typedef PlayerProfileDataLocalPlayerGems super;
private:
    static std::string IS_DEVICE_CONNECTED;

    static std::string CONNECTED_DEVICE_ID;
    static std::string CONNECTED_DEVICE_KEY;

public:
    PlayerProfileDataLocalAccount(PlayerProfileDataLocalAccount const&) = delete;
    PlayerProfileDataLocalAccount(PlayerProfileDataLocalAccount &&) = default;
    PlayerProfileDataLocalAccount& operator=(PlayerProfileDataLocalAccount const&) = delete;
    PlayerProfileDataLocalAccount& operator=(PlayerProfileDataLocalAccount &&) = default;

    explicit PlayerProfileDataLocalAccount(ILogHandler* logger, base::IProfileStorage* localDataHandler);

    bool getIsDeviceConnected() override;
    void setIsDeviceConnected(bool isDeviceConnected) override;
    int getConnectedDeviceId() override;
    void setConnectedDeviceId(int connectedDeviceId) override;
    std::shared_ptr<std::string> getConnectedDeviceKey() override;
    void setConnectedDeviceKey(std::shared_ptr<std::string> connectedDeviceKey) override;

    ~PlayerProfileDataLocalAccount() override;
};
};
