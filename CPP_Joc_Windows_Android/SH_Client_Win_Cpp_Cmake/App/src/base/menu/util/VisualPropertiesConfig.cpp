#include "VisualPropertiesConfig.h"
#include <base/MM.h>

VisualPropertiesConfig::VisualPropertiesConfig() {
    //void
}

VisualPropertiesConfig* VisualPropertiesConfig::put(VisualPropertyValueConfig* composedValue) {
    propertyConfigs.appendDirect(composedValue);

    return this;
}

VisualPropertiesConfig* VisualPropertiesConfig::put(
	std::string property, VisualValueConfig* smartValue)
{
    return put(new VisualPropertyValueConfig(
        property,
		smartValue
    ));
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, std::nullptr_t value, std::string valueMultiplierComposedIdProp) {
    return put(new VisualPropertyValueConfig(
        property,
        new VisualValueConfig(
            value, UiId::newIdFromComposedIdProp(valueMultiplierComposedIdProp, "_local"), UiId::newPropStringFromComposedIdProp(valueMultiplierComposedIdProp)
        )
    ));
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, std::shared_ptr<base::IWrappedValue> value, std::string valueMultiplierComposedIdProp) {
    return put(new VisualPropertyValueConfig(
        property,
        new VisualValueConfig(
            value, UiId::newIdFromComposedIdProp(valueMultiplierComposedIdProp, "_local"), UiId::newPropStringFromComposedIdProp(valueMultiplierComposedIdProp)
        )
    ));
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, int value, std::string valueMultiplierComposedIdProp) {
    return put(
        property,
        base::IWrappedValue::new_int(value),
        valueMultiplierComposedIdProp
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, std::shared_ptr<float> value, std::string valueMultiplierComposedIdProp) {
    return put(
        property,
        base::IWrappedValue::new_float_sharedPtr(value),
        valueMultiplierComposedIdProp
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, float value, std::string valueMultiplierComposedIdProp) {
    return put(
        property,
        base::IWrappedValue::new_float(value),
        valueMultiplierComposedIdProp
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, std::shared_ptr<base::IWrappedValue> value) {
    return put(new VisualPropertyValueConfig(
        property,
        new VisualValueConfig(
            value
        )
    ));
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, int value) {
    return put(
        property,
        base::IWrappedValue::new_int(value)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, float value) {
    return put(
        property,
        base::IWrappedValue::new_float(value)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, bool value) {
    return put(
        property,
        base::IWrappedValue::new_bool(value)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, std::string value) {
    return put(
        property,
        base::IWrappedValue::new_String(value)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, const char* value) {
    std::string s = value;
    return put(
        property,
        base::IWrappedValue::new_String(s)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, Color value) {
    return put(
        property,
        base::IWrappedValue::new_Color(value)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, Color* value) {
    return put(
        property,
        base::IWrappedValue::new_Color(*value)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, std::string* value) {
    return put(
        property,
        base::IWrappedValue::new_String_ptr(value)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(std::string property, std::nullptr_t value) {
    return put(
        property,
        base::IWrappedValue::new_String_ptr(value)
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(
    std::string property,
    std::shared_ptr<base::IWrappedValue> valueA, std::string valueAMultiplierComposedIdProp,
    int operationType,
    std::shared_ptr<base::IWrappedValue> valueB, std::string valueBMultiplierComposedIdProp)
{
    return put(new VisualPropertyValueConfig(
        property,
        new VisualValueConfig(
            valueA, UiId::newIdFromComposedIdProp(valueAMultiplierComposedIdProp, "_local"), UiId::newPropStringFromComposedIdProp(valueAMultiplierComposedIdProp),
            operationType,
            new VisualValueConfig(
                valueB, UiId::newIdFromComposedIdProp(valueBMultiplierComposedIdProp, "_local"), UiId::newPropStringFromComposedIdProp(valueBMultiplierComposedIdProp)
            )
        )
    ));
}

VisualPropertiesConfig* VisualPropertiesConfig::put(
    std::string property,
    float valueA, std::string valueAMultiplierComposedIdProp,
    int operationType,
    float valueB, std::string valueBMultiplierComposedIdProp)
{
    return put(
        property,

        base::IWrappedValue::new_float(valueA),
        valueAMultiplierComposedIdProp,

        operationType,

        base::IWrappedValue::new_float(valueB),
        valueBMultiplierComposedIdProp
    );
}

VisualPropertiesConfig* VisualPropertiesConfig::put(
    std::string property,
    float valueA, std::string valueAMultiplierComposedIdProp,
    int operationType,
    float valueB, std::nullptr_t valueBMultiplierComposedIdProp)
{
    return put(new VisualPropertyValueConfig(
        property,
        new VisualValueConfig(
			base::IWrappedValue::new_float(valueA), UiId::newIdFromComposedIdProp(valueAMultiplierComposedIdProp, "_local"), UiId::newPropStringFromComposedIdProp(valueAMultiplierComposedIdProp),
            operationType,
            new VisualValueConfig(
				base::IWrappedValue::new_float(valueB)
            )
        )
    ));
}

VisualPropertiesConfig* VisualPropertiesConfig::put(
	std::string property,
	float valueA, std::string valueAMultiplierComposedIdProp,
	int operationType,
	VisualValueConfig* valueB)
{
    return put(new VisualPropertyValueConfig(
        property,
        new VisualValueConfig(
			base::IWrappedValue::new_float(valueA), UiId::newIdFromComposedIdProp(valueAMultiplierComposedIdProp, "_local"), UiId::newPropStringFromComposedIdProp(valueAMultiplierComposedIdProp),
            operationType,
			valueB
        )
    ));
}

VisualPropertiesConfig* VisualPropertiesConfig::putAnimating(std::string property, MenuVisualAnimationProperty* value) {
    return put(new VisualPropertyValueConfig(
        property,
        new VisualValueConfig(
            base::IWrappedValue::new_MenuVisualAnimationProperty_ptr(value)
        )
    ));
}

VisualPropertiesConfig::~VisualPropertiesConfig() {
    //void
}

/*asdAA;// Implement only hte ones that are needed.
public VisualPropertiesConfig put(String property,
                                  Object valueA, String[] multiplierSourceIDA, String multiplierPropertyA,
                                  int operationType,
                                  Object valueB, String[] multiplierSourceIDB, String multiplierPropertyB)
{
    return put(new VisualPropertyValueConfig(property,
            new VisualValueConfig(
                    valueA, multiplierSourceIDA, multiplierPropertyA,
                    operationType,
                    new VisualValueConfig(valueB, multiplierSourceIDB, multiplierPropertyB)
            )
    ));
}

public VisualPropertiesConfig put(String property,
                                  Object valueA, String smartMultiplierA,
                                  int operationType,
                                  Object valueB, String smartMultiplierB)
{
    return put(new VisualPropertyValueConfig(property,
            new VisualValueConfig(
                    valueA, smartMultiplierA,
                    operationType,
                    new VisualValueConfig(valueB, smartMultiplierB)
            )
    ));
}

public VisualPropertiesConfig put(String property,
                                  Object valueA, String smartMultiplierA,
                                  int operationType,
                                                                                                                                                                                                                                                                                                                                                  VisualValueConfig valueB)
{
    return put(new VisualPropertyValueConfig(property,
            new VisualValueConfig(
                    valueA, smartMultiplierA,
                    operationType,
                    valueB
            )
    ));
}

public VisualPropertiesConfig putManualValue(String property, VisualValueConfig smartValue) {
    return put(new VisualPropertyValueConfig(property, smartValue));
}

public VisualPropertiesConfig put(String property, Object value) {
    return put(new VisualPropertyValueConfig(property, new VisualValueConfig(value, null, null)));
}

public VisualPropertiesConfig put(String property, Object value, String[] multiplierSource, String multiplierParameter) {
    return put(new VisualPropertyValueConfig(property, new VisualValueConfig(value, multiplierSource, multiplierParameter)));
}

public VisualPropertiesConfig put(String property, Object value, String multiplierSource, String multiplierParameter) {
    return put(property, value, new String[] {multiplierSource}, multiplierParameter);
}

public VisualPropertiesConfig put(String property, Object value, String smartMultiplier) {
    return put(new VisualPropertyValueConfig(property, new VisualValueConfig(value, smartMultiplier)));
}*/
