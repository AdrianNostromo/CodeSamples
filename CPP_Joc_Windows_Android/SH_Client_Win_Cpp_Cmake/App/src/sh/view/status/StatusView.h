#pragma once

#include "SVFlasher.h"

namespace sh {
class StatusView : public SVFlasher {
private: typedef SVFlasher super;
private:
    static ArrayList<MenuItemConfig*>* viewItemConfigs;

    static ArrayList<StateChangeDurations*>* viewEaseDurationsSList;

public:
    StatusView(StatusView const&) = delete;
    StatusView(StatusView &&) = default;
    StatusView& operator=(StatusView const&) = delete;
    StatusView& operator=(StatusView &&) = default;

    explicit StatusView(
        IAppSquareHeads* app);

    ~StatusView() override;
};
};
