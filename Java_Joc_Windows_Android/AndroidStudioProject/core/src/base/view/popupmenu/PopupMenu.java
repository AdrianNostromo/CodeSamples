package base.view.popupmenu;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;

import java.util.LinkedList;
import java.util.Map;

import base.IBaseAppComponent;
import base.font.config.FontConfig;
import base.font.util.FontSourceGenerated;
import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.EaseConfig;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.util.AppStyles;
import base.view.popupmenu.util.PopupConfig;
import base.view.popupmenu.util.PopupEventListener;
import base.view.popupmenu.util.PopupInstanceData;
import base.visual.BitmapLabel;
import base.visual.IInteractiveDrawable;
import base.visual.InteractiveDrawable;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import base.visual.util.VisualValueConfig;
import local.app.AppLocal;
import local.audio.util.SoundConfig;

public class PopupMenu extends AnimatedComposedStiledVisual implements IBaseAppComponent, IPopupMenu {
    
    public static final FontConfig FC_PopupView_BottomPopupTextFont = new FontConfig(
            "FC_PopupView_BottomPopupTextFont", FontConfig.CHARS_ID_AaN,
            new FontSourceGenerated("font/Roboto_Regular.ttf", 14, Color.WHITE, "_dpm_width", 1f)
    );
    
    public static final FontConfig FC_PopupView_BottomPopupActionFont = new FontConfig(
            "FC_PopupView_BottomPopupActionFont", FontConfig.CHARS_ID_AaN,
            new FontSourceGenerated("font/Roboto_Regular.ttf", 14, Color.WHITE, "_dpm_width", 1f)
    );
    
    private static int COMMAND_NONE = 0;

    private int outCommandType = COMMAND_NONE;

    private PopupInstanceData activePopupData = null;
    private float popupHideDelayS = 0;

    private boolean isPopupShowWaiting = false;
    private float popupShowDelayS = 0;

    LinkedList<PopupInstanceData> nextPopupsList = new LinkedList<PopupInstanceData>();

    private base.interaction.TouchListener touchListener = new base.interaction.TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if (!isActive()) {
                return false;
            }

            if(target == getItem(new String[] {"_root", "backgroundTouchArea"})) {
                if(!isAnimating()) {
                    gotoShowState(computeStateFlags("_out_"), true, menuAnimationListener, true);
                }
            }else if(target == getItem(new String[] {"_root", "actionTouchArea"})) {
                if(activePopupData != null && activePopupData.eventListener != null) {
                    activePopupData.eventListener.popupActionActivated();
                }

                gotoShowState(computeStateFlags("_out_"), true, menuAnimationListener, true);
            }else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");
            }

            return false;
        }
    };

    private IMenuAnimationListener menuAnimationListener = new IMenuAnimationListener() {
        @Override
        public void animationFinished(Object target) {
            // Ignore show animation complete events.
            if (!isActive()) {
                activePopupData = null;

                isPopupShowWaiting = true;
                popupShowDelayS = 0.0f;

                if (outCommandType <= COMMAND_NONE) {
                    //void
                } else {
                    Gdx.app.debug("Error", "Unhandled command type.");
                }

                outCommandType = COMMAND_NONE;
            } else {
                popupHideDelayS = 3.4f;
            }
        }
    };

    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 48f, new String[] {"_dpm"}, "width")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .putAnimating("y", new MenuVisualAnimationProperty()
                            .putState("in", 0f, null, null, EaseConfig.DECELERATION)
                            .putState("out", -1f, new String[] {"_local", "rendering3DSize"}, "height", EaseConfig.SHARP)
                    )
            ),

            new MenuItemConfig_Visual("bg", new String[] {"_root", "layerBase"}, VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
                    .put("tintColor", Color.valueOf("#000000FF"))
            ),

            new MenuItemConfig_Visual("backgroundTouchArea", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("lblTitle", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FC_PopupView_BottomPopupTextFont.usageFontID, "TITLE_TEXT"), new VisualPropertiesConfig()
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.5f)
                    .put("x", 24f, new String[] {"_dpm"}, "width")
                    .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("alpha", AppStyles.LIGHT_PRIMARY_TEXT_ALPHA)
            ),

            new MenuItemConfig_Visual("actionTouchArea", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaAudible(SoundConfig.button_down, null), new VisualPropertiesConfig()
                    // The width is manually set.
                    //.put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 34f, new String[] {"_dpm"}, "width")
                    .put("anchorX", 1.0f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            1f, new String[] {"_local", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_SUBTRACT,
                            12f, new String[] {"_dpm"}, "width"
                    )
                    .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
            ),

            new MenuItemConfig_Visual("lblAction_yellow", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FC_PopupView_BottomPopupActionFont.usageFontID, "ACTION"), new VisualPropertiesConfig()
                    .put("anchorX", 1.0f)
                    .put("anchorY", 0.5f)
                    .put("x",
                            1f, new String[] {"_local", "rendering3DSize"}, "width",
                            VisualValueConfig.OPERATION_SUBTRACT,
                            24f, new String[] {"_dpm"}, "width"
                    )
                    .put("y", 0.5f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("alpha", AppStyles.LIGHT_PRIMARY_TEXT_ALPHA)
                    .put("tintColor", Color.valueOf("#EEFF41FF"))
            ),
    };

    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public PopupMenu(AppLocal app) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        ((InteractiveDrawable) getItem(new String[]{"_root", "backgroundTouchArea"})).setTouchListener(touchListener);
        ((InteractiveDrawable) getItem(new String[]{"_root", "actionTouchArea"})).setTouchListener(touchListener);
    }

    public void showPopup(PopupConfig popupConfig, PopupEventListener eventListener, boolean showDuplicates) {
        // Add the popup to the list.
        if(!showDuplicates && isPopupActiveOrWaiting(popupConfig)) {
            return ;
        }

        nextPopupsList.add(new PopupInstanceData(popupConfig, eventListener));

        if(!isActiveOrAnimating() && !isPopupShowWaiting) {
            // Check if popup can be shown.
            showPopupIfAnyWaiting();
        }
    }

    private boolean isPopupActiveOrWaiting(PopupConfig popupConfig) {
        if(activePopupData != null && activePopupData.popupConfig == popupConfig) {
            return true;
        }

        for (PopupInstanceData entry : nextPopupsList) {
            if(entry.popupConfig == popupConfig) {
                return true;
            }
        }

        return false;
    }

    private void showPopupIfAnyWaiting() {
        if(!isActiveOrAnimating()) {
            if(nextPopupsList.peek() != null) {
                activePopupData = nextPopupsList.removeFirst();

                String stateID = computeInState();
                if(!endStateID.equals(stateID)) {
                    setShowState(computeStateFlags("_out_toIn_"), stateID, true, menuAnimationListener, false);
                }

                // Configure the data.
                ((BitmapLabel) getItem(new String[]{"_root", "lblTitle"})).setText(activePopupData.popupConfig.msg);

                boolean isAnyActionEnabled = false;

                BitmapLabel lblAction_yellow = ((BitmapLabel) getItem(new String[]{"_root", "lblAction_yellow"}));
                if(activePopupData.popupConfig.isYellowActionEnabled) {
                    isAnyActionEnabled = true;

                    lblAction_yellow.setVisible(true);

                    lblAction_yellow.setText(activePopupData.popupConfig.actionText);
                }else {
                    lblAction_yellow.setVisible(false);
                }

                IInteractiveDrawable actionTouchArea = ((IInteractiveDrawable) getItem(new String[]{"_root", "actionTouchArea"}));
                if(isAnyActionEnabled) {
                    actionTouchArea.setVisible(true);
                }else {
                    actionTouchArea.setVisible(false);
                }

                syncCustomVisuals();
            }
        }
    }

    private void syncCustomVisuals() {
        if(activePopupData != null) {
            IInteractiveDrawable actionTouchArea = ((IInteractiveDrawable) getItem(new String[]{"_root", "actionTouchArea"}));

            actionTouchArea.setWidth(activePopupData.popupConfig.touchAreaWidth * app.screenDensityMultiplier.x);
        }
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        super.create(createConfigMap);

        //void
    }

    @Override
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        syncCustomVisuals();
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
    protected void isOnDisplayListChanged() {
        super.isOnDisplayListChanged();

        if(!isVisible()) {
            //void
        }else {
            //void
        }
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(!isActiveOrAnimating()) {
            if (isPopupShowWaiting && popupShowDelayS >= 0) {
                popupShowDelayS = Math.max(0, popupShowDelayS - deltaS);
                if (popupShowDelayS <= 0) {
                    isPopupShowWaiting = false;

                    showPopupIfAnyWaiting();
                }
            }
        }else {
            if (popupHideDelayS > 0) {
                popupHideDelayS = Math.max(0, popupHideDelayS - deltaS);
                if (popupHideDelayS <= 0) {
                    gotoShowState(computeStateFlags("_out_"), true, menuAnimationListener, true);
                }
            }
        }
    }

    @Override
    protected String computeStateFlags(String baseState) {
        String state = super.computeStateFlags(baseState);

        return state;
    }

    public void hideActivePopup(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        if(isActive()) {
            gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, true);
        }
    }

    @Override
    public void dispose() {
        //void

        super.dispose();
    }
}
