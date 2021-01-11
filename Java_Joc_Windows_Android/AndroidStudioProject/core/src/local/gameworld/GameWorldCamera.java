package local.gameworld;

import com.badlogic.gdx.math.Vector3;

import base.ease.EaseFunctions;
import base.ease.pennereasing.EaseFunctionCubic;
import local.app.AppLocal;

public class GameWorldCamera extends GameWorldPlayer {

    private Vector3 cameraFocusPosition;

    private boolean isCameraDropActive = false;
    private float cameraDropCompletionPercent = 0f;
    private static final float cameraDropAnimationAmount = 8f;

    private float levelEndCameraDropPercent = 0;

    // The shop menu disables this so it can animate it as needed.
    private boolean isCinematicCamera = false;

    public GameWorldCamera(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        cameraFocusPosition = new Vector3();

        this.synchronizeCamera(0);
    }

    public void hookCinematicCamera() {
        this.isCinematicCamera = true;
    }

    public void setCinematicCameraPos(float x, float y, float z) {
        setCamera3DPosition(x, y, z);
    }

    public void setCinematicCameraLookAt(float x, float y, float z) {
        setCamera3DLookAtTarget(x, y, z);
    }

    public void releaseCinematicCamera() {
        isCinematicCamera = false;
    }

    public void doCameraDropAnimation() {
        isCameraDropActive = true;
        cameraDropCompletionPercent = 0f;
    }

    @Override
    protected void gameLoop(float deltaS, int deltaMS) {
        super.gameLoop(deltaS, deltaMS);

        if(isCinematicCamera) {
            return;
        }

        this.synchronizeCamera(deltaS);
    }

    private void synchronizeCamera(float deltaS) {
        if(isCinematicCamera) {
            return ;
        }

        if(!getIsLevelEnded()) {
            float posZ = 2.2f;
            float cameraExtraOffsetZ = 0f;

            if(isCameraDropActive) {
                cameraDropCompletionPercent = Math.min(1, cameraDropCompletionPercent + deltaS * 1f);

                cameraExtraOffsetZ = cameraDropAnimationAmount - cameraDropAnimationAmount * EaseFunctions.computeEaseValue("easeOutCubic", null, cameraDropCompletionPercent);
                posZ = posZ + cameraExtraOffsetZ;

                if(cameraDropCompletionPercent >= 1) {
                    isCameraDropActive = false;
                    cameraDropCompletionPercent = 0f;
                }
            }
            setCamera3DPosition(0f, playerUnit.pos.y - 10f, posZ);

            cameraFocusPosition.set(0, playerUnit.pos.y, -0.5f);

            setCamera3DLookAtTarget(cameraFocusPosition.x, cameraFocusPosition.y, cameraFocusPosition.z + cameraExtraOffsetZ);

            sunLight3D.setPosition(playerUnit.pos.x, playerUnit.pos.y, playerUnit.pos.z + 3f);
        }else {
            levelEndCameraDropPercent = Math.min(1, levelEndCameraDropPercent + deltaS);

            float cameraTargetOffsetZ = EaseFunctionCubic.easeOut(levelEndCameraDropPercent, 0, 3f, 1);

            setCamera3DLookAtTarget(cameraFocusPosition.x, cameraFocusPosition.y, cameraFocusPosition.z - cameraTargetOffsetZ);
        }
    }

}
