package local.gameworld;

import local.app.AppLocal;
import local.gameworld.util.WorldItem;

public class GameWorldCollision extends GameWorldPlatformsGenerator {

    private boolean isPostJumpCheck = false;

    public static final int LEVEL_END_TYPE_FALL = 1;
    public static final int LEVEL_END_TYPE_FAILED_JUMP = 2;

    public GameWorldCollision(AppLocal app) {
        super(app);

        //void
    }

    @Override
    protected void gameLoop(float deltaS, int deltaMS) {
        super.gameLoop(deltaS, deltaMS);

        if(isPlayerJumpActive) {
            isPostJumpCheck = true;

            return ;
        }

        // Check if a item needs to be collected.
        WorldItem item = getCollidingItemToCollect(playerUnit.pos, playerUnit.config.collisionRadius);
        if(item != null) {
            // Item needs to be collected.

            collectItem(item);
        }

        // Enumerate all active platforms and check if the player is on one of them;
        boolean isOnPlatform = isPlatformUnderPoint(playerUnit.tubeRowIndex, playerUnit.pos.y);

        // Make sure the player doesn't fall off the platform because of low fps.
        if(!isOnPlatform && isTutorialModeActive) {
            isOnPlatform = true;
        }

        if(!isOnPlatform) {
            // Level lost, player is not on the platform;

            // Fade out the player unit shadow.
            playerUnit.hasFallenOfPlatform = true;

            if(!isPostJumpCheck) {
                // Player fell of the platform.

                if(!getIsLevelEnded()) {
                    levelEnded(LEVEL_END_TYPE_FALL);
                }
            }else {
                // Player made a failed jump.

                if(!getIsLevelEnded()) {
                    levelEnded(LEVEL_END_TYPE_FAILED_JUMP);
                }
            }
        }else if(isTutorialModeActive && !isPlayerJumpActive && !isTutorialJumpWaiting) {
            // If the player is near the edge of the current platform, stop the forward movement.

            boolean willBeOnPlatform = isPlatformUnderPoint(playerUnit.tubeRowIndex, playerUnit.pos.y + 0.75f);
            if(!willBeOnPlatform) {
                tutorialInputRequired();
            }
        }

        isPostJumpCheck = false;
    }

}
