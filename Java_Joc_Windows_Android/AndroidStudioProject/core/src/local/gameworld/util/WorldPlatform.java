package local.gameworld.util;

import base.visual3d.ModelInstance3D;

public class WorldPlatform {

    public PlatformConfig config;

    public ModelInstance3D modelInstance;
    public ModelInstance3D outlineModelInstance;

    public int tubeRowIndex;

    public float posY;

    public boolean isInAnimation = false;
    public float inAnimationCompletionPercent = 0f;

    public WorldPlatform(PlatformConfig config, ModelInstance3D modelInstance, ModelInstance3D outlineModelInstance, int tubeRowIndex, float posY) {
        super();

        this.config = config;

        this.modelInstance = modelInstance;
        this.outlineModelInstance = outlineModelInstance;

        this.tubeRowIndex = tubeRowIndex;

        this.posY = posY;
    }

}
