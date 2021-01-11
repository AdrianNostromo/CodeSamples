package local.view.playercurrency;

import com.badlogic.gdx.graphics.Color;

import java.util.Map;

import base.IBaseAppComponent;
import base.menu.util.EaseConfig;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.profile.util.PlayerProfileDataEventListener;
import base.util.AppStyles;
import base.view.AppView;
import base.visual.BitmapLabel;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import local.app.AppLocal;
import local.font.config.FontConfig;
import local.profile.PlayerProfileDataLocal;

public class PlayerCurrencyView extends AppView implements IBaseAppComponent {

    private int activeShowCounter = 0;
    private boolean isDirty_showCounter = false;

    private BitmapLabel lblGems = null;

    private PlayerProfileDataEventListener playerProfileDataEventListener = new PlayerProfileDataEventListener() {
        @Override
        public void profileDataChanged(String dataID) {
            super.profileDataChanged(dataID);

            if(dataID == PlayerProfileDataLocal.CURRENCY_PLAYER_GEMS) {
                if (isActiveOrAnimating()) {
                    syncGemsLabel();
                }
            }
        }
    };

    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 1f, new String[] {"_appSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[] {"_root", "visualsHolder"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState("in", 1f, null, null, EaseConfig.DEFAULT)
                            .putState("out", 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),

            new MenuItemConfig_Visual("lblGems", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FontConfig.PlayerCurrency_AaN_TSCALED.usageFontID, "000"), new VisualPropertiesConfig()
                    .put("anchorX", 1.0f)
                    .put("anchorY", 0.5f)
                    .put("x", 315f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 542f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
                    .put("alpha", 0.87f)
                    .put("tintColor", Color.valueOf("#000000ff"))
            ),
            new MenuItemConfig_Visual("imgGems", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "general/imgGems"), new VisualPropertiesConfig()
                    .put("width", 24f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 24f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 332f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 542f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            )
    };

    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public PlayerCurrencyView(AppLocal app) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        lblGems = (BitmapLabel) getItem(new String[]{"_root", "lblGems"});
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(!isAnimating()) {
            if(isDirty_showCounter) {
                isDirty_showCounter = false;

                if(activeShowCounter < 0) {
                    app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");
                }else if(activeShowCounter > 0) {
                    if(!isActive()) {
                        show(true, null);
                    }
                }else if(activeShowCounter == 0) {
                    if(isActive()) {
                        hide(true, null);
                    }
                }
            }
        }
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
            syncGemsLabel();
        }
    }

    private void syncGemsLabel() {
        int playerGems = app.profileData.getPlayerGems();

        lblGems.setText(Integer.toString(playerGems));
    }

    @Override
    protected void isOnDisplayListChanged() {
        super.isOnDisplayListChanged();

        if(!isVisible()) {
            app.profileData.removeEventsListener(playerProfileDataEventListener);
        }else {
            app.profileData.addEventsListener(playerProfileDataEventListener);
        }
    }

    public void showIncrement() {
        activeShowCounter++;
        isDirty_showCounter = true;
    }

    public void hideIncrement() {
        activeShowCounter--;
        isDirty_showCounter = true;
    }

    @Override
    protected String computeStateFlags(String baseState) {
        String state = super.computeStateFlags(baseState);

        return state;
    }

    @Override
    public void dispose() {
        //void

        super.dispose();
    }
}
