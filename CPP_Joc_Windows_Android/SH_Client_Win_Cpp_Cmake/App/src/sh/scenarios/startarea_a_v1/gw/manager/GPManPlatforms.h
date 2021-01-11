#pragma once

#include "GPManBase.h"

namespace startarea_a_v1 {
class GPManPlatforms : public GPManBase {
private: typedef GPManBase super;
public:
    GPManPlatforms(GPManPlatforms const&) = delete;
    GPManPlatforms(GPManPlatforms &&) = default;
    GPManPlatforms& operator=(GPManPlatforms const&) = delete;
    GPManPlatforms& operator=(GPManPlatforms &&) = default;

    explicit GPManPlatforms(base::IGameWorld* gw);

    ~GPManPlatforms() override;
private:
	void onEvent(IEventListener& eventListener, base::GWEvent& event);

};
};
