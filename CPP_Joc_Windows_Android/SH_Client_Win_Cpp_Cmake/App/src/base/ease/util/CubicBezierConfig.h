#pragma once

#include "IEaseConfig.h"

class CubicBezierConfig : virtual public IEaseConfig {
public:
    float startX;
    float startY;

    float endX;
    float endY;

public:
    CubicBezierConfig(CubicBezierConfig const&) = delete;
    CubicBezierConfig(CubicBezierConfig &&) = default;
    CubicBezierConfig& operator=(CubicBezierConfig const&) = delete;
    CubicBezierConfig& operator=(CubicBezierConfig &&) = default;
    explicit CubicBezierConfig(float startX, float startY, float endX, float endY);

};
