package local.view.main;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Color;

import java.util.Map;

import base.IBaseAppComponent;
import base.audio.util.SoundControllerEventListener;
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
import base.visual.IDrawableNode;
import base.visual.IInteractiveDrawable;
import base.visual.SpriteButton;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import local.ads.config.RewardVideoAdsConfig;
import local.ads.util.AdsEventListener;
import local.app.AppLocal;
import local.audio.util.SoundConfig;
import local.font.config.FontConfig;
import local.playgames.leaderboards.config.LeaderboardConfig;
import local.profile.config.ProfileDataIDs;
import local.web.UrlIDs;

public class MainView extends AppView implements IBaseAppComponent {

    private static int COMMAND_NONE = 0;

    private int outCommandType = COMMAND_NONE;

    private int activeEventType = 0;
    private TimerItem activeEventTimer = null;

    private SpriteButton tbtnUnmute = null;
    private IDrawableNode imgTapToPlay = null;
    
    private float tapToPlayAnimationPercent = -1f;

    private static final int EVENT_OPEN_LEADERBOARD = 1;
    private static final int EVENT_OPEN_ACHIEVEMENTS = 2;
    private static final int EVENT_WATCH_REWARD_VIDEO = 3;

    private TimerEventListener delayTimerListener = new TimerEventListener() {
        @Override
        public void timerFinished(TimerItem target) {
            super.timerFinished(target);

            activeEventTimer.stop();
            activeEventTimer = null;

            if(activeEventType == EVENT_WATCH_REWARD_VIDEO) {
                app.adsManager.showRewardVideoAdIfPossible(RewardVideoAdsConfig.REWARD_VIDEO_TYPE_MAIN_VIEW_GEMS, false);
            }else if(activeEventType == EVENT_OPEN_LEADERBOARD) {
                app.leaderboardsWrapper.managedShowLeaderboard(LeaderboardConfig.TotalScore.leaderboardID);
            }else if(activeEventType == EVENT_OPEN_ACHIEVEMENTS) {
                app.achievementsWrapper.managedShowAchievements();
            }else {
                Gdx.app.debug("Error", "Unhandled timer event target.");
            }
        }
    };

    private TouchListener touchListener = new base.interaction.TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(!isActive() || activeEventTimer != null) {
                return false;
            }

            if(target == getItem(new String[] {"_root", "touchAreaBg"})) {
                hide(true, menuAnimationListener);

                app.getCurrentGameWorld().startLevelPlay();
            }

            else if(target == getItem(new String[] {"_root", "btnRateApp"})) {
                app.webManager.openUrlWithID(UrlIDs.PLAY_STORE_APP_PAGE);
            }else if(target == getItem(new String[] {"_root", "tbtnUnmute"})) {
                boolean isOn = tbtnUnmute.getSelected();
                app.soundManager.setIsUnmuted(!isOn);
            }else if(target == getItem(new String[] {"_root", "btnShop"})) {
                onBtnShopDown();
            }else if(target == getItem(new String[] {"_root", "btnLeaderboards"})) {
                startEventTimer(EVENT_OPEN_LEADERBOARD, 0.2f);
            }else if(target == getItem(new String[] {"_root", "btnAchievements"})) {
                startEventTimer(EVENT_OPEN_ACHIEVEMENTS, 0.2f);
            }else if(target == getItem(new String[] {"_root", "btnWatchRewardedVideo"})) {
                startEventTimer(EVENT_WATCH_REWARD_VIDEO, 0.2f);
            }

            else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");

                return false;
            }

            return false;
        }
    };

    private AdsEventListener adsEventListener = new AdsEventListener() {
        @Override
        public void rewardVideoReadyStateStateChanged(boolean isReady) {
            super.rewardVideoReadyStateStateChanged(isReady);

            if(isActive() && !isAnimating()) {
                updateShowStateIfChanged(true, null);
            }
        }

        @Override
        public void rewardVideoComplete(int rewardedVideoID) {
            super.rewardVideoComplete(rewardedVideoID);

            app.profileData.addPlayerGems(10);
        }
    };

    private IMenuAnimationListener menuAnimationListener = new IMenuAnimationListener() {
        @Override
        public void animationFinished(Object target) {
            // Ignore show animation complete events.
            if(isActive()) {
                return ;
            }

            if(outCommandType <= 0) {
                //void
            }else {
                Gdx.app.debug("Error", "Unhandled command type.");
            }

            outCommandType = COMMAND_NONE;
        }
    };

    private SoundControllerEventListener soundManagerEventListener = new SoundControllerEventListener() {
        @Override
        public void unmuteStatusChanged() {
            super.unmuteStatusChanged();

            if(isActiveOrAnimating()) {
                tbtnUnmute.setSelected(app.soundManager.getIsUnmuted());
            }
        }
    };

    /*
    This array is used to create the menu visuals, configure their size, positions
    and menu states (active menu, hidden menu);
    Each menu uses an array like this.
    */
    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[] {
            /*
            This is the main menu container. All menu visuals are attached to it.
            This container is used to animate the menu transparency when it is shown and hidden.
             */
            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, base.visual.util.VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    // This is an example of an animated visual property.
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            //This is the value of the visual property when the menu is active.
                            .putState("_in_", 1f, null, null, EaseConfig.DECELERATION)
                            // This is the value of the visual property when the menu is hidden.
                            .putState("_out_", 0f, null, null, EaseConfig.ACCELERATION)
                    )
            ),

            // This is an example of a screen dimmer image. It can be used to make the game look darker while the menu is active.
            new MenuItemConfig_Visual("screenDimmer", new String[] {"_local", "layerBase"}, base.visual.util.VisualCreateConfig.newSprite("visual/taUtilLinear.atlas", "bg/simple_white_FFFFFF"), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 1f, new String[] {"_appSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
                    // This is the alpha of the image.
                    .put("alpha", 0.0f)
                    // This is the tint color of the image.
                    .put("tintColor", Color.valueOf("#000000FF"))
            ),

            // This is a touch area rectangle use to catch touch event that are used to start the game.
            new MenuItemConfig_Visual("touchAreaBg", new String[] {"_local", "layerBase"}, VisualCreateConfig.newTouchAreaLightweight(), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    // Limit the height to 0.8 of the screen height so the game play doesn't start when the player drags the top of the screen to temporarily disable full screen.
                    .put("height", 0.8f, new String[] {"_appSize"}, "height")
                    .put("anchorX", 0.0f)
                    .put("anchorY", 0.0f)
                    .put("x", 0f)
                    .put("y", 0f)
            ),

            new MenuItemConfig_Visual("imgTitle", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "mainView/imgTitle"), new VisualPropertiesConfig()
                    .put("width", 222f / 360f, new String[] {"_appSize"}, "width")
                    .put("height", 44f / 360f, new String[] {"_appSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 180f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 495f / 570f, new String[] {"_appSize"}, "height")
            ),
            new MenuItemConfig_Visual("imgTapToPlay", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "mainView/imgTapToPlay"), new VisualPropertiesConfig()
                    .put("width", 164f / 360f, new String[] {"_appSize"}, "width")
                    .put("height", 20f / 360f, new String[] {"_appSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 180f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 445f / 570f, new String[] {"_appSize"}, "height")
            ),

            new MenuItemConfig_Visual("lblBestScore", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FontConfig.MediumText_B_AaN_TSCALED.usageFontID, "BEST SCORE: 00"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 180f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 109f / 570f, new String[] {"_appSize"}, "height")
                    .put("alpha", 0.87f)
                    .put("tintColor", Color.valueOf("#000000ff"))
            ),
            new MenuItemConfig_Visual("lblRoundsPlayed", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FontConfig.MediumText_B_AaN_TSCALED.usageFontID, "ROUNDS PLAYED: 000"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 180f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 91f / 570f, new String[] {"_appSize"}, "height")
                    .put("alpha", 0.87f)
                    .put("tintColor", Color.valueOf("#000000ff"))
            ),

            new MenuItemConfig_Visual("btnRateApp", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "mainView/btnRateApp_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("height", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 61f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 56f / 570f, new String[] {"_appSize"}, "height")
            ),
            new MenuItemConfig_Visual("tbtnUnmute", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "mainView/tbtnUnmute_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("height", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 120f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 56f / 570f, new String[] {"_appSize"}, "height")
            ),
            new MenuItemConfig_Visual("btnShop", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "mainView/btnShop_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("height", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 180f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 56f / 570f, new String[] {"_appSize"}, "height")
            ),
            new MenuItemConfig_Visual("btnLeaderboards", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "mainView/btnLeaderboards_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("height", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 240f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 56f / 570f, new String[] {"_appSize"}, "height")
            ),
            new MenuItemConfig_Visual("btnAchievements", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "mainView/btnAchievements_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("height", 42f / 360f, new String[] {"_appSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 300f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 56f / 570f, new String[] {"_appSize"}, "height")
            ),
            
            new MenuItemConfig_Visual("btnWatchRewardedVideo", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "mainView/btnWatchRewardedVideo_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 48f / 360f, new String[] {"_appSize"}, "width")
                    .put("height", 55f / 360f, new String[] {"_appSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 300f / 360f, new String[] {"_appSize"}, "width")
                    .put("y", 285f / 570f, new String[] {"_appSize"}, "height")
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_in_", "_rewardVideoAvailable_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState("_out_", 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("_visibleFloat", new MenuVisualAnimationProperty()
                            .putState(new String[] {"_in_", "_rewardVideoAvailable_"}, 1f, null, null, EaseConfig.DEFAULT)
                            .putState("_out_", 0f, null, null, EaseConfig.DEFAULT)
                    )
            )
    };

    // This list contains the menu animation durations. The values may be changed (eg: 0.5f for half of a second).
    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            // This is the menu show animation duration.
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            // This is the menu hide animation duration.
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            // This is the menu animation duration when .
            new StateChangeDurations("_in_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public MainView(AppLocal app) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    private void onBtnShopDown() {
        hide(true, null);

        app.getShopView().show(true, null, this);
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        tbtnUnmute = (SpriteButton) getItem(new String[]{"_root", "tbtnUnmute"});
        imgTapToPlay = (IDrawableNode) getItem(new String[]{"_root", "imgTapToPlay"});
        
        ((IInteractiveDrawable) getItem(new String[]{"_root", "touchAreaBg"})).setTouchListener(touchListener);

        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnRateApp"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "tbtnUnmute"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnShop"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnLeaderboards"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnAchievements"})).setTouchListener(touchListener);
        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnWatchRewardedVideo"})).setTouchListener(touchListener);
    }

    @Override
    public void create(Map<String, Object> createConfigMap) {
        super.create(createConfigMap);

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
    protected void syncVisuals(boolean syncAbsoluteValues, boolean syncAnimatingValues, String startStateID, String endStateID, float stateCompletionPercent) {
        super.syncVisuals(syncAbsoluteValues, syncAnimatingValues, startStateID, endStateID, stateCompletionPercent);

        //void
    }

    @Override
    protected void activeStateChangedEvent() {
        super.activeStateChangedEvent();

        if(!isActive()) {
            app.getPlayerCurrencyView().hideIncrement();
        }else {
            app.getPlayerCurrencyView().showIncrement();

            syncData(false);

            if(app.getCurrentGameWorld() == null) {
                app.getNewGameWorld();

                app.getCurrentGameWorld().setIsPlayEnabled(true);
                app.getCurrentGameWorld().doCameraDropAnimation();

            }

            tbtnUnmute.setSelected(app.soundManager.getIsUnmuted());
        }
    }

    @Override
    protected void isOnDisplayListChanged() {
        super.isOnDisplayListChanged();

        if(!isVisible()) {
            app.soundManager.removeEventsListener(soundManagerEventListener);
    
            app.adsManager.removeEventsListener(adsEventListener);
        }else {
            app.soundManager.addEventsListener(soundManagerEventListener);
    
            app.adsManager.addEventsListener(adsEventListener);
        }
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(isActive() && !isAnimating()) {
            tapToPlayAnimationPercent += 0.75f * deltaS;
            if(tapToPlayAnimationPercent > 1f) {
                tapToPlayAnimationPercent -= 2f;
            }

            float newAlpha = Math.abs(tapToPlayAnimationPercent);

            imgTapToPlay.setAlpha(newAlpha);

        }
    }

    public void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        String stateID = computeInState();
        if(!doAnimation || !endStateID.equals(stateID)) {
            setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
        }
    }

    @Override
    protected void syncData(boolean doAnimation) {
        super.syncData(doAnimation);

        int roundsPlayed = app.profileData.getDataWithID_int(ProfileDataIDs.ROUNDS_PLAYED);
        int bestScore = app.profileData.getDataWithID_int(ProfileDataIDs.BEST_SCORE);

        ((BitmapLabel) getItem(new String[]{"_root", "lblRoundsPlayed"})).setText(
                "ROUNDS PLAYED: " + Integer.toString(roundsPlayed)
        );
        ((BitmapLabel) getItem(new String[]{"_root", "lblBestScore"})).setText(
                "BEST SCORE: " + Integer.toString(bestScore)
        );
    }

    @Override
    protected String computeStateFlags(String baseState) {
        String state = super.computeStateFlags(baseState);

        if(app.adsManager.isRewardVideoAdDelayFinishedAndCached()) {
            state = state + "rewardVideoAvailable_";
        }

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
