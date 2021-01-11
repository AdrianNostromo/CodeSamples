#pragma once

#include "ScreenListeners.h"

namespace base {
class Screen : public ScreenListeners {
private: typedef ScreenListeners super;
public:
    Screen(Screen const&) = delete;
    Screen(Screen &&) = default;
    Screen& operator=(Screen const&) = delete;
    Screen& operator=(Screen &&) = default;

    explicit Screen();

    ~Screen() override;
};
};
