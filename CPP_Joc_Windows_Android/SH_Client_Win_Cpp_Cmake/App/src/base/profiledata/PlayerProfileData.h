#pragma once

#include "PlayerProfileDataBase.h"

namespace base {
class PlayerProfileData : public PlayerProfileDataBase {
private: typedef PlayerProfileDataBase super;
public:
    PlayerProfileData(PlayerProfileData const&) = delete;
    PlayerProfileData(PlayerProfileData &&) = default;
    PlayerProfileData& operator=(PlayerProfileData const&) = delete;
    PlayerProfileData& operator=(PlayerProfileData &&) = default;

    explicit PlayerProfileData(ILogHandler* logger, IProfileStorage* localDataHandler);

    virtual ~PlayerProfileData();
};
};
