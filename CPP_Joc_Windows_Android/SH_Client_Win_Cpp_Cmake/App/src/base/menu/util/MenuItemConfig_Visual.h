#pragma once

#include <base/visual2D/util/VisualCreateConfig.h>
#include "MenuItemConfig.h"
#include "IMenuItemConfig_DynamicProperties.h"
#include "UiId.h"

class MenuItemConfig_Visual : public MenuItemConfig, virtual public IMenuItemConfig_DynamicProperties {
private: typedef MenuItemConfig super;
public:
    UiId* parentComposedID;

    VisualCreateConfig* visualCreateConfig;

private:
    VisualPropertiesConfig* itemProperties;

public:
    MenuItemConfig_Visual(MenuItemConfig_Visual const&) = delete;
    MenuItemConfig_Visual(MenuItemConfig_Visual &&) = default;
    MenuItemConfig_Visual& operator=(MenuItemConfig_Visual const&) = delete;
    MenuItemConfig_Visual& operator=(MenuItemConfig_Visual &&) = default;

    explicit MenuItemConfig_Visual(
        std::string itemID,
        UiId* parentComposedID,
        VisualCreateConfig* visualCreateConfig, VisualPropertiesConfig* itemProperties);
    explicit MenuItemConfig_Visual(
        std::string itemID,
        std::string parentComposedID,
        VisualCreateConfig* visualCreateConfig, VisualPropertiesConfig* itemProperties);

    VisualPropertiesConfig* getItemProperties() override;

    ~MenuItemConfig_Visual() override;

};
