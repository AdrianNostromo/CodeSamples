#pragma once

#include "GPManStarter.h"

namespace battlezones_a_v1 {
class GPManPlatforms : public GPManStarter {
private: typedef GPManStarter super;
public:
	static std::string GWE_RequestStartScenario_StartArea;

public:
    GPManPlatforms(GPManPlatforms const&) = delete;
    GPManPlatforms(GPManPlatforms &&) = default;
    GPManPlatforms& operator=(GPManPlatforms const&) = delete;
    GPManPlatforms& operator=(GPManPlatforms &&) = default;

    explicit GPManPlatforms(base::IGameWorld* gw);

    ~GPManPlatforms() override;
};
};
