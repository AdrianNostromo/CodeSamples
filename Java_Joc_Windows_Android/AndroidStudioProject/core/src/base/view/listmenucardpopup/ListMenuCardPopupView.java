package base.view.listmenucardpopup;

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
import base.visual.IDrawableNode;
import base.visual.IInteractiveDrawable;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;

public class ListMenuCardPopupView extends ListMenuCardPopupViewList {

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(!isActive()) {
                return false;
            }

            if(target == getItem(new String[] {"_root", "touchAreaFullScreen"})) {
                helperCallCloseRequestedEvent();
            }else if(target == getItem(new String[] {"_root", "contentArea", "touchAreaContentBg"})) {
                //void
            }else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");

                return false;
            }

            return false;
        }
    };

    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 1f, new String[] {"_appSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState("in", 1f, null, null, EaseConfig.DECELERATION)
                            .putState("out", 0f, null, null, EaseConfig.ACCELERATION)
                    )
            ),

            new MenuItemConfig_Visual("touchAreaFullScreen", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(null, null), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
                    //.put("isTouchFallthroughEnabled", true)
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
                            .putState(new String[] {"_in_", "_isFullScreenDimmer_"}, AppStyles.SCREEN_DIMMER_ALPHA, null, null, EaseConfig.DEFAULT)
                            .putState("out", 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_in_", "_isFullScreenDimmer_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState("out", 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_ComposedVisual("contentArea", new MenuItemConfig[] {
                    new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                            // Dynamic position
                    ),

                    menuItemConfigs_contentArea_optionsList,

                    new MenuItemConfig_ComposedVisual("shadowHolder", new MenuItemConfig[] {
                            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                                    .put("width", 1f, new String[] {"_local", "parent", "optionsList"}, "listWidth")
                                    .put("height", 1f, new String[] {"_local", "parent", "optionsList"}, "listHeight")
                            ),
                            new MenuItemConfig_Data("shadowOffset", new VisualPropertiesConfig()
                                    .put("x", 0f)
                                    .put("y", 0f)
                            ),
                            new MenuItemConfig_Visual("layerBase", new String[] {"_local", "parent", "layerBase"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                                    .put("x", 0.5f, new String[] {"_local", "rendering3DSize"}, "width")
                                    .put("y", -0.5f, new String[] {"_local", "rendering3DSize"}, "height")
                            ),
                            AppStyles.ITEM_CONFIG_shadow_CARD
                    })
            })
    };

    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public ListMenuCardPopupView(AppLocal app) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    private void helperCallCloseRequestedEvent() {
        if(viewEventListener != null) {
            viewEventListener.closeRequested(this);
        }
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        ((IInteractiveDrawable) getItem(new String[]{"_root", "touchAreaFullScreen"})).setTouchListener(touchListener);
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
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        if(viewConfig != null) {
            IDrawableNode contentArea_layerBase = (IDrawableNode) getItem(new String[]{"_root", "contentArea", "layerBase"});

            contentArea_layerBase.setPosition(viewConfig.posXDip * app.screenDensityMultiplier.x, viewConfig.posYDip * app.screenDensityMultiplier.y);
        }
    }

}
