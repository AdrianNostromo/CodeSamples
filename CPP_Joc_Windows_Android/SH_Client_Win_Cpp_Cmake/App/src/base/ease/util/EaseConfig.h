#pragma once

#include "IEaseConfig.h"
#include <string>

class EaseConfig : virtual public IEaseConfig {
public:
	static EaseConfig* LINEAR;
    static EaseConfig* ACCELERATION;
    static EaseConfig* DECELERATION;
    static EaseConfig* SHARP;

public:
    std::string easeFunctionID;
    IEaseConfig* easeFunctionConfig;

    float startPercentOverride;
    float endPercentOverride;

public:
    EaseConfig(EaseConfig const&) = delete;
    EaseConfig(EaseConfig &&) = default;
    EaseConfig& operator=(EaseConfig const&) = delete;
    EaseConfig& operator=(EaseConfig &&) = default;
    explicit EaseConfig(std::string easeFunctionID, IEaseConfig* easeFunctionConfig, float startPercentOverride, float endPercentOverride);
    explicit EaseConfig(std::string easeFunctionID, IEaseConfig* easeFunctionConfig);

};
