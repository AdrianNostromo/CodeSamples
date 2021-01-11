#include "PlayerProfileDataLocalPlayerGoldCoins.h"

using namespace base;

std::string PlayerProfileDataLocalPlayerGoldCoins::CURRENCY_PLAYER_GOLD_COINS = "CURRENCY_PLAYER_GOLD_COINS";

PlayerProfileDataLocalPlayerGoldCoins::~PlayerProfileDataLocalPlayerGoldCoins() {
    //void
}

PlayerProfileDataLocalPlayerGoldCoins::PlayerProfileDataLocalPlayerGoldCoins(ILogHandler* logger, base::IProfileStorage* localDataHandler)
    : super(logger, localDataHandler)
{
    //void
}

int PlayerProfileDataLocalPlayerGoldCoins::getPlayerGoldCoins() {
    return localDataHandler->getInt(CURRENCY_PLAYER_GOLD_COINS, 0);
}

void PlayerProfileDataLocalPlayerGoldCoins::putPlayerGoldCoins(int goldCoins) {
    localDataHandler->putInt(CURRENCY_PLAYER_GOLD_COINS, goldCoins);

    dispatchEvent_profileDataChanged(CURRENCY_PLAYER_GOLD_COINS);
}

void PlayerProfileDataLocalPlayerGoldCoins::addPlayerGoldCoins(int goldCoinsAddValue) {
    putPlayerGoldCoins(getPlayerGoldCoins() + goldCoinsAddValue);
}
