package local.view.levelend;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;

import java.util.Map;

import base.IBaseAppComponent;
import base.interaction.TouchListener;
import base.menu.util.EaseConfig;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.timer.util.TimerEventListener;
import base.timer.util.TimerItem;
import base.util.AppStyles;
import base.view.AppView;
import base.visual.BitmapLabel;
import base.visual.IInteractiveDrawable;
import base.visual.VisualUtil;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import local.app.AppLocal;
import local.audio.util.SoundConfig;
import local.font.config.FontConfig;
import local.profile.config.ProfileDataIDs;
import local.web.UrlIDs;

public class LevelEndView extends AppView implements IBaseAppComponent {

    private int data_currentScore;

    private int activeEventType = 0;
    private TimerItem activeEventTimer = null;

    private static final int EVENT_SHARE_SCREENSHOT = 1;

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(!isActive()) {
                return false;
            }

            if(target == getItem(new String[] {"_root", "touchAreaBg"})) {
                //void
            }

            else if(target == getItem(new String[] {"_root", "btnRetry"})) {
                hide(true, null);

                app.disposeGameWorld();
                app.getNewGameWorld();
                app.getCurrentGameWorld().setIsPlayEnabled(true);
                app.getCurrentGameWorld().doCameraDropAnimation();

                app.getMainView().show(true, null);
            }else if(target == getItem(new String[] {"_root", "btnShare"})) {
                startEventTimer(EVENT_SHARE_SCREENSHOT, 0.2f);
            }else if(target == getItem(new String[] {"_root", "btnRateGame"})) {
                app.webManager.openUrlWithID(UrlIDs.PLAY_STORE_APP_PAGE);
            }

            else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");

                return false;
            }

            return false;
        }
    };

    private TimerEventListener delayTimerListener = new TimerEventListener() {
        @Override
        public void timerFinished(TimerItem target) {
            super.timerFinished(target);

            activeEventTimer.stop();
            activeEventTimer = null;

            if(activeEventType == EVENT_SHARE_SCREENSHOT) {
                if(VisualUtil.saveScreenshot(app, "shareIMG.png")) {
                    app.webManager.shareScreenshot(Gdx.files.getExternalStoragePath() + "shareIMG.png");
                }
            }else {
                Gdx.app.debug("Error", "Unhandled timer event target.");
            }

            activeEventType = 0;
        }
    };

    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 1f, new String[] {"_appSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, base.visual.util.VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState("in", 1f, null, null, EaseConfig.DEFAULT)
                            .putState("out", 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("screenDimmer", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
                    .put("alpha", 0.0f)
                    .put("tintColor", Color.valueOf("#000000FF"))
            ),

            new MenuItemConfig_Visual("touchAreaBg", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaLightweight(), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("imgTitle", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "levelEndView/imgTitle"), new VisualPropertiesConfig()
                    .put("width", 246f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 32f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 285f / 570f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 492f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),

            new MenuItemConfig_Visual("imgScorePanelBg", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "levelEndView/imgScorePanelBg"), new VisualPropertiesConfig()
                    .put("width", 258f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 187f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 285f / 570f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 340f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),

            new MenuItemConfig_Visual("lblCurrentScore", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FontConfig.MediumText_A_N_TSCALED.usageFontID, "00"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 285f / 570f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 365f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("alpha", 0.87f)
                    .put("tintColor", Color.valueOf("#000000ff"))
            ),
            new MenuItemConfig_Visual("lblBestScore", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FontConfig.MediumText_A_N_TSCALED.usageFontID, "00"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 285f / 570f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 290f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("alpha", 0.87f)
                    .put("tintColor", Color.valueOf("#000000ff"))
            ),

            new MenuItemConfig_Visual("btnRetry", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "levelEndView/btnRetry_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 181f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 72f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 285f / 570f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 204f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),
            new MenuItemConfig_Visual("btnShare", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "levelEndView/btnShare_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 181f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 51f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 285f / 570f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 138f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),
            new MenuItemConfig_Visual("btnRateGame", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "levelEndView/btnRateGame_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 181f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 51f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 285f / 570f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 81f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            )
    };

    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public LevelEndView(AppLocal app) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    private boolean startEventTimer(int timerEventType, float delayS) {
        if(activeEventTimer != null) {
            activeEventTimer.stop();
            activeEventTimer = null;
        }

        activeEventType = timerEventType;
        activeEventTimer = app.timerManager.newTimer(delayS, delayTimerListener);

        return true;
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        ((IInteractiveDrawable) getItem(new String[]{"_root", "touchAreaBg"})).setTouchListener(touchListener);

        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnRetry"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnShare"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnRateGame"})).setTouchListener(touchListener);
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        super.create(createConfigMap);

        //void
    }

    @Override
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        //void
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
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(isActiveOrAnimating()) {
            //void
        }
    }

    public void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener, int currentScore) {
        this.data_currentScore = currentScore;

        String stateID = computeInState();
        if(!doAnimation || !endStateID.equals(stateID)) {
            setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
        }

        syncData();
    }

    private void syncData() {
        int bestScore = app.profileData.getDataWithID_int(ProfileDataIDs.BEST_SCORE);

        ((BitmapLabel) getItem(new String[]{"_root", "lblCurrentScore"})).setText(
                Integer.toString(data_currentScore)
        );
        ((BitmapLabel) getItem(new String[]{"_root", "lblBestScore"})).setText(
                Integer.toString(bestScore)
        );
    }

    @Override
    protected String computeStateFlags(String baseState) {
        String state = super.computeStateFlags(baseState);

        return state;
    }

    public void hide(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, false);
    }

    @Override
    public void dispose() {
        //void

        super.dispose();
    }
}
