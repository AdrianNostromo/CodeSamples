#include <base/ease/easefunctions/EaseFunctionQuad.h>
#include <base/ease/easefunctions/EaseFunctionCubic.h>
#include <base/ease/easefunctions/EaseFunctionQuart.h>
#include <base/ease/easefunctions/EaseFunctionQuint.h>
#include <base/ease/easefunctions/EaseFunctionExpo.h>
#include <base/ease/easefunctions/EaseFunctionCirc.h>
#include <base/ease/easefunctions/EaseFunctionBack.h>
#include <base/ease/easefunctions/EaseFunctionElastic.h>
#include <base/ease/easefunctions/EaseFunctionBounce.h>
#include "Ease.h"
#include "easefunctions/EaseFunctionLinear.h"
#include "easefunctions/EaseFunctionSine.h"
#include <base/exceptions/LogicException.h>
#include <base/ease/util/CubicBezierConfig.h>
#include <base/ease/easefunctions/EaseFunctionCubicBezier.h>

std::string Ease::linear = "linear";

std::string Ease::easeInSine = "easeInSine";
std::string Ease::easeOutSine = "easeOutSine";

std::string Ease::easeInCubic = "easeInCubic";
std::string Ease::easeOutCubic = "easeOutCubic";

std::string Ease::easeInQuint = "easeInQuint";
std::string Ease::easeOutQuint = "easeOutQuint";

std::string Ease::easeOutElastic = "easeOutElastic";

std::string Ease::easeInQuart = "easeInQuart";

std::string Ease::easeOutExpo = "easeOutExpo";

float Ease::interpolate(
    float completionPercent,
    float startValue, float endValue,
    const std::string* easeFunctionName, IEaseConfig* easeFunctionConfig)
{
    float easedCp;
    if (easeFunctionName != nullptr) {
        easedCp = computeEaseValue(
            *easeFunctionName,
            easeFunctionConfig,
            completionPercent
        );
    } else {
        easedCp = completionPercent;
    }

    return startValue + (endValue - startValue) * easedCp;
}

void Ease::interpolate(
    Vector3& out,
    float completionPercent,
    Vector3& startValue, Vector3& endValue,
    const std::string* easeFunctionName, IEaseConfig* easeFunctionConfig)
{
    float easedCp;
    if (easeFunctionName != nullptr) {
        easedCp = computeEaseValue(
            *easeFunctionName,
            easeFunctionConfig,
            completionPercent
        );
    } else {
        easedCp = completionPercent;
    }

    out.set(
        startValue.x + (endValue.x - startValue.x) * easedCp,
        startValue.y + (endValue.y - startValue.y) * easedCp,
        startValue.z + (endValue.z - startValue.z) * easedCp
    );
}

float Ease::computeEaseValue(
    const std::string& easeFunctionName,
    const float completionPercent)
{
    return computeEaseValue(easeFunctionName, nullptr, completionPercent);
}

float Ease::computeEaseValue(
    const std::string& easeFunctionName,
    const float completionPercent,
    const float startValue, float endValue)
{
    return computeEaseValue(easeFunctionName, nullptr, completionPercent, startValue, endValue);
}

float Ease::computeEaseValue(
    const float completionPercent,
    const float starValue, float endValue,
    const EaseConfig& easeConfig)
{
    float easedCp = computeEaseValue(
        easeConfig.easeFunctionID,
        easeConfig.easeFunctionConfig,
        completionPercent
    );

    return starValue + (endValue - starValue) * easedCp;
}

float Ease::computeEaseValue(
    float completionPercent,
    const EaseConfig& easeConfig)
{
    if(completionPercent < easeConfig.startPercentOverride) {
        completionPercent = 0.0f;
    }else if(completionPercent > easeConfig.endPercentOverride) {
        completionPercent = 1.0f;
    }else if(easeConfig.startPercentOverride > 0.0f || easeConfig.endPercentOverride < 1.0f) {
        float animationClipAmount = easeConfig.startPercentOverride + (1.0f - easeConfig.endPercentOverride);

        float animationRegionSize = 1.0f - animationClipAmount;

        completionPercent = (completionPercent - easeConfig.startPercentOverride) / animationRegionSize;
    }

    return computeEaseValue(
        easeConfig.easeFunctionID,
        easeConfig.easeFunctionConfig,
        completionPercent
    );
}

float Ease::computeEaseValue(
    const std::string& easeFunctionName,
    IEaseConfig* easeFunctionConfig,
    const float completionPercent)
{
    float animatedValue;

    if(easeFunctionName == "cubicBezier") {
        CubicBezierConfig* bezierConfig = dynamic_cast<CubicBezierConfig*>(easeFunctionConfig);
        animatedValue = EaseFunctionCubicBezier::easeCubicBezier(
                bezierConfig->startX, bezierConfig->startY, bezierConfig->endX, bezierConfig->endY,
                completionPercent
        );
    }else if(easeFunctionName == "linear") {
        animatedValue = EaseFunctionLinear::easeNone(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInSine") {
        animatedValue = EaseFunctionSine::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutSine") {
        animatedValue = EaseFunctionSine::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutSine") {
        animatedValue = EaseFunctionSine::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInQuad") {
        animatedValue = EaseFunctionQuad::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutQuad") {
        animatedValue = EaseFunctionQuad::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutQuad") {
        animatedValue = EaseFunctionQuad::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInCubic") {
        animatedValue = EaseFunctionCubic::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutCubic") {
        animatedValue = EaseFunctionCubic::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutCubic") {
        animatedValue = EaseFunctionCubic::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInQuart") {
        animatedValue = EaseFunctionQuart::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutQuart") {
        animatedValue = EaseFunctionQuart::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutQuart") {
        animatedValue = EaseFunctionQuart::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInExpo") {
        animatedValue = EaseFunctionExpo::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutExpo") {
        animatedValue = EaseFunctionExpo::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutExpo") {
        animatedValue = EaseFunctionExpo::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInQuint") {
        animatedValue = EaseFunctionQuint::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutQuint") {
        animatedValue = EaseFunctionQuint::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutQuint") {
        animatedValue = EaseFunctionQuint::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInExpo") {
        animatedValue = EaseFunctionExpo::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutExpo") {
        animatedValue = EaseFunctionExpo::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutExpo") {
        animatedValue = EaseFunctionExpo::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInCirc") {
        animatedValue = EaseFunctionCirc::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutCirc") {
        animatedValue = EaseFunctionCirc::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutCirc") {
        animatedValue = EaseFunctionCirc::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInBack") {
        animatedValue = EaseFunctionBack::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutBack") {
        animatedValue = EaseFunctionBack::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutBack") {
        animatedValue = EaseFunctionBack::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInElastic") {
        animatedValue = EaseFunctionElastic::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutElastic") {
        animatedValue = EaseFunctionElastic::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutElastic") {
        animatedValue = EaseFunctionElastic::easeInOut(completionPercent, 0, 1, 1);
    }

    else if(easeFunctionName == "easeInBounce") {
        animatedValue = EaseFunctionBounce::easeIn(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeOutBounce") {
        animatedValue = EaseFunctionBounce::easeOut(completionPercent, 0, 1, 1);
    }else if(easeFunctionName == "easeInOutBounce") {
        animatedValue = EaseFunctionBounce::easeInOut(completionPercent, 0, 1, 1);
    }

    else {
        // "Unhandled ease function: " + easeFunctionName;
        throw LogicException(LOC);
    }

    return animatedValue;
}

float Ease::computeEaseValue(
    const std::string& easeFunctionName,
    IEaseConfig* easeFunctionConfig,
    const float completionPercent,
    const float startValue, float endValue)
{
    float n = computeEaseValue(easeFunctionName, easeFunctionConfig, completionPercent);

    float val = startValue + (endValue - startValue) * n;

    return val;
}
