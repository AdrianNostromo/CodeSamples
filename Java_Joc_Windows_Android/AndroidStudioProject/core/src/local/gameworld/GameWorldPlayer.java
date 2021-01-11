package local.gameworld;

import com.badlogic.gdx.math.Vector3;

import base.audio.util.SoundItem;
import local.app.AppLocal;
import local.audio.util.SoundConfig;
import local.gameworld.util.BallConfig;
import local.gameworld.util.WorldItem;
import local.gameworld.util.WorldUnit;

public class GameWorldPlayer extends GameWorldUnits {

    protected WorldUnit playerUnit;

    protected boolean isPlayerJumpActive = false;
    private int playerJumpDirection = 0;
    private float playerJumpCompletionPercent = 0;

    private float playerFallVelocityS = 0;

    private int pointsCollected = 0;

    private SoundItem sound_ball_jump = null;
    private SoundItem sound_point_collected = null;

    private float playerMoveSpeedS = 3.0f;
    private float playerJumpAnimationSpeedS = 3.0f;
    private static final float playerUnitJumpAmountZ = 1f;

    private static final BallConfig PLAYER_BALL_CONFIG = BallConfig.PlayerSphereA;

    public GameWorldPlayer(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        playerUnit = createNewUnit(PLAYER_BALL_CONFIG, 0, new Vector3(0f, 0f, -tubeRadius + PLAYER_BALL_CONFIG.collisionRadius));

        if(SoundConfig.ball_jump != null) {
            sound_ball_jump = app.soundManager.hookNewSoundItem(SoundConfig.ball_jump);
        }
        if(SoundConfig.point_collected != null) {
            sound_point_collected = app.soundManager.hookNewSoundItem(SoundConfig.point_collected);
        }
    }

    @Override
    protected void gameLoop(float deltaS, int deltaMS) {
        super.gameLoop(deltaS, deltaMS);

        if(getIsLevelStarted()) {
            if(!isTutorialPlayerMovementDisabled) {
                playerUnit.pos.y += playerMoveSpeedS * deltaS;
                playerUnit.angRX -= 1.57f * 360f * deltaS;

                playerUnit.syncVisuals();

                platformsScaleOriginY = playerUnit.pos.y;
            }

            if(!getIsLevelEnded()) {
                removePlatformsWhereBackLowerThan(playerUnit.pos.y - 10f);
                removeItemsWhereBackLowerThan(playerUnit.pos.y - 10f);
            }
        }

        if(getIsLevelEnded()) {
            playerFallVelocityS = Math.min(50f, playerFallVelocityS + 50f * deltaS);
            playerUnit.pos.z -= playerFallVelocityS * deltaS;

            playerUnit.syncVisuals();
        }

        if(getIsLevelStarted() && !getIsLevelEnded() && isPlayerJumpActive) {
            playerJumpCompletionPercent = Math.min(1, playerJumpCompletionPercent + playerJumpAnimationSpeedS * deltaS);

            if(playerJumpCompletionPercent >= 1) {
                // Jump finished.

                playerUnit.jumpOffsetZ = 0;

                isPlayerJumpActive = false;
                playerJumpCompletionPercent = 0;

                playerUnit.tubeRowIndex = (playerUnit.tubeRowIndex + playerJumpDirection);
                if(playerUnit.tubeRowIndex < 0) {
                    playerUnit.tubeRowIndex += TUBE_ROWS_COUNT;
                }
                playerUnit.tubeRowIndex = playerUnit.tubeRowIndex % TUBE_ROWS_COUNT;

                // Move the player to the final position on the z axis.
                float finalPosZ = -tubeRadius + PLAYER_BALL_CONFIG.collisionRadius;

                playerUnit.pos.z = finalPosZ;

                setTubeRotation((360 / TUBE_ROWS_COUNT) * playerUnit.tubeRowIndex);
            }else {
                // Jump is in progress.
                float previousJumpOffsetZ = playerUnit.jumpOffsetZ;
                playerUnit.jumpOffsetZ = (float)Math.sin(Math.PI * playerJumpCompletionPercent) * playerUnitJumpAmountZ;
                float jumpPosDeltaZ = playerUnit.jumpOffsetZ - previousJumpOffsetZ;

                playerUnit.pos.z += jumpPosDeltaZ;

                setTubeRotation((360 / TUBE_ROWS_COUNT) * playerUnit.tubeRowIndex + (360 / TUBE_ROWS_COUNT) * playerJumpCompletionPercent * playerJumpDirection);
            }

            playerUnit.syncVisuals();
        }
    }

    @Override
    protected void collectItem(WorldItem item) {
        super.collectItem(item);

        pointsCollected++;

        if(sound_point_collected != null) {
            sound_point_collected.play();
        }

        playerMoveSpeedS = playerMoveSpeedS * 1.2f;
        playerJumpAnimationSpeedS = playerJumpAnimationSpeedS * 1.1f;
    }

    protected void doPlayerJump(int jumpDirection) {
        playerJumpCompletionPercent = 0;
        playerJumpDirection = jumpDirection;
        isPlayerJumpActive = true;

        if(isTutorialModeActive) {
            tutorialInputReceived();
        }

        if(sound_ball_jump != null) {
            sound_ball_jump.play();
        }
    }

    public int getPointsCollected() {
        return pointsCollected;
    }

    @Override
    public void dispose() {
        if(playerUnit.modelInstance != null) {
            stage3D.removeChild(playerUnit.modelInstance);

            app.embeddedAssetsManager.releaseModelInstance(playerUnit.modelInstance);

            playerUnit.modelInstance = null;
        }

        super.dispose();
    }

}
