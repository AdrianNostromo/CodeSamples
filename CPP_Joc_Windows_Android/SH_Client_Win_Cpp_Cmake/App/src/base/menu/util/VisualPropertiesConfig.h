#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include "VisualPropertyValueConfig.h"
#include "MenuVisualAnimationProperty.h"
#include <graphics/visual/Color.h>

class VisualPropertiesConfig {pub dCtor(VisualPropertiesConfig);
    pub ArrayList<VisualPropertyValueConfig*> propertyConfigs{};

    pub explicit VisualPropertiesConfig();

    pub VisualPropertiesConfig* put(VisualPropertyValueConfig* composedValue);
    pub VisualPropertiesConfig* put(
		std::string property, VisualValueConfig* smartValue);
    pub VisualPropertiesConfig* put(
        std::string property,
		std::nullptr_t value, std::string valueMultiplierComposedIdProp);
    pub VisualPropertiesConfig* put(
        std::string property,
		std::shared_ptr<base::IWrappedValue> value, std::string valueMultiplierComposedIdProp);
    pub VisualPropertiesConfig* put(
        std::string property,
        int value, std::string valueMultiplierComposedIdProp);
    pub VisualPropertiesConfig* put(
        std::string property,
        float value, std::string valueMultiplierComposedIdProp);
    pub VisualPropertiesConfig* put(
        std::string property,
		std::shared_ptr<float> value, std::string valueMultiplierComposedIdProp);
    pub VisualPropertiesConfig* put(std::string property, std::shared_ptr<base::IWrappedValue> value);
    pub VisualPropertiesConfig* put(std::string property, int value);
    pub VisualPropertiesConfig* put(std::string property, float value);
    pub VisualPropertiesConfig* put(std::string property, bool value);
    pub VisualPropertiesConfig* put(std::string property, std::string value);
    pub VisualPropertiesConfig* put(std::string property, const char* value);
    pub VisualPropertiesConfig* put(std::string property, Color* value);
    pub VisualPropertiesConfig* put(std::string property, Color value);
    pub VisualPropertiesConfig* put(std::string property, std::string* value);
	// This is threated as a string* nullptr;
    pub VisualPropertiesConfig* put(std::string property, std::nullptr_t value);
    pub VisualPropertiesConfig* put(
        std::string property,
        std::shared_ptr<base::IWrappedValue> valueA, std::string valueAMultiplierComposedIdProp,
        int operationType,
        std::shared_ptr<base::IWrappedValue> valueB, std::string valueBMultiplierComposedIdProp);
    pub VisualPropertiesConfig* put(
        std::string property,
        float valueA, std::string valueAMultiplierComposedIdProp,
        int operationType,
        float valueB, std::string valueBMultiplierComposedIdProp);
    pub VisualPropertiesConfig* put(
        std::string property,
        float valueA, std::string valueAMultiplierComposedIdProp,
        int operationType,
        float valueB, std::nullptr_t valueBMultiplierComposedIdProp);
    pub VisualPropertiesConfig* put(
		std::string property,
		float valueA, std::string valueAMultiplierComposedIdProp,
		int operationType,
		VisualValueConfig* valueB);

    pub VisualPropertiesConfig* putAnimating(std::string property, MenuVisualAnimationProperty* value);

    pub virtual ~VisualPropertiesConfig();
};
