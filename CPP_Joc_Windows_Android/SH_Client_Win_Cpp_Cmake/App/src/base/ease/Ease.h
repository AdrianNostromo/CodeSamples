#pragma once

#include <base/gh.h>
#include <string>
#include <base/ease/util/EaseConfig.h>
#include <memory>
#include <base/math/Vector3.h>

class Ease {
    pub static std::string linear;

    pub static std::string easeInSine;
    pub static std::string easeOutSine;

    pub static std::string easeInCubic;
    pub static std::string easeOutCubic;

    pub static std::string easeInQuint;
    pub static std::string easeOutQuint;

    pub static std::string easeOutElastic;

    pub static std::string easeInQuart;

    pub static std::string easeOutExpo;

    pub static float interpolate(
        float completionPercent,
        float startValue = 0.0f, float endValue = 1.0f,
        const std::string* easeFunctionName = nullptr, IEaseConfig* easeFunctionConfig = nullptr);
    
    pub static void interpolate(
        Vector3& out,
        float completionPercent,
        Vector3& startValue, Vector3& endValue,
        const std::string* easeFunctionName = nullptr, IEaseConfig* easeFunctionConfig = nullptr);

    pub static float computeEaseValue(
        const std::string& easeFunctionName,
        float completionPercent);
    pub static float computeEaseValue(
        const std::string& easeFunctionName,
        float completionPercent,
        float startValue, float endValue);
    pub static float computeEaseValue(
        float completionPercent,
        float starValue, float endValue,
        const EaseConfig& easeConfig);
    pub static float computeEaseValue(
        float completionPercent,
        const EaseConfig& easeConfig);
    pub static float computeEaseValue(
        const std::string& easeFunctionName,
        IEaseConfig* easeFunctionConfig,
        float completionPercent);
    pub static float computeEaseValue(
        const std::string& easeFunctionName,
        IEaseConfig* easeFunctionConfig,
        float completionPercent,
        float startValue, float endValue);

};
