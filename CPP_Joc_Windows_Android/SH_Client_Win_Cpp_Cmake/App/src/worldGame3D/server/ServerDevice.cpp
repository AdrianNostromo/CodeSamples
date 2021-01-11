#include <base/app/config/AppConfig.h>
#include "ServerDevice.h"
#include "util/ConnectedDeviceInformation.h"
#include <base/exceptions/LogicException.h>

using namespace worldGame3D;

const bool ServerDevice::IS_DEBUG_ACCOUNT = true;

ServerDevice::ServerDevice(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app)
    : super(logger, errorHandler, profileData, app)
{
    //void
}

void ServerDevice::createPost() {
    super::createPost();

    if(profileData->getIsDeviceConnected()) {
        int deviceId = profileData->getConnectedDeviceId();
        std::shared_ptr<std::string> deviceKey = profileData->getConnectedDeviceKey();
        if (deviceId == -1 || deviceKey == nullptr) {
            throw LogicException(LOC);
        }

        deviceInformation = new ConnectedDeviceInformation(
            deviceId,
            deviceKey
        );

        if(AppConfig::IS_DEBUG && IS_DEBUG_ACCOUNT) {
            logger->postDebug(std::string("Account. Local data device id: ") + std::to_string(deviceInformation->deviceId));
        }
    }
}

bool ServerDevice::isDeviceConnected() {
    if(deviceInformation != nullptr) {
        return true;
    }

    return false;
}

ConnectedDeviceInformation* ServerDevice::getDeviceInformation() {
    return deviceInformation;
}

void ServerDevice::deviceConnectOccured(int deviceId, std::shared_ptr<std::string> deviceKey) {
    if(isDeviceConnected()) {
        throw LogicException(LOC);
    }

    deviceInformation = new ConnectedDeviceInformation(
        deviceId,
        deviceKey
    );
    profileData->setConnectedDeviceId(deviceInformation->deviceId);
    profileData->setConnectedDeviceKey(deviceInformation->deviceKey);
    profileData->setIsDeviceConnected(true);

    profileData->sensitiveDataChanged();

    if(AppConfig::IS_DEBUG && IS_DEBUG_ACCOUNT) {
        logger->postDebug(std::string("Account. New device connected id: ") + std::to_string(deviceInformation->deviceId));
    }
}

void ServerDevice::onAccountInteractionFailed() {
    if(deviceInformation != nullptr) {
        clearDeviceInformation();
    }
}

void ServerDevice::userDisconnectOccuredDetected() {
    if(!isDeviceConnected()) {
        throw LogicException(LOC);
    }

    clearDeviceInformation();

    if(AppConfig::IS_DEBUG && IS_DEBUG_ACCOUNT) {
        logger->postDebug("Account. Device disconnected.");
    }
}

void ServerDevice::clearDeviceInformation() {
    if(deviceInformation != nullptr) {
        deviceInformation = nullptr;

        profileData->setConnectedDeviceId(-1);
        profileData->setConnectedDeviceKey(nullptr);
        profileData->setIsDeviceConnected(false);

        // This is used to overwrite the backup local file.
        profileData->sensitiveDataChanged();
    }
}

ServerDevice::~ServerDevice() {
    //void
}
