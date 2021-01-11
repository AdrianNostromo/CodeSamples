#pragma once

#include <memory>
#include <string>
#include <worldGame3D/gw/online/server/util/ServerConnectConfig.h>

class ScenarioConfig {
public:
    std::shared_ptr<base::ServerConnectConfig> serverConnectConfig;

public:
	ScenarioConfig(
		std::shared_ptr<base::ServerConnectConfig> serverConnectConfig);

	~ScenarioConfig();
};
