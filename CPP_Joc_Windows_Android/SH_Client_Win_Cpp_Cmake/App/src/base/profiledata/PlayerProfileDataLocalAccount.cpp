#include "PlayerProfileDataLocalAccount.h"

using namespace base;

std::string PlayerProfileDataLocalAccount::IS_DEVICE_CONNECTED = "IS_DEVICE_CONNECTED";

std::string PlayerProfileDataLocalAccount::CONNECTED_DEVICE_ID = "CONNECTED_DEVICE_ID";
std::string PlayerProfileDataLocalAccount::CONNECTED_DEVICE_KEY = "CONNECTED_DEVICE_KEY";

PlayerProfileDataLocalAccount::~PlayerProfileDataLocalAccount() {
    //void
}

PlayerProfileDataLocalAccount::PlayerProfileDataLocalAccount(ILogHandler* logger, base::IProfileStorage* localDataHandler)
    : super(logger, localDataHandler)
{
    //void
}

bool PlayerProfileDataLocalAccount::getIsDeviceConnected() {
    return localDataHandler->getBoolean(IS_DEVICE_CONNECTED, false);
}

void PlayerProfileDataLocalAccount::setIsDeviceConnected(bool isDeviceConnected) {
    localDataHandler->putBoolean(IS_DEVICE_CONNECTED, isDeviceConnected);
}

int PlayerProfileDataLocalAccount::getConnectedDeviceId() {
    return localDataHandler->getInt(CONNECTED_DEVICE_ID, -1);
}

void PlayerProfileDataLocalAccount::setConnectedDeviceId(int connectedDeviceId) {
    localDataHandler->putInt(CONNECTED_DEVICE_ID, connectedDeviceId);
}

std::shared_ptr<std::string> PlayerProfileDataLocalAccount::getConnectedDeviceKey() {
    return localDataHandler->getSharedPtrString(CONNECTED_DEVICE_KEY);
}

void PlayerProfileDataLocalAccount::setConnectedDeviceKey(std::shared_ptr<std::string> connectedDeviceKey) {
    localDataHandler->putSharedPtrString(CONNECTED_DEVICE_KEY, connectedDeviceKey);
}
