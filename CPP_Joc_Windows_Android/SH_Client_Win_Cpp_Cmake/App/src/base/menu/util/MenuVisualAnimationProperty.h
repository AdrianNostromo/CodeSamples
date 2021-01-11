#pragma once

#include <base/list/ArrayList.h>
#include <base/ease/util/EaseConfig.h>
#include "UiId.h"
#include "VisualValueConfig.h"
#include <graphics/visual/Color.h>

class MenuVisualAnimationProperty {
public:
    class StateComposedValue {
    public:
        // String or String[].
        // In case of String[], all entries must pass against the menu stateID for the baseValue to be used.
        // In case of String, if it contains "." it is split into String[].
        UiId* stateIDFragments;

        EaseConfig const* easeConfig;

        VisualValueConfig* smartValue;

        StateComposedValue(UiId* stateIDFragments, EaseConfig const* easeConfig, VisualValueConfig* smartValue);
    };
public:
    ArrayList<StateComposedValue*> stateValuesList{};

public:
    MenuVisualAnimationProperty(MenuVisualAnimationProperty const&) = delete;
    MenuVisualAnimationProperty(MenuVisualAnimationProperty &&) = default;
    MenuVisualAnimationProperty& operator=(MenuVisualAnimationProperty const&) = delete;
    MenuVisualAnimationProperty& operator=(MenuVisualAnimationProperty &&) = default;

    explicit MenuVisualAnimationProperty();

    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        std::shared_ptr<base::IWrappedValue> value,
        std::string valueMultiplierComposedIDProp,
        EaseConfig const* easeConfig);
    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        float value,
        std::string valueMultiplierComposedIDProp,
        EaseConfig const* easeConfig);
    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        std::shared_ptr<base::IWrappedValue> valueA,
        std::string valueMultiplierComposedIDPropA,
        int operationType,
        std::shared_ptr<base::IWrappedValue> valueB,
        std::string valueMultiplierComposedIDPropB,
        EaseConfig const* easeConfig);
    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        float valueA,
        std::string valueMultiplierComposedIDPropA,
        int operationType,
        float valueB,
        std::string valueMultiplierComposedIDPropB,
        EaseConfig const* easeConfig);
    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        std::shared_ptr<base::IWrappedValue> value,
        EaseConfig const* easeConfig);
    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        float value,
        EaseConfig const* easeConfig);
    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        bool value);
    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        Color value,
        EaseConfig const* easeConfig);
    MenuVisualAnimationProperty* putState(
        std::string stateIDFragments,
        VisualValueConfig* value,
        EaseConfig const* easeConfig);

    virtual ~MenuVisualAnimationProperty();

};
