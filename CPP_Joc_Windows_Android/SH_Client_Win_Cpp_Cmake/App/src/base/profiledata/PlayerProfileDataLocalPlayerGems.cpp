#include "PlayerProfileDataLocalPlayerGems.h"

using namespace base;

std::string PlayerProfileDataLocalPlayerGems::CURRENCY_PLAYER_GEMS = "CURRENCY_PLAYER_GEMS";

PlayerProfileDataLocalPlayerGems::~PlayerProfileDataLocalPlayerGems() {
    //void
}

PlayerProfileDataLocalPlayerGems::PlayerProfileDataLocalPlayerGems(ILogHandler* logger, base::IProfileStorage* localDataHandler)
    : super(logger, localDataHandler)
{
    //void
}

int PlayerProfileDataLocalPlayerGems::getPlayerGems() {
    return localDataHandler->getInt(CURRENCY_PLAYER_GEMS, 0);
}

void PlayerProfileDataLocalPlayerGems::putPlayerGems(int gems) {
    localDataHandler->putInt(CURRENCY_PLAYER_GEMS, gems);

    dispatchEvent_profileDataChanged(CURRENCY_PLAYER_GEMS);
}

void PlayerProfileDataLocalPlayerGems::addPlayerGems(int gemsAddValue) {
    putPlayerGems(getPlayerGems() + gemsAddValue);
}
