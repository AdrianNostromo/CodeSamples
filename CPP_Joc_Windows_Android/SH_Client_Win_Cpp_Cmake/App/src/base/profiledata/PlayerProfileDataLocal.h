#pragma once

#include "PlayerProfileDataLocalAccount.h"

namespace base {
class PlayerProfileDataLocal : public PlayerProfileDataLocalAccount {
private: typedef PlayerProfileDataLocalAccount super;
public:
    PlayerProfileDataLocal(PlayerProfileDataLocal const&) = delete;
    PlayerProfileDataLocal(PlayerProfileDataLocal &&) = default;
    PlayerProfileDataLocal& operator=(PlayerProfileDataLocal const&) = delete;
    PlayerProfileDataLocal& operator=(PlayerProfileDataLocal &&) = default;

    explicit PlayerProfileDataLocal(ILogHandler* logger, base::IProfileStorage* localDataHandler);

    virtual ~PlayerProfileDataLocal();
};
};
