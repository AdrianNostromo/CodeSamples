#pragma once

#include <string>
#include <memory>

namespace worldGame3D {
class ConnectedDeviceInformation {
public:
    int deviceId;
    std::shared_ptr<std::string> deviceKey;

public:
    ConnectedDeviceInformation(ConnectedDeviceInformation const&) = delete;
    ConnectedDeviceInformation(ConnectedDeviceInformation &&) = default;
    ConnectedDeviceInformation& operator=(ConnectedDeviceInformation const&) = delete;
    ConnectedDeviceInformation& operator=(ConnectedDeviceInformation &&) = default;

    explicit ConnectedDeviceInformation(int deviceId, std::shared_ptr<std::string> deviceKey);

    virtual ~ConnectedDeviceInformation();
};
};
