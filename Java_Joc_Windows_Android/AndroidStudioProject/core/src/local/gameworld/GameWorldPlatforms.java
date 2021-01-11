package local.gameworld;

import com.badlogic.gdx.graphics.g3d.attributes.ColorAttribute;
import com.badlogic.gdx.math.Vector3;

import java.util.Stack;

import base.ease.pennereasing.EaseFunctionCubic;
import base.profile.util.PlayerProfileDataEventListener;
import base.visual3d.ModelInstance3D;
import local.app.AppLocal;
import local.gameworld.util.PlatformConfig;
import local.gameworld.util.PlatformStyleConfig;
import local.gameworld.util.WorldPlatform;
import local.profile.PlayerProfileDataLocal;

public class GameWorldPlatforms extends GameWorldTutorialPanel {

    protected static final int TUBE_ROWS_COUNT = 8;

    protected Stack<WorldPlatform> worldPlatforms = new Stack<WorldPlatform>();

    protected final float tubeRadius = 3.2f;

    protected float tubeGlobalRotationDegreesY = 0;

    // This is used to increase the outline of far away platforms. It is the player position on the y axis.
    protected float platformsScaleOriginY = 0f;

    protected final float platformInAnimationRadius = 30f;

    private PlatformStyleConfig activePlatformStyle = null;

    private PlayerProfileDataEventListener playerProfileDataEventListener = new PlayerProfileDataEventListener() {
        @Override
        public void profileDataChanged(String dataID) {
            super.profileDataChanged(dataID);

            if(dataID == PlayerProfileDataLocal.ACTIVE_PLATFORM_STYLE_ID){
                activePlatformStyle = PlatformStyleConfig.entriesMap.get(app.profileData.getActivePlatformStyleId());

                updateAllPlatformStyles();
            }
        }
    };

    public GameWorldPlatforms(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        app.profileData.addEventsListener(playerProfileDataEventListener);

        activePlatformStyle = PlatformStyleConfig.entriesMap.get(app.profileData.getActivePlatformStyleId());
    }

    protected WorldPlatform createPlatform(PlatformConfig platformConfig, int tubeRowIndex, float posY, boolean doAnimation) {
        ModelInstance3D modelInstance = app.embeddedAssetsManager.getModelInstance(platformConfig.modelID);

        ModelInstance3D outlineModelInstance = app.embeddedAssetsManager.getModelInstance(platformConfig.outlineModelID);

        WorldPlatform platform = new WorldPlatform(platformConfig, modelInstance, outlineModelInstance, tubeRowIndex, posY);

        updatePlatformEntryStyle(platform);

        if(doAnimation) {
            platform.isInAnimation = true;
            platform.inAnimationCompletionPercent = 0f;
        }else {
            platform.isInAnimation = false;
            platform.inAnimationCompletionPercent = 0f;
        }

        updatePlatformTransform(platform);

        worldPlatforms.add(platform);

        stage3D.addChild(platform.modelInstance);
        stage3D.addChild(platform.outlineModelInstance);

        return platform;
    }

    private void updateAllPlatformStyles() {
        for(int i=0;i< worldPlatforms.size();i++) {
            WorldPlatform worldPlatform = worldPlatforms.get(i);
            if (worldPlatform != null) {
                updatePlatformEntryStyle(worldPlatform);
            }
        }
    }

    private void updatePlatformEntryStyle(WorldPlatform worldPlatform) {
        worldPlatform.modelInstance.materials.get(0).set(ColorAttribute.createDiffuse(activePlatformStyle.platformDiffuseColor));        worldPlatform.modelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
        worldPlatform.modelInstance.materials.get(0).set(sunLight3DAttribute);

        worldPlatform.outlineModelInstance.materials.get(0).set(ColorAttribute.createDiffuse(activePlatformStyle.outlineDiffuseColor));
        worldPlatform.outlineModelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
    }

    protected void setTubeRotation(float angDegrees) {
        tubeGlobalRotationDegreesY = angDegrees;

        updateAllPlatformsTransform();
    }

    private void updateAllPlatformsTransform() {
        for(int i=0;i< worldPlatforms.size();i++) {
            WorldPlatform platform = worldPlatforms.get(i);
            if (platform != null) {
                updatePlatformTransform(platform);
            }
        }
    }

    private void updatePlatformTransform(WorldPlatform platform) {
        float visualPosX, visualPosY, visualPosZ, angYRad;

        if(platform.isInAnimation) {
            angYRad = -(float)(-Math.PI / 2 + ((Math.PI * 2) / TUBE_ROWS_COUNT) * platform.tubeRowIndex) + (float)Math.toRadians(tubeGlobalRotationDegreesY);
            float easedValue = EaseFunctionCubic.easeOut(platform.inAnimationCompletionPercent, platformInAnimationRadius, -platformInAnimationRadius, 1);

            visualPosX = (float)Math.cos(angYRad) * (tubeRadius + platform.config.rendering3DSize.z / 2 + easedValue);
            visualPosZ = -(float)Math.sin(angYRad) * (tubeRadius + platform.config.rendering3DSize.z / 2 + easedValue);
        }else {
            angYRad = -(float)(-Math.PI / 2 + ((Math.PI * 2) / TUBE_ROWS_COUNT) * platform.tubeRowIndex) + (float)Math.toRadians(tubeGlobalRotationDegreesY);
            visualPosX = (float)Math.cos(angYRad) * (tubeRadius + platform.config.rendering3DSize.z / 2);
            visualPosZ = -(float)Math.sin(angYRad) * (tubeRadius + platform.config.rendering3DSize.z / 2);
        }

        float outlineScaleMultiplier = 1;
        if(platform.posY > platformsScaleOriginY) {
            float deltaY = platform.posY - platformsScaleOriginY;
            outlineScaleMultiplier = Math.min(1.3f, 1 + (deltaY / 40) * 0.3f);
        }

        visualPosY = platform.posY + platform.config.rendering3DSize.y / 2;
        float sX = platform.config.rendering3DSize.x / platform.config.model3DSize.x;
        float sY = platform.config.rendering3DSize.y / platform.config.model3DSize.y;
        float sZ = platform.config.rendering3DSize.z / platform.config.model3DSize.z;
        platform.modelInstance.transform
                .idt()
                .translate(visualPosX, visualPosY, visualPosZ)
                .rotate(Vector3.Y, (float) Math.toDegrees(angYRad) - 90)
                .scale(
                        sX,
                        sY,
                        sZ
                );
        platform.outlineModelInstance.transform
                .idt()
                .translate(visualPosX, visualPosY, visualPosZ)
                .rotate(Vector3.Y, (float) Math.toDegrees(angYRad) - 90).
                scale(
                        sX + (platform.config.outlineThickness * outlineScaleMultiplier) / platform.config.rendering3DSize.x,
                        sY + (platform.config.outlineThickness * outlineScaleMultiplier) / platform.config.rendering3DSize.y,
                        sZ + (platform.config.outlineThickness * outlineScaleMultiplier) / platform.config.rendering3DSize.z
                );
    }

    protected boolean isPlatformUnderPoint(int tubeRowIndex, float posY) {
        return getPlatformUnderPoint(tubeRowIndex, posY) != null;
    }

    protected WorldPlatform getPlatformUnderPoint(int tubeRowIndex, float posY) {
        for(int i=0;i< worldPlatforms.size();i++) {
            WorldPlatform platform = worldPlatforms.get(i);

            if (platform != null && !platform.isInAnimation && platform.tubeRowIndex == tubeRowIndex) {
                // Add a extra 0.2f so the collision box extends more on the closer side.
                if(posY >= platform.posY - 0.2f && posY <= platform.posY + platform.config.rendering3DSize.y) {
                    return platform;
                }
            }
        }

        return null;
    }

    protected void removePlatformsWhereBackLowerThan(float backPosY) {
        for(int i=0;i< worldPlatforms.size();i++) {
            WorldPlatform platform = worldPlatforms.get(i);

            if (platform != null && platform.posY + platform.config.rendering3DSize.y < backPosY) {
                worldPlatforms.remove(i);

                recyclePlatform(platform);
            }
        }
    }

    protected int getNextPlatformRow(int currentTubeRowIndice, float currentPosY) {
        return getNextPlatform(currentTubeRowIndice, currentPosY).tubeRowIndex;
    }

    protected WorldPlatform getNextPlatform(int currentTubeRowIndex, float currentPosY) {
        WorldPlatform currentPlatform = getPlatformUnderPoint(currentTubeRowIndex, currentPosY);

        WorldPlatform nextPlatform = null;
        for(int i=0;i< worldPlatforms.size();i++) {
            WorldPlatform platform = worldPlatforms.get(i);
            if (platform != null) {
                if(platform.posY > currentPlatform.posY && (nextPlatform == null || platform.posY < nextPlatform.posY)) {
                    nextPlatform = platform;
                }
            }
        }

        return nextPlatform;
    }

    @Override
    protected void gameLoop(float deltaS, int deltaMS) {
        super.gameLoop(deltaS, deltaMS);

        for(int i=0;i< worldPlatforms.size();i++) {
            WorldPlatform platform = worldPlatforms.get(i);
            if (platform != null && platform.isInAnimation) {
                platform.inAnimationCompletionPercent = Math.min(1, platform.inAnimationCompletionPercent + deltaS * 2);

                if(platform.inAnimationCompletionPercent >= 1) {
                    // In animation finished.

                    platform.isInAnimation = false;
                    platform.inAnimationCompletionPercent = 0;
                }

                updatePlatformTransform(platform);
            }
        }
    }

    private void recyclePlatform(WorldPlatform worldPlatform) {
        if(worldPlatform.modelInstance != null) {
            stage3D.removeChild(worldPlatform.modelInstance);

            app.embeddedAssetsManager.releaseModelInstance(worldPlatform.modelInstance);

            worldPlatform.modelInstance = null;
        }

        if(worldPlatform.outlineModelInstance != null) {
            stage3D.removeChild(worldPlatform.outlineModelInstance);

            app.embeddedAssetsManager.releaseModelInstance(worldPlatform.outlineModelInstance);

            worldPlatform.outlineModelInstance = null;
        }
    }

    @Override
    public void dispose() {
        app.profileData.removeEventsListener(playerProfileDataEventListener);

        while(worldPlatforms.size() > 0) {
            WorldPlatform worldPlatform = worldPlatforms.pop();
            if(worldPlatform != null) {
                recyclePlatform(worldPlatform);
            }
        }

        super.dispose();
    }

}
