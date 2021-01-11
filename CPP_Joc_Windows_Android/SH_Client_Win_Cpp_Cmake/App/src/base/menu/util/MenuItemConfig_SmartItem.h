#pragma once

#include "MenuItemConfig.h"
#include "IMenuItemConfig_DynamicProperties.h"
#include "UiId.h"
#include <functional>

class IAppMetrics;
class IAppAssets;
class ComposedStiledVisual;

class MenuItemConfig_SmartItem : public MenuItemConfig, virtual public IMenuItemConfig_DynamicProperties {
private: typedef MenuItemConfig super;
public:
    UiId* parentVisualID;

    std::string initialBaseStateId;

	std::function<ComposedStiledVisual* (IAppMetrics* appMetrics, IAppAssets* appAssets)> classContructor;

private:
    VisualPropertiesConfig* itemProperties;

public:
    MenuItemConfig_SmartItem(MenuItemConfig_SmartItem const&) = delete;
    MenuItemConfig_SmartItem(MenuItemConfig_SmartItem &&) = default;
    MenuItemConfig_SmartItem& operator=(MenuItemConfig_SmartItem const&) = delete;
    MenuItemConfig_SmartItem& operator=(MenuItemConfig_SmartItem &&) = default;

    explicit MenuItemConfig_SmartItem(
        std::string itemID,
        UiId* parentVisualID,
        std::string initialBaseStateId,
		std::function<ComposedStiledVisual*(IAppMetrics* appMetrics, IAppAssets* appAssets)> classContructor,
        VisualPropertiesConfig* itemProperties);
	
    explicit MenuItemConfig_SmartItem(
        std::string itemID,
        std::string parentComposedID,
        std::string initialBaseStateId,
		std::function<ComposedStiledVisual*(IAppMetrics* appMetrics, IAppAssets* appAssets)> classContructor,
        VisualPropertiesConfig* itemProperties);

    VisualPropertiesConfig* getItemProperties() override;

    ~MenuItemConfig_SmartItem() override;

};
