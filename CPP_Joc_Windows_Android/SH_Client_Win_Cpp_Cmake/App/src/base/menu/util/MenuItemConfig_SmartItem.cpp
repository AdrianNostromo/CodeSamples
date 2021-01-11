#include "MenuItemConfig_SmartItem.h"

MenuItemConfig_SmartItem::MenuItemConfig_SmartItem(
    std::string itemID,
    UiId* parentVisualID,
    std::string initialBaseStateId,
	std::function<ComposedStiledVisual*(IAppMetrics* appMetrics, IAppAssets* appAssets)> classContructor,
    VisualPropertiesConfig* itemProperties)
    : super(itemID),
      parentVisualID(parentVisualID), initialBaseStateId(initialBaseStateId), classContructor(classContructor), itemProperties(itemProperties)
{
    //void
}

MenuItemConfig_SmartItem::MenuItemConfig_SmartItem(
	std::string itemID,
	std::string parentComposedID,
	std::string initialBaseStateId,
	std::function<ComposedStiledVisual* (IAppMetrics* appMetrics, IAppAssets* appAssets)> classContructor,
	VisualPropertiesConfig* itemProperties)
    : MenuItemConfig_SmartItem(
		itemID,
		UiId::newIdFromComposedId(parentComposedID, "_local"),
		initialBaseStateId,
		classContructor,
		itemProperties)
{
    //void
}

VisualPropertiesConfig* MenuItemConfig_SmartItem::getItemProperties() {
    return itemProperties;
}

MenuItemConfig_SmartItem::~MenuItemConfig_SmartItem() {
    //void
}
