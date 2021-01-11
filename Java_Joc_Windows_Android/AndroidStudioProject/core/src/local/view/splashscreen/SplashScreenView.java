package local.view.splashscreen;

import base.IBaseAppComponent;
import base.menu.util.EaseConfig;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.util.AppStyles;
import base.view.AppView;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import local.app.AppLocal;

// This is the game title. A separate menu is used because it is animated in and out in some cases.
public class SplashScreenView extends AppView implements IBaseAppComponent {

    private static final MenuItemConfig[] menuItemConfigs = new MenuItemConfig[] {
            new MenuItemConfig_Data("rendering3DSize", new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_appSize"}, "width")
                    .put("height", 1f, new String[] {"_appSize"}, "height")
            ),

            new MenuItemConfig_Visual("layerTopArea", new String[] {"_root", "visualsHolder"}, VisualCreateConfig.newGroup(), new VisualPropertiesConfig()
                    .putAnimating("alpha", new MenuVisualAnimationProperty()
                            .putState("in", 1f, null, null, EaseConfig.DEFAULT)
                            .putState("out", 0f, null, null, EaseConfig.DEFAULT)
                    )
            ),
            new MenuItemConfig_Visual("bg", new String[] {"_local", "layerTopArea"}, VisualCreateConfig.newSprite("visual/taSplashScreen.atlas", "splashScreenView/bg"), new VisualPropertiesConfig()
                    .put("width", 1f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 1f, new String[] {"_local", "rendering3DSize"}, "height")
            )
    };

    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_SPLASH_SCREEN_OUT_DURATION_S)
    };

    public SplashScreenView(AppLocal app) {
        super(app, menuItemConfigs, menuAnimationDurations);

        //void
    }

    public void show(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        String stateID = computeInState();
        if(!doAnimation || !endStateID.equals(stateID)) {
            setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
        }
    }

    public void hide(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, false);
    }

}
