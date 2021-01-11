package local.gameworld;

import com.badlogic.gdx.Gdx;

import base.interaction.TouchListener;
import base.visual.IInteractiveDrawable;
import base.visual.VisualUtil;
import local.app.AppLocal;
import local.gameworld.menu.UserInterfacePanel;
import local.gameworld.util.WorldItem;

public class GameWorldUserInterface extends GameWorldCollision {

    private UserInterfacePanel userInterfacePanel = null;

    private TouchListener touchListener = new TouchListener() {
        @Override
        public boolean touchDown(IInteractiveDrawable target, float worldX, float worldY) {
            // Touch events are ignored while the menu is animating.
            if(getIsLevelEnded() || !getIsLevelStarted() || !getIsPlayEnabled()) {
                return false;
            }

            if(target == visualsMap.get("touchAreaBg")) {
                if(!isPlayerJumpActive && !isTutorialJumpDisabled) {
                    // Get the next platform row index.
                    int nextPlatformRow = getNextPlatformRow(playerUnit.tubeRowIndex, playerUnit.pos.y);

                    int jumpDirection;
                    if(playerUnit.tubeRowIndex == nextPlatformRow) {
                        jumpDirection = 0;
                    }else if((nextPlatformRow == 0 && playerUnit.tubeRowIndex == (TUBE_ROWS_COUNT - 1)) || ((nextPlatformRow != (TUBE_ROWS_COUNT - 1) || playerUnit.tubeRowIndex != 0) && nextPlatformRow > playerUnit.tubeRowIndex)) {
                        jumpDirection = 1;
                    }else {
                        jumpDirection = -1;
                    }

                    doPlayerJump(jumpDirection);
                }
            }else {
                Gdx.app.debug("Error", "Touch event received for a unknown target.");
            }

            return false;
        }
    };

    public GameWorldUserInterface(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(userInterfacePanel != null) {
            userInterfacePanel.appLoop(deltaS, deltaMS);
        }
    }

    @Override
    public void create() {
        super.create();

        visualsMap.put("touchAreaBg", VisualUtil.newTouchAreaLightweight(app, 0, 0, app.screenSize.width, app.screenSize.height, 0f, 0f, layerTutorial, touchListener));

        if(!isTutorialModeActive) {
            userInterfacePanel = new UserInterfacePanel(app);
            userInterfacePanel.create();
            layerUserInterface.addChild(userInterfacePanel.getVisualsHolder());
        }
    }

    @Override
    protected void collectItem(WorldItem item) {
        super.collectItem(item);

        if(userInterfacePanel != null) {
            userInterfacePanel.setScore(getPointsCollected());
        }
    }

    @Override
    public void startLevelPlay() {
        super.startLevelPlay();

        if(userInterfacePanel != null) {
            userInterfacePanel.show(true, null);
        }
    }

    @Override
    protected void levelEnded(int levelEndType) {
        super.levelEnded(levelEndType);

        if(userInterfacePanel != null) {
            userInterfacePanel.hide(true, null);
        }
    }

    private void disposeScorePanel() {
        if(userInterfacePanel != null) {
            userInterfacePanel.dispose();

            userInterfacePanel = null;
        }
    }

    @Override
    public void dispose() {
        disposeScorePanel();

        super.dispose();
    }
}
