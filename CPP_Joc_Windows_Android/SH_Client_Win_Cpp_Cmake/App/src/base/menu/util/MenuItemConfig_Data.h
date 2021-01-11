#pragma once

#include "MenuItemConfig.h"
#include "IMenuItemConfig_DynamicProperties.h"

class MenuItemConfig_Data : public MenuItemConfig, virtual public IMenuItemConfig_DynamicProperties {
private: typedef MenuItemConfig super;
private:
    VisualPropertiesConfig* itemProperties;

public:
    MenuItemConfig_Data(MenuItemConfig_Data const&) = delete;
    MenuItemConfig_Data(MenuItemConfig_Data &&) = default;
    MenuItemConfig_Data& operator=(MenuItemConfig_Data const&) = delete;
    MenuItemConfig_Data& operator=(MenuItemConfig_Data &&) = default;

    explicit MenuItemConfig_Data(std::string itemID, VisualPropertiesConfig* itemProperties);

    VisualPropertiesConfig* getItemProperties() override;

    ~MenuItemConfig_Data() override;

};
