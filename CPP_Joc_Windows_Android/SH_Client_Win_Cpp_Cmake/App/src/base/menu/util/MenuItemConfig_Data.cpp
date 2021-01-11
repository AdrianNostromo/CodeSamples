#include "MenuItemConfig_Data.h"

MenuItemConfig_Data::MenuItemConfig_Data(std::string itemID, VisualPropertiesConfig* itemProperties)
    : super(itemID),
      itemProperties(itemProperties)
{
    //void
}

VisualPropertiesConfig* MenuItemConfig_Data::getItemProperties() {
    return itemProperties;
}

MenuItemConfig_Data::~MenuItemConfig_Data() {
    //void
}
