package base.util;

import local.audio.util.SoundConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_SmartItem;
import base.visual.ToggleSwitch;
import base.visual.util.VisualPropertiesConfig;

/**
 * Created by Adrin on 6/4/2016.
 */
public class AppStyles_MaterialDesignComponents {


    // Example
    public static final MenuItemConfig toggleSwitch = new MenuItemConfig_SmartItem("toggleSwitch", new String[] {"_local", "layerBase"}, "_in_", AppStyles.makeSmartItemConstructor(ToggleSwitch.class), new VisualPropertiesConfig()
            .put("x", 0f, new String[] {"_dpm"}, "width")
            .put("y", 0f, new String[] {"_dpm"}, "width")
            .put("toggleOffSoundConfig", SoundConfig.button_down)
            .put("toggleOnSoundConfig", SoundConfig.button_down)
    );

    /*
    // Example
    public static final MenuItemConfig toggleCheckbox = new MenuItemConfig_SmartItem("toggleCheckbox", new String[] {"_local", "layerBase"}, "_in_", AppStyles.makeSmartItemConstructor(ToggleCheckbox.class), new VisualPropertiesConfig()
            .put("x", 257f, new String[] {"_dpm"}, "width")
            .put("y", 134f, new String[] {"_dpm"}, "width")
            .put("toggleOffSoundConfig", SoundConfig.button_down)
            .put("toggleOnSoundConfig", SoundConfig.button_down)
    );*/

}
