package base.visual;

import com.badlogic.gdx.graphics.Color;

import base.menu.util.EaseConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;
import base.util.AppStyles;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;

/**
 * Created by Adrin on 6/4/2016.
 */
public class ToggleSwitch extends ToggleControl {

    private static final MenuItemConfig[] itemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("touchAreaBack", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(null, null), new VisualPropertiesConfig()
                    .put("width", 50f, new String[] {"_dpm"}, "width")
                    .put("height", 32f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("bg", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "general/toggleSwitch/bg"), new VisualPropertiesConfig()
                    .put("width", 36f, new String[] {"_dpm"}, "width")
                    .put("height", 14f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 0f)
                    .put("y", 0f)
                    .putAnimating("tintColor", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_disabled_"}, Color.valueOf("#D5D5D5FF"), null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_enabled_", "_on_"}, Color.valueOf("#2196F3FF"), null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_enabled_", "_off_"}, Color.valueOf("#D5D5D5FF"), null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("knobShadowDisabled", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "general/toggleSwitch/knobShadowDisabled"), new VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put("height", 28f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .putAnimating("x", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_on_"}, 8f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                            .putState(new String[] {"_off_"}, -8f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                    )
                    .put("y", 0f, new String[] {"_dpm"}, "width")
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_disabled_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_disabled_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),
            new MenuItemConfig_Visual("knobShadow_enabled", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "general/toggleSwitch/knobShadow_enabled"), new VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put("height", 28f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .putAnimating("x", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_on_"}, 8f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                            .putState(new String[] {"_off_"}, -8f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                    )
                    .put("y", 0f, new String[] {"_dpm"}, "width")
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_enabled_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_enabled_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("knob_disabled", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "general/toggleSwitch/knob"), new VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put("height", 28f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .putAnimating("x", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_on_"}, 8f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                            .putState(new String[] {"_off_"}, -8f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                    )
                    .put("y", 0f, new String[] {"_dpm"}, "width")
                    .put("tintColor", Color.valueOf("#BDBDBDFF"))
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_disabled_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_disabled_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),
            new MenuItemConfig_Visual("knob_enabled", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "general/toggleSwitch/knob"), new VisualPropertiesConfig()
                    .put("width", 28f, new String[] {"_dpm"}, "width")
                    .put("height", 28f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .putAnimating("x", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_on_"}, 8f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                            .putState(new String[] {"_off_"}, -8f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                    )
                    .put("y", 0f, new String[] {"_dpm"}, "width")
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_enabled_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_enabled_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState(new String[] {"_"}, 0f, null, null, EaseConfig.DEFAULT)
                    )
            )
    };

    private static final StateChangeDurations[] animationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_in_", "_in_", 0.1f),
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public ToggleSwitch(AppLocal app) {
        super(app, itemConfigs, animationDurations);

        //void
    }

}
