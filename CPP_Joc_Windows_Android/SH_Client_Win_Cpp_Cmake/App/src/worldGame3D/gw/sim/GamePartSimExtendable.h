#pragma once

#include "GPSFrustumCulling3D.h"

namespace base {
class GamePartSimExtendable : public GPSFrustumCulling3D {
private: typedef GPSFrustumCulling3D super;
public:
    GamePartSimExtendable(GamePartSimExtendable const&) = delete;
    GamePartSimExtendable(GamePartSimExtendable &&) = default;
    GamePartSimExtendable& operator=(GamePartSimExtendable const&) = delete;
    GamePartSimExtendable& operator=(GamePartSimExtendable &&) = default;

    explicit GamePartSimExtendable(IGameWorld* gw);

    ~GamePartSimExtendable() override;
};
};
