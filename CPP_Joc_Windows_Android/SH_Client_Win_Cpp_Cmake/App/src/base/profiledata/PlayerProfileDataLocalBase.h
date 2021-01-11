#pragma once

#include <base/profiledata/PlayerProfileData.h>
#include "IPlayerProfileDataLocal.h"

namespace base {
class PlayerProfileDataLocalBase : public base::PlayerProfileData, virtual public IPlayerProfileDataLocal {
private: typedef base::PlayerProfileData super;
public:
    PlayerProfileDataLocalBase(PlayerProfileDataLocalBase const&) = delete;
    PlayerProfileDataLocalBase(PlayerProfileDataLocalBase &&) = default;
    PlayerProfileDataLocalBase& operator=(PlayerProfileDataLocalBase const&) = delete;
    PlayerProfileDataLocalBase& operator=(PlayerProfileDataLocalBase &&) = default;

    explicit PlayerProfileDataLocalBase(ILogHandler* logger, base::IProfileStorage* localDataHandler);

    virtual ~PlayerProfileDataLocalBase();
};
};
