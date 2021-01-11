#pragma once

#include "PlayerProfileDataLocalBase.h"

namespace base {
class PlayerProfileDataLocalPlayerGoldCoins : public PlayerProfileDataLocalBase {
private: typedef PlayerProfileDataLocalBase super;
private:
    static std::string CURRENCY_PLAYER_GOLD_COINS;

public:
    PlayerProfileDataLocalPlayerGoldCoins(PlayerProfileDataLocalPlayerGoldCoins const&) = delete;
    PlayerProfileDataLocalPlayerGoldCoins(PlayerProfileDataLocalPlayerGoldCoins &&) = default;
    PlayerProfileDataLocalPlayerGoldCoins& operator=(PlayerProfileDataLocalPlayerGoldCoins const&) = delete;
    PlayerProfileDataLocalPlayerGoldCoins& operator=(PlayerProfileDataLocalPlayerGoldCoins &&) = default;

    explicit PlayerProfileDataLocalPlayerGoldCoins(ILogHandler* logger, base::IProfileStorage* localDataHandler);

    int getPlayerGoldCoins() override;
    void putPlayerGoldCoins(int goldCoins) override;
    void addPlayerGoldCoins(int goldCoinsAddValue) override;

    ~PlayerProfileDataLocalPlayerGoldCoins() override;
};
};
