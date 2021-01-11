#include "MenuItemConfig_Visual.h"

MenuItemConfig_Visual::MenuItemConfig_Visual(
    std::string itemID,
    UiId* parentComposedID,
    VisualCreateConfig* visualCreateConfig, VisualPropertiesConfig* itemProperties)
    : super(itemID),
      parentComposedID(parentComposedID),
      visualCreateConfig(visualCreateConfig), itemProperties(itemProperties)
{
	if (itemID.size() == 0) {
		throw LogicException(LOC);
	}
}

MenuItemConfig_Visual::MenuItemConfig_Visual(
    std::string itemID,
    std::string parentComposedID,
    VisualCreateConfig* visualCreateConfig, VisualPropertiesConfig* itemProperties)
    : MenuItemConfig_Visual(
        itemID,
        UiId::newIdFromComposedId(parentComposedID, "_local"),
        visualCreateConfig, itemProperties)
{
    //void
}

VisualPropertiesConfig* MenuItemConfig_Visual::getItemProperties() {
    return itemProperties;
}

MenuItemConfig_Visual::~MenuItemConfig_Visual                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           () {
    //void
}

/*
asdAA;


public MenuItemConfig_Visual(String itemID, VisualCreateConfig visualCreateConfig, VisualPropertiesConfig itemProperties) {
    //this(itemID, null, visualCreateConfig, itemProperties);

    super(itemID);

    this.parentVisualID = null;

    this.visualCreateConfig = visualCreateConfig;

    this.itemProperties = itemProperties;
}
*/
