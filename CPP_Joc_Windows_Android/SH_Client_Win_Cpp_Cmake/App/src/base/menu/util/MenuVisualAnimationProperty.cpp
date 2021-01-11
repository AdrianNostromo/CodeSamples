#include "MenuVisualAnimationProperty.h"
#include <base/exceptions/LogicException.h>
#include <base/MM.h>

MenuVisualAnimationProperty::StateComposedValue::StateComposedValue(UiId* stateIDFragments, EaseConfig const* easeConfig, VisualValueConfig* smartValue)
    : stateIDFragments(stateIDFragments), easeConfig(easeConfig), smartValue(smartValue)
{
    //void
}

MenuVisualAnimationProperty::MenuVisualAnimationProperty() {
    //void
}

MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    std::shared_ptr<base::IWrappedValue> value,
    std::string valueMultiplierComposedIDProp,
    EaseConfig const* easeConfig)
{
    stateValuesList.appendDirect(new StateComposedValue(
        UiId::newIdFromComposedId(stateIDFragments), easeConfig,
        new VisualValueConfig(
            value,
            UiId::newIdFromComposedIdProp(valueMultiplierComposedIDProp, "_local"),
            UiId::newPropStringFromComposedIdProp(valueMultiplierComposedIDProp)
        )
    ));

    return this;
}

MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    float value,
    std::string valueMultiplierComposedIDProp,
    EaseConfig const* easeConfig)
{
    return putState(
        stateIDFragments,
        base::IWrappedValue::new_float(value),
        valueMultiplierComposedIDProp,
        easeConfig);
}

//asdZ;// Implement a variant that has params for stateIDFragments_start and stateIDFragments_end. This is usefull for animations control.
MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    std::shared_ptr<base::IWrappedValue> valueA,
    std::string valueMultiplierComposedIDPropA,
    int operationType,
    std::shared_ptr<base::IWrappedValue> valueB,
    std::string valueMultiplierComposedIDPropB,
    EaseConfig const* easeConfig)
{
    stateValuesList.appendDirect(new StateComposedValue(
        UiId::newIdFromComposedId(stateIDFragments), easeConfig,
        new VisualValueConfig(
            valueA,
            UiId::newIdFromComposedIdProp(valueMultiplierComposedIDPropA, "_local"),
            UiId::newPropStringFromComposedIdProp(valueMultiplierComposedIDPropA),

            operationType,

            new VisualValueConfig(
                valueB,
                UiId::newIdFromComposedIdProp(valueMultiplierComposedIDPropB, "_local"),
                UiId::newPropStringFromComposedIdProp(valueMultiplierComposedIDPropB)
            )
        )
    ));

    return this;
}

MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    float valueA, std::string valueMultiplierComposedIDPropA,
    int operationType,
    float valueB, std::string valueMultiplierComposedIDPropB,
    EaseConfig const* easeConfig)
{
    return putState(
        stateIDFragments,

        base::IWrappedValue::new_float(valueA),
        valueMultiplierComposedIDPropA,

        operationType,

        base::IWrappedValue::new_float(valueB),
        valueMultiplierComposedIDPropB,

        easeConfig
    );
}

MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    std::shared_ptr<base::IWrappedValue> value,
    EaseConfig const* easeConfig)
{
    stateValuesList.appendDirect(new StateComposedValue(
        UiId::newIdFromComposedId(stateIDFragments), easeConfig,
        new VisualValueConfig(
            value
        )
    ));

    return this;
}

MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    float value,
    EaseConfig const* easeConfig)
{
    if(easeConfig == nullptr) {
        // This should not occur. Either called with nullptr param or ease util was not initialised problem still exists.
        throw LogicException(LOC);
    }
    return putState(
        stateIDFragments,
        base::IWrappedValue::new_float(value),
        easeConfig);
}

MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    bool value)
{
    return putState(
        stateIDFragments,
        base::IWrappedValue::new_bool(value),
        nullptr);
}

MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    Color value,
    EaseConfig const* easeConfig)
{
    return putState(
        stateIDFragments,
        base::IWrappedValue::new_Color(value),
        easeConfig);
}

MenuVisualAnimationProperty* MenuVisualAnimationProperty::putState(
    std::string stateIDFragments,
    VisualValueConfig* value,
    EaseConfig const* easeConfig)
{
    stateValuesList.appendDirect(new StateComposedValue(
        UiId::newIdFromComposedId(stateIDFragments), easeConfig,
        value
    ));

    return this;
}

MenuVisualAnimationProperty::~MenuVisualAnimationProperty() {
    //void
}

/*asdAA;// implement as needed
public MenuVisualAnimationProperty putState(Object stateIDFragments, Object value, String smartMultiplier, EaseConfig easeConfig) {
    stateValuesList.add(new StateComposedValue(stateIDFragments, easeConfig, new VisualValueConfig(value, smartMultiplier)));

    return this;
}

public MenuVisualAnimationProperty putState(Object stateIDFragments, Object value, EaseConfig easeConfig) {
    return putState(stateIDFragments, value, null, null, easeConfig);
}

public MenuVisualAnimationProperty putState(Object stateIDFragments, VisualValueConfig smartValue, EaseConfig easeConfig) {
    stateValuesList.add(new StateComposedValue(stateIDFragments, easeConfig, smartValue));

    return this;
}

public MenuVisualAnimationProperty putState(Object stateIDFragments,
                                            Object baseValue, String[] baseMultiplierSourceID, String baseMultiplierProperty,
                                            int operationType,
                                            Object secondaryValue, String[] secondaryMultiplierSourceID, String secondaryMultiplierProperty,
                                            EaseConfig easeConfig
) {
    stateValuesList.add(new StateComposedValue(stateIDFragments, easeConfig, new VisualValueConfig(
            baseValue, baseMultiplierSourceID, baseMultiplierProperty,
            operationType,
            secondaryValue, secondaryMultiplierSourceID, secondaryMultiplierProperty
    )));

    return this;
}

public MenuVisualAnimationProperty putState(Object stateIDFragments,
                                            Object baseValue, String baseSmartMultiplier,
                                            int operationType,
                                            Object secondaryValue, String secondarySmartMultiplier,
                                            EaseConfig easeConfig
) {
    stateValuesList.add(new StateComposedValue(stateIDFragments, easeConfig, new VisualValueConfig(
            baseValue, baseSmartMultiplier,
            operationType,
            secondaryValue, secondarySmartMultiplier
    )));

    return this;
}*/
