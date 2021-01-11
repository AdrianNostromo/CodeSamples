#pragma once

class VisualPropertiesConfig;

class IMenuItemConfig_DynamicProperties {
public:
    virtual VisualPropertiesConfig* getItemProperties() = 0;

    virtual ~IMenuItemConfig_DynamicProperties() = default;
};
