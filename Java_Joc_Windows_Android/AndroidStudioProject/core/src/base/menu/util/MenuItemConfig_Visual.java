package base.menu.util;

import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;

public class MenuItemConfig_Visual extends MenuItemConfig implements IMenuItemConfig_DynamicProperties {

    public String[] parentVisualID;

    public VisualCreateConfig visualCreateConfig;

    private VisualPropertiesConfig itemProperties;

	public MenuItemConfig_Visual(String itemID, String[] parentVisualID, VisualCreateConfig visualCreateConfig, VisualPropertiesConfig itemProperties) {
		super(itemID);

        this.parentVisualID = parentVisualID;

        this.visualCreateConfig = visualCreateConfig;

        this.itemProperties = itemProperties;
	}

    @Override
    public VisualPropertiesConfig getItemProperties() {
        return itemProperties;
    }

}
