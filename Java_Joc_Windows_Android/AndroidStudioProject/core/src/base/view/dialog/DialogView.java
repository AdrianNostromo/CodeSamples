package base.view.dialog;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;

import java.util.Map;

import base.interaction.TouchListener;
import base.menu.util.EaseConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_ComposedVisual;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import local.app.AppLocal;
import base.util.AppStyles;
import base.visual.IInteractiveDrawable;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import base.visual.util.VisualValueConfig;

public class DialogView extends DialogViewControls {

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(!isActive()) {
                return false;
            }

            if(target == getItem(new String[] {"_root", "touchAreaFullScreen"})) {
                handleTouchRequest_close();
            }else if(target == getItem(new String[] {"_root", "contentArea", "touchAreaContentBg"})) {
                //void
            }else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");

                return false;
            }

            return false;
        }
    };

    protected static final MenuItemConfig menuItemConfigs_contentArea = new MenuItemConfig_ComposedVisual("contentArea", new MenuItemConfig[] {
            new MenuItemConfig_Data("titleAreaSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_dataMap"}, "width")
                    .put("height", 1f, new String[] {"_dataMap"}, "titleAreaHeight")
            ),
            new MenuItemConfig_Data("bodyAreaSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_dataMap"}, "width")
                    .put("height", 1f, new String[] {"_dataMap"}, "bodyAreaHeight")
            ),
            new MenuItemConfig_Data("controlsAreaSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_dataMap"}, "width")
                    .put("height", 1f, new String[] {"_dataMap"}, "controlsAreaHeight")
            ),
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "titleAreaSize"}, "width")
                    .putManualValue("height", new VisualValueConfig(
                            1f, new String[] {"_local", "titleAreaSize"}, "height",
                            VisualValueConfig.OPERATION_ADD,
                            new VisualValueConfig(
                                    1f, new String[] {"_local", "bodyAreaSize"}, "height",
                                    VisualValueConfig.OPERATION_ADD,
                                    new VisualValueConfig(
                                            1f, new String[] {"_local", "controlsAreaSize"}, "height"
                                    )
                            )
                    ))
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .putAnimating("x", new MenuVisualAnimationProperty()
                            /*.putState(new String[] {"_out_"},
                                    -1f, new String[] {"_local", "rendering3DSize"}, "width",
                                    AppStyles.EASE_FUNCTION_ACCELERATION_ID, AppStyles.EASE_FUNCTION_ACCELERATION_CONFIG
                            )*/
                            .putState(new String[] {"_"},
                                    0.5f, new String[] {"_appSize"}, "width",
                                    VisualValueConfig.OPERATION_SUBTRACT,
                                    0.5f, new String[] {"_local", "rendering3DSize"}, "width",
                                    EaseConfig.DECELERATION
                            )
                    )
                    .putAnimating("y", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_out_"},
                                    1f, new String[] {"_appSize"}, "height",
                                    EaseConfig.DEFAULT
                            )
                            .putState(new String[] {"_"},
                                    0.5f, new String[] {"_appSize"}, "height",
                                    VisualValueConfig.OPERATION_SUBTRACT,
                                    0.5f, new String[] {"_local", "rendering3DSize"}, "height",
                                    EaseConfig.DEFAULT
                            )
                    )
            ),

            new MenuItemConfig_ComposedVisual("shadowHolder", new MenuItemConfig[] {
                        new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                                .put("width", 1f, new String[] {"_local", "parent", "rendering3DSize"}, "width")
                                .put("height", 1f, new String[] {"_local", "parent", "rendering3DSize"}, "height")
                        ),
                    new MenuItemConfig_Data("shadowOffset", new VisualPropertiesConfig()
                            .put("x", 0f)
                            .put("y", 0f)
                    ),
                    new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                            .put("x", 0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "width")
                            .put("y", 0.5f, new String[] {"_local", "parent", "rendering3DSize"}, "height")

                    ),
                    AppStyles.ITEM_CONFIG_shadow_DIALOG
            }),

            new MenuItemConfig_Visual("touchAreaContentBg", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(null, null), new VisualPropertiesConfig()
                    .putAnimating("width", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_"}, 1f, new String[] {"_local", "rendering3DSize"}, "width", EaseConfig.DEFAULT)
                    )
                    .putAnimating("height", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_"}, 1f, new String[] {"_local", "rendering3DSize"}, "height", EaseConfig.DEFAULT)
                    )
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("bg", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new base.visual.util.VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            menuItemConfigs_contentArea_titleArea,
            menuItemConfigs_contentArea_bodyArea,
            menuItemConfigs_contentArea_controlsArea
    });

    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 1f, new String[] {"_appSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    //void
            ),

            new MenuItemConfig_Visual("screenDimmerFullScreen", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
                    .put("tintColor", Color.valueOf("#000000FF"))
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState("in", AppStyles.SCREEN_DIMMER_ALPHA, null, null, EaseConfig.DEFAULT)
                            .putState("out", 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("touchAreaFullScreen", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(null, null), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            menuItemConfigs_contentArea
    };

    private static final StateChangeDurations[] easeDurationsSList = new StateChangeDurations[] {
            new StateChangeDurations("_in_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public DialogView(AppLocal app) {
        super(app, menuItemConfigs, easeDurationsSList);

        //void
    }

    private void handleTouchRequest_close() {
        if(dialogEventListener != null) {
            dialogEventListener.closeRequested(this);
        }
    }

    @Override
    protected void activeStateChangedEvent() {
        super.activeStateChangedEvent();

        if(!isActive()) {
            //void
        }else {
            //void
        }
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        ((IInteractiveDrawable) getItem(new String[]{"_root", "touchAreaFullScreen"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "contentArea", "touchAreaContentBg"})).setTouchListener(touchListener);
    }
}
