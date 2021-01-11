#pragma once

#include "APreRendering.h"

namespace base {
class Animator : public APreRendering {
private: typedef APreRendering super;
public:
    Animator(Animator const&) = delete;
    Animator(Animator &&) = default;
    Animator& operator=(Animator const&) = delete;
    Animator& operator=(Animator &&) = default;

    explicit Animator();

    ~Animator() override;
};
};
