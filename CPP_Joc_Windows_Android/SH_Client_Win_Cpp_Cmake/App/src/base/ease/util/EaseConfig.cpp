#include "EaseConfig.h"
#include "CubicBezierConfig.h"
#include <base/MM.h>

EaseConfig* EaseConfig::LINEAR = new EaseConfig(
	"linear",
	nullptr
);
EaseConfig* EaseConfig::ACCELERATION = new EaseConfig(
	"cubicBezier",
	new CubicBezierConfig(0.4f, 0.0f, 1.0f, 1.0f)
);
EaseConfig* EaseConfig::DECELERATION = new EaseConfig(
	"cubicBezier",
	new CubicBezierConfig(0.0f, 0.0f, 0.2f, 1.0f)
);
EaseConfig* EaseConfig::SHARP = new EaseConfig(
	"cubicBezier",
	new CubicBezierConfig(0.4f, 0.0f, 0.6f, 1.0f)
);

EaseConfig::EaseConfig(std::string easeFunctionID, IEaseConfig* easeFunctionConfig, float startPercentOverride, float endPercentOverride)
    : easeFunctionID(easeFunctionID), easeFunctionConfig(easeFunctionConfig), startPercentOverride(startPercentOverride), endPercentOverride(endPercentOverride)
{
    //void
}

EaseConfig::EaseConfig(std::string easeFunctionID, IEaseConfig* easeFunctionConfig)
    : EaseConfig(easeFunctionID, easeFunctionConfig, 0.0f, 1.0f)
{
    //void
}
