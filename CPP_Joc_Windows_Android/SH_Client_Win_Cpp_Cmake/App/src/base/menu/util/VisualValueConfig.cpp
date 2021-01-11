#include "VisualValueConfig.h"
#include <cassert>
#include <base/MM.h>

const int VisualValueConfig::OPERATION_NONE = 0;

// Single baseValue operations.
const int VisualValueConfig::OPERATION_ROUND = 1;
const int VisualValueConfig::OPERATION_FLOOR = 2;
const int VisualValueConfig::OPERATION_CEIL = 3;

// Dual baseValue operations.
const int VisualValueConfig::OPERATION_ADD = 4;
const int VisualValueConfig::OPERATION_SUBTRACT = 5;
const int VisualValueConfig::OPERATION_MULTIPLY = 6;
const int VisualValueConfig::OPERATION_DIVIDE = 7;
const int VisualValueConfig::OPERATION_MIN = 8;
const int VisualValueConfig::OPERATION_MAX = 9;

VisualValueConfig::VisualValueConfig(
    std::shared_ptr<base::IWrappedValue> baseValue, UiId* baseMultiplierSourceID, std::string* baseMultiplierProperty,
    int operationType,
    VisualValueConfig* secondarySmartValue)
    : baseValue(baseValue), baseMultiplierSourceID(baseMultiplierSourceID), baseMultiplierProperty(baseMultiplierProperty),
    operationType(operationType),
    secondarySmartValue(secondarySmartValue)
{
    //void
}

VisualValueConfig::VisualValueConfig(std::shared_ptr<base::IWrappedValue> baseValue, UiId* baseMultiplierComposedID, std::string* baseMultiplierProperty)
    : VisualValueConfig(
        baseValue,
        baseMultiplierComposedID, baseMultiplierProperty,
        OPERATION_NONE,
        nullptr
    )
{
    //void
}

VisualValueConfig::VisualValueConfig(std::shared_ptr<base::IWrappedValue> baseValue)
    : VisualValueConfig(baseValue, nullptr, nullptr)
{
    //void
}

VisualValueConfig::VisualValueConfig(float value, std::string valueMultiplierComposedIdProp)
    : VisualValueConfig(
        base::IWrappedValue::new_float(value),
        UiId::newIdFromComposedIdProp(valueMultiplierComposedIdProp, "_local"),
        UiId::newPropStringFromComposedIdProp(valueMultiplierComposedIdProp)
    )
{
    //void
}

VisualValueConfig::VisualValueConfig(
    float value, std::string valueMultiplierComposedIdProp,
    int operationType,
    VisualValueConfig* secondarySmartValue)
    : VisualValueConfig(
        base::IWrappedValue::new_float(value),
        UiId::newIdFromComposedIdProp(valueMultiplierComposedIdProp, "_local"),
        UiId::newPropStringFromComposedIdProp(valueMultiplierComposedIdProp),
        operationType,
        secondarySmartValue
    )
{
    //void
}

VisualValueConfig::VisualValueConfig(
	float valueA, std::string valueMultiplierComposedIdPropA,
	int operationType,
	float valueB, std::string valueMultiplierComposedIdPropB)
    : VisualValueConfig(
        base::IWrappedValue::new_float(valueA),
        UiId::newIdFromComposedIdProp(valueMultiplierComposedIdPropA, "_local"),
        UiId::newPropStringFromComposedIdProp(valueMultiplierComposedIdPropA),
        operationType,
        new VisualValueConfig(valueB, valueMultiplierComposedIdPropB)
    )
{
    //void
}

VisualValueConfig::VisualValueConfig(
    float value,
    int operationType,
    VisualValueConfig* secondarySmartValue)
    : VisualValueConfig(
        base::IWrappedValue::new_float(value),
        nullptr,
		nullptr,
        operationType,
        secondarySmartValue
    )
{
    //void
}

VisualValueConfig::VisualValueConfig(
    float value, std::nullptr_t nullpErrorCheckParam,
    int operationType,
    VisualValueConfig* secondarySmartValue)
{
	// This is used for error detection.
	// If nullpErrorCheckParam is nullptr, the above constructor with valueMultiplierComposedIdProp::std::string would be used and that causes a hard to detect runtime error.
	
	// If this constructor occurs, remove the nullptr from the caller so it uses the version without a nullpErrorCheckParam parameter.

	assert(false);
}

VisualValueConfig::~VisualValueConfig() {
    //void
}
