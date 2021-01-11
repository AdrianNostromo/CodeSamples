#pragma once

#include "ComponentIpPortActiveRequests.h"

namespace worldGame3D {
class ComponentIpPort : public ComponentIpPortActiveRequests {
private: typedef ComponentIpPortActiveRequests super;
public:
    ComponentIpPort(ComponentIpPort const&) = delete;
    ComponentIpPort(ComponentIpPort &&) = default;
    ComponentIpPort& operator=(ComponentIpPort const&) = delete;
    ComponentIpPort& operator=(ComponentIpPort &&) = default;

    explicit ComponentIpPort(IServerPartHandler* handler, IApp* app);

    ~ComponentIpPort() override;
};
};
