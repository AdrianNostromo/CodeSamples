#pragma once

#include "PlayerProfileDataLocalPlayerGoldCoins.h"

namespace base {
class PlayerProfileDataLocalPlayerGems : public PlayerProfileDataLocalPlayerGoldCoins {
private: typedef PlayerProfileDataLocalPlayerGoldCoins super;
private:
    static std::string CURRENCY_PLAYER_GEMS;

public:
    PlayerProfileDataLocalPlayerGems(PlayerProfileDataLocalPlayerGems const&) = delete;
    PlayerProfileDataLocalPlayerGems(PlayerProfileDataLocalPlayerGems &&) = default;
    PlayerProfileDataLocalPlayerGems& operator=(PlayerProfileDataLocalPlayerGems const&) = delete;
    PlayerProfileDataLocalPlayerGems& operator=(PlayerProfileDataLocalPlayerGems &&) = default;

    explicit PlayerProfileDataLocalPlayerGems(ILogHandler* logger, base::IProfileStorage* localDataHandler);

    int getPlayerGems() override;
    void putPlayerGems(int gems) override;
    void addPlayerGems(int gemsAddValue) override;

    ~PlayerProfileDataLocalPlayerGems() override;
};
};
