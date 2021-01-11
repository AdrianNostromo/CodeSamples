#pragma once

#include "MenuItemConfig.h"
#include "IMenuItemConfig_ComposedVisual.h"
#include <base/list/ArrayList.h>

class MenuItemConfig_ComposedVisual : public MenuItemConfig, virtual public IMenuItemConfig_ComposedVisual {
private: typedef MenuItemConfig super;
private:
    ArrayList<MenuItemConfig*>* menuItemConfigs;

public:
    MenuItemConfig_ComposedVisual(MenuItemConfig_ComposedVisual const&) = delete;
    MenuItemConfig_ComposedVisual(MenuItemConfig_ComposedVisual &&) = default;
    MenuItemConfig_ComposedVisual& operator=(MenuItemConfig_ComposedVisual const&) = delete;
    MenuItemConfig_ComposedVisual& operator=(MenuItemConfig_ComposedVisual &&) = default;

    explicit MenuItemConfig_ComposedVisual(std::string itemID, ArrayList<MenuItemConfig*>* menuItemConfigs);

    ArrayList<MenuItemConfig*>* getMenuItemConfigs() override;

    ~MenuItemConfig_ComposedVisual() override;

};
