#pragma once

#include <string>
#include <memory>
#include <base/profiledata/IPlayerProfileData.h>

namespace base {
class IPlayerProfileDataLocal : virtual public base::IPlayerProfileData {
public:
    virtual bool getIsDeviceConnected() = 0;
    virtual void setIsDeviceConnected(bool isDeviceConnected) = 0;
    virtual int getConnectedDeviceId() = 0;
    virtual void setConnectedDeviceId(int connectedDeviceId) = 0;
    virtual std::shared_ptr<std::string> getConnectedDeviceKey() = 0;
    virtual void setConnectedDeviceKey(std::shared_ptr<std::string> connectedDeviceKey) = 0;

    virtual int getPlayerGoldCoins() = 0;
    virtual void putPlayerGoldCoins(int goldCoins) = 0;
    virtual void addPlayerGoldCoins(int goldCoinsAddValue) = 0;

    virtual int getPlayerGems() = 0;
    virtual void putPlayerGems(int gems) = 0;
    virtual void addPlayerGems(int gemsAddValue) = 0;

    virtual ~IPlayerProfileDataLocal() = default;
};
};
