package base.util;

import com.badlogic.gdx.graphics.Color;

import local.audio.util.SoundConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import base.visual.util.VisualValueConfig;

/**
 * Created by Adrin on 6/4/2016.
 */
public class AppStyles_TitleBar {

    public static final MenuItemConfig size_SINGLE = new MenuItemConfig_Data("rendering3DSize", new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "parent", "rendering3DSize"}, "width")
            .put("height", 56f, new String[] {"_dpm"}, "width")
    );
    public static final MenuItemConfig size_TABS_ZONE = new MenuItemConfig_Data("rendering3DSize", new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "parent", "rendering3DSize"}, "width")
            .put("height", 56f, new String[] {"_dpm"}, "width")
    );
    public static final MenuItemConfig size_DEFAULT_AND_TABS = new MenuItemConfig_Data("rendering3DSize", new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "parent", "rendering3DSize"}, "width")
            .put("height", 56f + 48f, new String[] {"_dpm"}, "width")
    );

    public static final MenuItemConfig bgOffset = new MenuItemConfig_Data("bgOffset", new base.visual.util.VisualPropertiesConfig()
            .put("x", 0.5f, new String[] {"_local", "rendering3DSize"}, "width")
            .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
    );
    public static final MenuItemConfig layerBase = new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newGroup(), new base.visual.util.VisualPropertiesConfig()
            .put("x", 0f)
            .put("y",
                    1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                    VisualValueConfig.OPERATION_SUBTRACT,
                    1f, new String[] {"_local", "rendering3DSize"}, "height"
            )
    );

    public static final MenuItemConfig bg = new MenuItemConfig_Visual("bg", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
            .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 1f, new String[] {"_local", "bgOffset"}, "x")
            .put("y", 1f, new String[] {"_local", "bgOffset"}, "y")
            .put("tintColor", Color.valueOf("#EA1D24FF"))
    );

    public static final MenuItemConfig taBack = new MenuItemConfig_Visual("taBack", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaLightweight(), new VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
            .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 1f, new String[] {"_local", "bgOffset"}, "x")
            .put("y", 1f, new String[] {"_local", "bgOffset"}, "y")
    );

    public static final MenuItemConfig shadow = new MenuItemConfig_Visual("shadow", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/titleBar_bottom"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
            .put("height", 9f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.0f)
            .put("anchorY", 1.0f)
            .put("x", 0f)
            .put("y", 0f)
    );

    public static final MenuItemConfig ITEM_CONFIG_shadow_BOTTOM_BAR = new MenuItemConfig_Visual("shadow", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/bottomBar_top"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
            .put("height", 20f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.0f)
            .put("anchorY", 0.0f)
            .put("x", 0f)
            .put("y", 1f, new String[] {"_local", "rendering3DSize"}, "height")
    );

    public static final MenuItemConfig ITEM_CONFIG_TITLE_BAR_SINGLE_btnMenu = new MenuItemConfig_Visual("btnMenu", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newButton("visual/taGame.atlas", "titleBar/btnMenu_", SoundConfig.button_down, null), new base.visual.util.VisualPropertiesConfig()
            .put("width", 24f, new String[] {"_dpm"}, "width")
            .put("height", 24f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 28f, new String[] {"_dpm"}, "width")
            .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
    );
    public static final MenuItemConfig btnBack = new MenuItemConfig_Visual("btnBack", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newButton("visual/taGame.atlas", "titleBar/btnBack_", SoundConfig.button_down, null), new base.visual.util.VisualPropertiesConfig()
            .put("width", 24f, new String[] {"_dpm"}, "width")
            .put("height", 24f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 27f, new String[] {"_dpm"}, "width")
            .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
    );
    public static final MenuItemConfig ITEM_CONFIG_TITLE_BAR_SINGLE_btnHome = new MenuItemConfig_Visual("btnHome", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newButton("visual/taGame.atlas", "titleBar/btnHome_", SoundConfig.button_down, null), new base.visual.util.VisualPropertiesConfig()
            .put("width", 24f, new String[] {"_dpm"}, "width")
            .put("height", 24f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 29f, new String[] {"_dpm"}, "width")
            .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
    );
    public static final MenuItemConfig ITEM_CONFIG_TITLE_BAR_SINGLE_btnMore = new MenuItemConfig_Visual("btnMore", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "titleBar/btnMore_", SoundConfig.button_down, null), new base.visual.util.VisualPropertiesConfig()
            .put("width", 24f, new String[] {"_dpm"}, "width")
            .put("height", 24f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x",
                    1f, new String[] {"_local", "rendering3DSize"}, "width",
                    VisualValueConfig.OPERATION_SUBTRACT,
                    21f, new String[] {"_dpm"}, "width"
            )
            .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
    );

}
