package base.menu.util;

import java.lang.reflect.Constructor;

import base.visual.util.VisualPropertiesConfig;

public class MenuItemConfig_SmartItem extends MenuItemConfig implements IMenuItemConfig_DynamicProperties {

    public String[] parentVisualID;

    public String initialBaseStateId;

    public Constructor classContructor;

	private VisualPropertiesConfig itemProperties;

	public MenuItemConfig_SmartItem(String itemID, String[] parentVisualID, String initialBaseStateId, Constructor classContructor, VisualPropertiesConfig itemProperties) {
		super(itemID);

        this.parentVisualID = parentVisualID;

        this.initialBaseStateId = initialBaseStateId;

        this.classContructor = classContructor;

		this.itemProperties = itemProperties;
	}

	@Override
	public VisualPropertiesConfig getItemProperties() {
		return itemProperties;
	}

}
