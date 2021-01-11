package base.menu.util;

public class MenuItemConfig_ComposedVisual extends MenuItemConfig implements IMenuItemConfig_ComposedVisual {

	private MenuItemConfig[] menuItemConfigs;

	public MenuItemConfig_ComposedVisual(String itemID, MenuItemConfig[] menuItemConfigs) {
		super(itemID);

		this.menuItemConfigs = menuItemConfigs;
	}

	@Override
	public MenuItemConfig[] getMenuItemConfigs() {
		return menuItemConfigs;
	}

}
