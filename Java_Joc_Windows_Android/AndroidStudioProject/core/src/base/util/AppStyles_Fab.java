package base.util;

import com.badlogic.gdx.graphics.Color;

import local.audio.util.SoundConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Visual;

/**
 * Created by Adrin on 6/4/2016.
 */
public class AppStyles_Fab {

    public static final MenuItemConfig fab_BLANK_MEDIUM_56To68_RED = new MenuItemConfig_Visual("fab", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newButton("visual/taGame.atlas", "general/fab/fabWhite56To68_", SoundConfig.button_down, null), new base.visual.util.VisualPropertiesConfig()
            .put("width", 68f, new String[] {"_dpm"}, "width")
            .put("height", 68f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 0f)
            .put("y", 0f)
            .put("tintColor", Color.valueOf("#EA1D24FF"))
    );

    public static final MenuItemConfig fab_BLANK_MEDIUM_56To68_WHITE = new MenuItemConfig_Visual("fab", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newButton("visual/taGame.atlas", "general/fab/fabWhite56To68_", SoundConfig.button_down, null), new base.visual.util.VisualPropertiesConfig()
            .put("width", 68f, new String[] {"_dpm"}, "width")
            .put("height", 68f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 0f)
            .put("y", 0f)
    );

    public static final MenuItemConfig fab_BLANK_SMALL_40To52_RED = new MenuItemConfig_Visual("fab", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newButton("visual/taGame.atlas", "general/fab/fabWhite40To52_", SoundConfig.button_down, null), new base.visual.util.VisualPropertiesConfig()
            .put("width", 52f, new String[] {"_dpm"}, "width")
            .put("height", 52f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 0f)
            .put("y", 0f)
            .put("tintColor", Color.valueOf("#EA1D24FF"))
    );

    public static final MenuItemConfig fab_BLANK_SMALL_40To52_WHITE = new MenuItemConfig_Visual("fab", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newButton("visual/taGame.atlas", "general/fab/fabWhite40To52_", SoundConfig.button_down, null), new base.visual.util.VisualPropertiesConfig()
            .put("width", 52f, new String[] {"_dpm"}, "width")
            .put("height", 52f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 0f)
            .put("y", 0f)
    );

    public static final MenuItemConfig shadow_MEDIUM_56To68 = new MenuItemConfig_Visual("shadow", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/fab_medium_56To68"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 68f, new String[] {"_dpm"}, "width")
            .put("height", 68f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 0f)
            .put("y", 0f)
    );
    public static final MenuItemConfig shadow_SMALL_40To52 = new MenuItemConfig_Visual("shadow", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/fab_small_40To52"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 52f, new String[] {"_dpm"}, "width")
            .put("height", 52f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 0f)
            .put("y", 0f)
    );

}
