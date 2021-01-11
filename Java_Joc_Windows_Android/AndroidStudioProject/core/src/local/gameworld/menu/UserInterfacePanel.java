package local.gameworld.menu;

import com.badlogic.gdx.graphics.Color;

import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.EaseConfig;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.util.AppStyles;
import base.visual.BitmapLabel;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import base.visual.util.VisualValueConfig;
import local.app.AppLocal;
import local.font.config.FontConfig;

// This is the game title. A separate menu is used because it is animated in and out in some cases.
public class UserInterfacePanel extends AnimatedComposedStiledVisual {

    private int score = 0;

    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[]{
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 1f, new String[] {"_appSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerBase", new String[]{"_root", "visualsHolder"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState("in", 1f, null, null, EaseConfig.DEFAULT)
                            .putState("out", 0f, null, null, EaseConfig.DEFAULT)
                    )
                    .putAnimating("y", new MenuVisualAnimationProperty()
                            .putState("in", 0f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                            .putState("out", 150f, new String[] {"_dpm"}, "width", EaseConfig.DEFAULT)
                    )
            ),
            new MenuItemConfig_Visual("lblScore", new String[] {"_local", "layerBase"}, VisualCreateConfig.newLabel(FontConfig.InGame_Score_NCO_TSCALE.usageFontID, "0"), new VisualPropertiesConfig()
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 0.5f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y",
                            1f, new String[] {"_local", "rendering3DSize"}, "height",
                            VisualValueConfig.OPERATION_SUBTRACT,
                            53f, new String[] {"_dpm"}, "width"
                    )
                    .put("tintColor", Color.valueOf("#000000FF"))
            )
    };

    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public UserInterfacePanel(AppLocal app) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    public void setScore(int score) {
        this.score = score;

        if (isActiveOrAnimating()) {
            syncData();
        }
    }

    private void syncData() {
        BitmapLabel lblScore = (BitmapLabel)getItem(new String[]{"_root", "lblScore"});

        String scoreText;
        if(score == 0) {
            scoreText = "O";
        }else {
            scoreText = Integer.toString(score);
        }
        lblScore.setText(scoreText);
    }

    public void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        String stateID = computeInState();
        if(!doAnimation || !endStateID.equals(stateID)) {
            setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
        }
    }

    @Override
    protected String computeStateFlags(String baseState) {
        String state = super.computeStateFlags(baseState);

        //void

        return state;
    }

    @Override
    protected void activeStateChangedEvent() {
        super.activeStateChangedEvent();

        if(!isActive()) {
            //void
        }else {
            syncData();
        }
    }

    public void hide(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, false);
    }

}
