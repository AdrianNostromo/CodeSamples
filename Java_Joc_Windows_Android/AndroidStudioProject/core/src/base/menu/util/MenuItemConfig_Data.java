package base.menu.util;

import base.visual.util.VisualPropertiesConfig;

public class MenuItemConfig_Data extends MenuItemConfig implements IMenuItemConfig_DynamicProperties {

	private VisualPropertiesConfig itemProperties;

	public MenuItemConfig_Data(String itemID, VisualPropertiesConfig itemProperties) {
		super(itemID);

		this.itemProperties = itemProperties;
	}

	@Override
	public VisualPropertiesConfig getItemProperties() {
		return itemProperties;
	}
}
