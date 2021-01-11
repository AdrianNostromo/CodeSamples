#pragma once

#include <base/list/ArrayList.h>

class MenuItemConfig;

class IMenuItemConfig_ComposedVisual {
public:
    virtual ArrayList<MenuItemConfig*>* getMenuItemConfigs() = 0;

	virtual ~IMenuItemConfig_ComposedVisual() = default;
};
