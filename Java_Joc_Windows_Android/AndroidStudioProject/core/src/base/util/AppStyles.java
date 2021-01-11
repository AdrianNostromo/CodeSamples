package base.util;

import com.badlogic.gdx.graphics.Color;

import java.lang.reflect.Constructor;

import base.menu.util.EaseConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_ComposedVisual;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.visual.util.VisualPropertyValueConfig;
import base.visual.util.VisualValueConfig;
import local.app.AppLocal;

/**
 * Created by Adrin on 6/4/2016.
 */
public class AppStyles {

    public static final Color THEME_COLOR = Color.valueOf("#4285f4ff");
    public static final Color BG_COLOR_WHITE = Color.valueOf("#ffffffff");
    public static final Color BG_COLOR_GREY = Color.valueOf("#ecececff");
    public static final Color DARK_TEXT_COLOR = Color.valueOf("#000000ff");

    public static final Color RED_COLOR = Color.valueOf("#ea1d24ff");

    public static final Color ICON_DARK_COLOR = Color.valueOf("#000000ff");
    public static final float ICON_DARK_ALPHA = 0.87f;

    // Source: https://material.google.com/patterns/navigation-drawer.html#navigation-drawer-specs
    // Is initialized on app start.
    public static float standardIncrement;
    public static final float STANDARD_INCREMENT_TABLET = 64f;

    public static final float ICON_BLACK_ALPHA = 0.87f;

    public static final Color DIVIDER_DARK_COLOR = Color.valueOf("#000000ff");
    public static final float DIVIDER_DARK_ALPHA = 0.12f;
    public static final float DIVIDER_DARK_THICKNESS_DPM = 2f;

    public static final float DARK_PRIMARY_TEXT_ALPHA = 0.87f;
    public static final float DARK_SECONDARY_TEXT_ALPHA = 0.54f;
    public static final float DARK_DISABLED_TEXT_ALPHA = 0.38f;

    public static final float LIGHT_PRIMARY_TEXT_ALPHA = 1.0f;
    public static final float LIGHT_SECONDARY_TEXT_ALPHA = 0.7f;
    public static final float LIGHT_DISABLED_TEXT_ALPHA = 0.5f;

    public static final float SCREEN_DIMMER_ALPHA = 0.19f;

    public static final float ANIMATION_COMPLEX_DURATION_S = 0.375f;
    public static final float ANIMATION_ENTER_SCREEN_DURATION_S = 0.225f;
    public static final float ANIMATION_LEAVE_SCREEN_DURATION_S = 0.195f;
    // This value is intuitive, not from material template.
    public static final float ANIMATION_IN_STATE_CHANGE_DURATION = 0.2f;

    public static final float ANIMATION_SWITCH_STATE_CHANGE = 0.100f;

    public static final float SLIDER_MOVEMENT_SPEED_MULTIPLIER_S = 10f;

    public static final float ANIMATION_DURATION_S_DEFAULT = 0.225f;

    public static final float ANIMATION_SPLASH_SCREEN_OUT_DURATION_S = 1.2f;

    public static Constructor makeSmartItemConstructor(Class targetClass) {
        Constructor ctor;
        try {
            ctor = targetClass.getDeclaredConstructor(AppLocal.class);
        }catch(NoSuchMethodException x) {
            throw new Error("Error occured.");
        }
        ctor.setAccessible(true);

        return ctor;
    }

    public static final MenuItemConfig ITEM_CONFIG_bg_SIMPLE_WHITE_FFFFFF = new MenuItemConfig_Visual("bg", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
            .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 1f, new String[] {"_local", "bgOffset"}, "x")
            .put("y", 1f, new String[] {"_local", "bgOffset"}, "y")
    );

    public static final MenuItemConfig ITEM_CONFIG_bg_SIMPLE_FAFAFA = new MenuItemConfig_Visual("bg", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_FAFAFA"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
            .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x", 1f, new String[] {"_local", "bgOffset"}, "x")
            .put("y", 1f, new String[] {"_local", "bgOffset"}, "y")
    );

    public static final MenuItemConfig ITEM_CONFIG_bg_CARD = new MenuItemConfig_ComposedVisual("bg", new MenuItemConfig[] {
            new MenuItemConfig_Visual("_TL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_TL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 10f, new String[] {"_dpm"}, "width")
                    .put("height", 10f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 1.0f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_TR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_TR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 10f, new String[] {"_dpm"}, "width")
                    .put("height", 10f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 1.0f)
                    .put("anchorY", 1.0f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_BL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 10f, new String[] {"_dpm"}, "width")
                    .put("height", 10f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_BR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 10f, new String[] {"_dpm"}, "width")
                    .put("height", 10f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 1.0f)
                    .put("anchorY", 0.0f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_TC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_TC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("height", 10f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 1.0f)
                    .put("x",
                            0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_BC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("height", 10f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.0f)
                    .put("x",
                            0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_CL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 10f, new String[] {"_dpm"}, "width")
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_CR"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 1.0f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/card_composed/_CC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "bgOffset"}, "y"
                    )
            )
    });

    public static final MenuItemConfig ITEM_CONFIG_SIDEBAR_LEFT_shadow_RIGHT = new MenuItemConfig_Visual("shadow", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/leftDrawer_right"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 30f, new String[] {"_dpm"}, "width")
            .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
            .put("anchorX", 0.0f)
            .put("anchorY", 0.0f)
            .put("x", 1f, new String[] {"_local", "rendering3DSize"}, "width")
            .put("y", 0f)
    );
    public static final MenuItemConfig ITEM_CONFIG_DRAWER_RIGHT_shadow_LEFT = new MenuItemConfig_Visual("shadow", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/rightDrawer_left"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 30f, new String[] {"_dpm"}, "width")
            .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
            .put("anchorX", 1.0f)
            .put("anchorY", 0.0f)
            .put("x", 0f)
            .put("y", 0f)
    );

    public static final MenuItemConfig ITEM_CONFIG_shadow_CARD = new MenuItemConfig_ComposedVisual("shadow", new MenuItemConfig[] {
            new MenuItemConfig_Visual("_TL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_TL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 20f, new String[] {"_dpm"}, "width")
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_TR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_TR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 20f, new String[] {"_dpm"}, "width")
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_BL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 20f, new String[] {"_dpm"}, "width")
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_BR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 20f, new String[] {"_dpm"}, "width")
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_TC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_TC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_BC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_CL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 20f, new String[] {"_dpm"}, "width")
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_CR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 20f, new String[] {"_dpm"}, "width")
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/card_composed/_CC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            )
    });

    private static float SHADOW_DIALOG_BASE_SIZE = 35f;
    public static final MenuItemConfig ITEM_CONFIG_shadow_DIALOG = new MenuItemConfig_ComposedVisual("shadow", new MenuItemConfig[] {
            new MenuItemConfig_Visual("_TL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_TL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("height", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_TR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_TR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("height", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_BL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("height", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_BR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("height", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_TC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_TC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("height", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_BC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("height", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_CL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_CR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/dialog_composed/_CC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(SHADOW_DIALOG_BASE_SIZE * 2f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            )
    });

    public static final MenuItemConfig ITEM_CONFIG_shadow_RAISED_BUTTON = new MenuItemConfig_ComposedVisual("shadow", new MenuItemConfig[] {
            new MenuItemConfig_Visual("_TL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_TL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_TR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_TR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_BL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_BR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_TC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_TC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(28f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_BC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_BC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(28f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("height", 20f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CL", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_CL"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            -0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CR", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_CR"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            ),
            new MenuItemConfig_Visual("_CC", new String[] {"_local", "parent", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "shadow/button_raised_composed/_CC"), new base.visual.util.VisualPropertiesConfig()
                    .put(new VisualPropertyValueConfig("width",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(28f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put(new VisualPropertyValueConfig("height",
                            new VisualValueConfig(
                                    0f, null, null,
                                    VisualValueConfig.OPERATION_MAX,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                                            VisualValueConfig.OPERATION_SUBTRACT,
                                            new VisualValueConfig(20f, new String[] {"_dpm"}, "width")
                                    )
                            )
                    ))
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "x"
                    )
                    .put("y",
                            0.0f, new String[] {"_local", "parent", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            1f, new String[] {"_local", "parent", "shadowOffset"}, "y"
                    )
            )
    });

    public static final MenuItemConfig ITEM_CONFIG_TITLE_BAR_SINGLE_imgGems = new MenuItemConfig_Visual("imgGems", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taGame.atlas", "titleBar/imgGems"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 24f, new String[] {"_dpm"}, "width")
            .put("height", 20f, new String[] {"_dpm"}, "width")
            .put("anchorX", 0.5f)
            .put("anchorY", 0.5f)
            .put("x",
                    1f, new String[] {"_local", "rendering3DSize"}, "width",
                    VisualValueConfig.OPERATION_SUBTRACT,
                    31f, new String[] {"_dpm"}, "width"
            )
            .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
    );
    /*zxc_x
    public static final MenuItemConfig ITEM_CONFIG_TITLE_BAR_SINGLE_lblGems = new MenuItemConfig_Visual("lblGems", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newLabel(FontInstances.Roboto_Medium_White_AaN_14_DPM.usageFontID, "000"), new base.visual.util.VisualPropertiesConfig()
            .put("anchorX", 1.0f, null, null)
            .put("anchorY", 0.5f, null, null)
            .put("x",
                    1f, new String[] {"_local", "rendering3DSize"}, "width",
                    VisualValueConfig.OPERATION_SUBTRACT,
                    48f, new String[] {"_dpm"}, "width"
            )
            .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
    );*/

    public static final MenuItemConfig ITEM_CONFIG_screenDimmer = new MenuItemConfig_Visual("screenDimmer", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new base.visual.util.VisualPropertiesConfig()
            .put("width", 1f, new String[] {"_appSize"}, "width")
            .put("height", 1f, new String[] {"_appSize"}, "height")
            .put("anchorX", 0.0f)
            .put("anchorY", 0.0f)
            .put("x", 0f)
            .put("y", 0f)
            .put("tintColor", Color.valueOf("#000000FF"))
            .putAnimating("alpha", new MenuVisualAnimationProperty()
                    .putState(new String[] {"in", "SD"}, AppStyles.SCREEN_DIMMER_ALPHA, null, null, EaseConfig.DEFAULT)
                    .putState("in", 0f, null, null, EaseConfig.DEFAULT)
                    .putState("out", 0f, null, null, EaseConfig.DEFAULT)
            )
            .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                    // Use float because it will be automatically casted to boolean.
                    .putState(new String[] {"in", "SD"}, 1f, null, null, EaseConfig.DEFAULT)
                    .putState("in", 0f, null, null, EaseConfig.DEFAULT)
                    .putState("out", 0f, null, null, EaseConfig.DEFAULT)
            )
    );

}
