#include "MenuItemConfig_ComposedVisual.h"

MenuItemConfig_ComposedVisual::MenuItemConfig_ComposedVisual(std::string itemID, ArrayList<MenuItemConfig*>* menuItemConfigs)
    : super(itemID), menuItemConfigs(menuItemConfigs)
{
    //void
}

ArrayList<MenuItemConfig*>* MenuItemConfig_ComposedVisual::getMenuItemConfigs() {
    return menuItemConfigs;
}

MenuItemConfig_ComposedVisual::~MenuItemConfig_ComposedVisual() {
    //void
}
