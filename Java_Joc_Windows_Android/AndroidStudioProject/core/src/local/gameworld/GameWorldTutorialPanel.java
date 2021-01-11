package local.gameworld;

import base.menu.util.IMenuAnimationListener;
import local.app.AppLocal;
import local.gameworld.menu.TutorialPanel;
import local.gameworld.menu.tutorialpanel.ITutorialPanelController;
import local.playgames.achievements.config.AchievementConfig;
import local.profile.config.ProfileDataIDs;

public class GameWorldTutorialPanel extends GameWorldLayers {

    protected boolean isTutorialModeActive = false;
    protected boolean isTutorialJumpWaiting = false;
    protected boolean isTutorialPlayerMovementDisabled = false;
    protected boolean isTutorialJumpDisabled = false;

    private TutorialPanel tutorialPanel = null;

    private ITutorialPanelController tutorialPanelHandler = new ITutorialPanelController() {
        @Override
        public void onBtnCloseTutorial() {
            isTutorialJumpDisabled = true;
            isTutorialPlayerMovementDisabled = true;

            app.profileData.putDataWithID_boolean(ProfileDataIDs.IS_TUTORIAL_FINISHED, true);

            tutorialPanel.hide(true, tutorialPanelAnimationListener);

            tutorialPanelHandler = null;

            app.achievementsWrapper.bufferAchievementUpload(AchievementConfig.TUTORIALIZED.achievementId);
        }
    };

    private IMenuAnimationListener tutorialPanelAnimationListener = null;

    public GameWorldTutorialPanel(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        if(!app.profileData.getDataWithID_boolean(ProfileDataIDs.IS_TUTORIAL_FINISHED)) {
            isTutorialModeActive = true;
            isTutorialJumpDisabled = true;
        }
    }

    @Override
    public void startLevelPlay() {
        super.startLevelPlay();

        if(isTutorialModeActive) {
            tutorialPanelAnimationListener = new IMenuAnimationListener() {
                @Override
                public void animationFinished(Object target) {
                    if(!tutorialPanel.isActive()) {
                        disposeTutorialPanel();

                        tutorialPanelAnimationListener = null;

                        app.disposeGameWorld();
                        app.getNewGameWorld();
                        app.getCurrentGameWorld().setIsPlayEnabled(true);
                        app.getCurrentGameWorld().doCameraDropAnimation();

                        app.getMainView().show(true, null);
                    }
                }
            };

            tutorialPanel = new TutorialPanel(app, tutorialPanelHandler);
            tutorialPanel.create();
            layerTutorial.addChild(tutorialPanel.getVisualsHolder());

            tutorialPanel.show(true, null);
        }
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(tutorialPanel != null) {
            tutorialPanel.appLoop(deltaS, deltaMS);
        }
    }

    protected final void tutorialInputRequired() {
        if(isTutorialModeActive) {
            isTutorialJumpWaiting = true;
            isTutorialPlayerMovementDisabled = true;
            isTutorialJumpDisabled = false;

            tutorialPanel.setIsWaiting(true);
        }
    }

    protected final void tutorialInputReceived() {
        if(isTutorialModeActive) {
            isTutorialJumpWaiting = false;
            isTutorialPlayerMovementDisabled = false;
            isTutorialJumpDisabled = true;

            tutorialPanel.setIsWaiting(false);
        }
    }

    private void disposeTutorialPanel() {
        if(tutorialPanel != null) {
            tutorialPanel.dispose();

            tutorialPanel = null;
        }
    }

    @Override
    public void dispose() {
        disposeTutorialPanel();

        super.dispose();
    }

}
