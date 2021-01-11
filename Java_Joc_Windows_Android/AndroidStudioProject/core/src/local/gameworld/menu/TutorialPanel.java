package local.gameworld.menu;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.g2d.Animation;

import java.util.Map;

import base.ease.EaseFunctions;
import base.interaction.TouchListener;
import base.menu.AnimatedComposedStiledVisual;
import base.menu.util.EaseConfig;
import base.menu.util.IMenuAnimationListener;
import base.menu.util.MenuItemConfig;
import base.menu.util.MenuItemConfig_Data;
import base.menu.util.MenuItemConfig_Visual;
import base.menu.util.MenuVisualAnimationProperty;
import base.menu.util.StateChangeDurations;
import base.util.AppStyles;
import base.util.Point2D;
import base.visual.IInteractiveDrawable;
import base.visual.MovieClip;
import base.visual.util.VisualCreateConfig;
import base.visual.util.VisualPropertiesConfig;
import local.app.AppLocal;
import local.audio.util.SoundConfig;
import local.gameworld.menu.tutorialpanel.ITutorialPanelController;

// This is the game title. A separate menu is used because it is animated in and out in some cases.
public class TutorialPanel extends AnimatedComposedStiledVisual {

    private ITutorialPanelController menuController;

    private boolean isHandIndicatorActive = false;
    private int touchDirection = 0;

    private int handAnimationStep = 0;
    private float handAnimationStepLifeS = 0;

    private boolean isWaiting = false;

    private MovieClip mcHand = null;

    private static final float[] handStepSpeedsList = new float[] {
            1f,
            1f,
            1f,
            1f,
            1f
    };

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if (!isActive()) {
                return false;
            }

            if(target == getItem(new String[]{"_root", "btnEndTutorial"})) {
                if(menuController != null) {
                    menuController.onBtnCloseTutorial();
                }
            } else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");
            }

            return false;
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

            new MenuItemConfig_Visual("imgTitle", new String[] {"_local", "layerBase"}, VisualCreateConfig.newSprite("visual/taGame.atlas", "gameWorld/tutorialPanel/imgTitle"), new VisualPropertiesConfig()
                    .put("width", 360f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 54f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 180f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 477f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),
            new MenuItemConfig_Visual("btnEndTutorial", new String[] {"_local", "layerBase"}, VisualCreateConfig.newButton("visual/taGame.atlas", "gameWorld/tutorialPanel/btnEndTutorial_", SoundConfig.button_down, null), new VisualPropertiesConfig()
                    .put("width", 132f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 46f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    .put("x", 180f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("y", 78f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            ),

            new MenuItemConfig_Visual("mcHand", new String[] {"_local", "layerBase"}, VisualCreateConfig.newMovieClip("visual/taGame.atlas", "gameWorld/tutorialPanel/mcHand_", 1, 2, 3, 1f / 1f, Animation.PlayMode.LOOP, false), new VisualPropertiesConfig()
                    .put("width", 52f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("height", 64f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    .put("anchorX", 0.5f)
                    .put("anchorY", 0.5f)
                    // Position is changed in programmatically.
                    //.put("x", 180f / 360f, new String[] {"_local", "rendering3DSize"}, "width")
                    //.put("y", 78f / 570f, new String[] {"_local", "rendering3DSize"}, "height")
            )
    };

    private static final StateChangeDurations[] menuAnimationDurations = new StateChangeDurations[] {
            new StateChangeDurations("_out_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_out_", AppStyles.ANIMATION_DURATION_S_DEFAULT),
            new StateChangeDurations("_in_", "_in_", AppStyles.ANIMATION_DURATION_S_DEFAULT)
    };

    public TutorialPanel(AppLocal app, ITutorialPanelController menuController) {
        super(app, menuItemConfigs, menuAnimationDurations);

        this.menuController = menuController;
    }

    @Override
    protected void createVisuals(Map<String, Object> createConfigMap) {
        super.createVisuals(createConfigMap);

        ((IInteractiveDrawable) getItem(new String[]{"_root", "btnEndTutorial"})).setTouchListener(touchListener);

        mcHand = (MovieClip)getItem(new String[]{"_root", "mcHand"});
        mcHand.setKeyFrameIndexOverride(0);
        mcHand.setVisible(false);
    }

    public void setIsHandIndicatorActive(boolean isHandIndicatorActive, int touchDirection) {
        if(this.isHandIndicatorActive == isHandIndicatorActive && this.touchDirection == touchDirection) {
            return ;
        }

        this.isHandIndicatorActive = isHandIndicatorActive;
        this.touchDirection = touchDirection;

        handAnimationStep = 0;
        handAnimationStepLifeS = 0;

        syncHandAnimationVisuals();
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(isHandIndicatorActive || mcHand.getVisible()) {
            handAnimationStepLifeS += deltaS;

            syncHandAnimationVisuals();
        }
    }

    private void syncHandAnimationVisuals() {
        if(!this.isHandIndicatorActive) {
            float newAlpha = Math.max(0f, mcHand.getAlpha() - (4f / 60f));

            mcHand.setAlpha(newAlpha);
            if(newAlpha <= 0) {
                mcHand.setVisible(false);
            }

            return ;
        }

        int kfFrom;
        int kfTo;

        if(touchDirection > 0) {
            kfFrom = 0;
            kfTo = 1;
        }else {
            kfFrom = 1;
            kfTo = 0;
        }

        Point2D animPosStart = new Point2D((400f / 360f) * app.screenSize.width, (145f / 570f) * app.screenSize.height);
        Point2D animPosEnd = new Point2D((280f / 360f) * app.screenSize.width, (175f / 570f) * app.screenSize.height);

        int previousAnimationStep = handAnimationStep;

        float cp;
        if(handAnimationStep == 0) {
            // Activate the correct visual at the initial pos.

            mcHand.setVisible(true);
            mcHand.setAlpha(1f);
            mcHand.setKeyFrameIndexOverride(kfFrom);
            mcHand.setPosition(animPosStart.x, animPosStart.y);

            handAnimationStep++;
        }else if(handAnimationStep == 1) {
            // Animate the visuals in.

            cp = Math.min(1, handAnimationStepLifeS / 0.5f);

            float posX = EaseFunctions.computeEaseValue(EaseFunctions.linear, null, cp, animPosStart.x, animPosEnd.x);
            float posY = EaseFunctions.computeEaseValue(EaseFunctions.linear, null, cp, animPosStart.y, animPosEnd.y);

            mcHand.setPosition(posX, posY);

            if(cp >= 1) {
                handAnimationStep++;
            }
        }else if(handAnimationStep == 2) {
            // Small in pre pause.

            if(handAnimationStepLifeS >= 0.25f) {
                handAnimationStep++;
            }
        }else if(handAnimationStep == 3) {
            // Switch to the new active visual.

            mcHand.setKeyFrameIndexOverride(kfTo);

            handAnimationStep++;
        }else if(handAnimationStep == 4) {
            // Post in pause.

            if(handAnimationStepLifeS >= 0.5f) {
                handAnimationStep++;
            }
        }else if(handAnimationStep == 5) {
            // Fade out.

            cp = Math.min(1, handAnimationStepLifeS / 0.5f);

            float newAlpha = EaseFunctions.computeEaseValue(EaseFunctions.linear, null, cp, 1, 0);

            mcHand.setAlpha(newAlpha);

            if(cp >= 1) {
                mcHand.setVisible(false);

                handAnimationStep++;
            }
        }else if(handAnimationStep == 6) {
            // Post delay.

            if(handAnimationStepLifeS >= 0.8f) {
                handAnimationStep = 0;
            }
        }else {
            Gdx.app.debug("Error", "Unhandled tutorial animation step: " + handAnimationStep);

            return ;
        }

        if(handAnimationStep != previousAnimationStep) {
            handAnimationStepLifeS = 0;
        }
    }

    public void setIsWaiting(boolean isWaiting) {
        if(this.isWaiting == isWaiting) {
            return ;
        }

        this.isWaiting = isWaiting;

        updateShowStateIfChanged(true, null);

        if(isWaiting) {
            setIsHandIndicatorActive(true, 1);
        }else {
            setIsHandIndicatorActive(false, 1);
        }
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

    public void hide(boolean doAnimation, IMenuAnimationListener menuAnimationListener) {
        gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, false);
    }

}
