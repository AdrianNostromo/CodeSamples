#pragma once

#include "GPOServer.h"

namespace base {
class GPOnline : public GPOServer {
private: typedef GPOServer super;
public:
    GPOnline(GPOnline const&) = delete;
    GPOnline(GPOnline &&) = default;
    GPOnline& operator=(GPOnline const&) = delete;
    GPOnline& operator=(GPOnline &&) = default;

    explicit GPOnline(IGameWorld* gw);

    ~GPOnline() override;
};
};
