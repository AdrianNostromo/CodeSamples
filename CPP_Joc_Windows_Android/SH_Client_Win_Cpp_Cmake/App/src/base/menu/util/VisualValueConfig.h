#pragma once

#include "UiId.h"
#include <base/object/WrappedValue.h>
#include <string>
#include <memory>
#include <cstddef>

class VisualValueConfig {
public:
    //asdAA;// move to a Operation class;
    static const int OPERATION_NONE;

    // Single baseValue operations.
    static const int OPERATION_ROUND;
    static const int OPERATION_FLOOR;
    static const int OPERATION_CEIL;

    // Dual baseValue operations.
    static const int OPERATION_ADD;
    static const int OPERATION_SUBTRACT;
    static const int OPERATION_MULTIPLY;
    static const int OPERATION_DIVIDE;
    static const int OPERATION_MIN;
    static const int OPERATION_MAX;

    // If the base value is null, the multiplier value is used as is (may be a different type, eg: Color).
    //asdAA;// this may be a MenuVisualAnimationProperty* in which case a special type_id is required ValueType::Direct/::Composed.
    std::shared_ptr<base::IWrappedValue> baseValue;
    UiId* baseMultiplierSourceID;
    std::string* baseMultiplierProperty;

    int operationType;

    VisualValueConfig* secondarySmartValue;

public:
    VisualValueConfig(VisualValueConfig const&) = delete;
    VisualValueConfig(VisualValueConfig &&) = default;
    VisualValueConfig& operator=(VisualValueConfig const&) = delete;
    VisualValueConfig& operator=(VisualValueConfig &&) = default;

    explicit VisualValueConfig(
        std::shared_ptr<base::IWrappedValue> baseValue, UiId* baseMultiplierComposedID, std::string* baseMultiplierProperty,
        int operationType,
        VisualValueConfig* secondarySmartValue);
    explicit VisualValueConfig(
        std::shared_ptr<base::IWrappedValue> baseValue, UiId* baseMultiplierComposedID, std::string* baseMultiplierProperty);
    explicit VisualValueConfig(
        std::shared_ptr<base::IWrappedValue> baseValue);
    explicit VisualValueConfig(
        float value, std::string valueMultiplierComposedIdProp);

    explicit VisualValueConfig(
        float value, std::string valueMultiplierComposedIdProp,
        int operationType,
        VisualValueConfig* secondarySmartValue);
    explicit VisualValueConfig(
        float valueA, std::string valueMultiplierComposedIdPropA,
        int operationType,
		float valueB, std::string valueMultiplierComposedIdPropB);
    explicit VisualValueConfig(
        float value,
        int operationType,
        VisualValueConfig* secondarySmartValue);
	
    explicit VisualValueConfig(
		float value, std::nullptr_t nullpErrorCheckParam,
		int operationType,
		VisualValueConfig* secondarySmartValue);

    virtual ~VisualValueConfig();
};
