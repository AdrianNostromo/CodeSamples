package local.gameworld.util;

import com.badlogic.gdx.math.Vector3;

import base.visual3d.ModelInstance3D;

public class WorldUnit {

    public BallConfig config;

    public ModelInstance3D modelInstance;
    public ModelInstance3D outlineModelInstance;
    public ModelInstance3D shadowModelInstance;

    public int tubeRowIndex;

    public Vector3 pos;
    // The angle on the x axis.
    public float angRX = 0f;

    public float jumpOffsetZ = 0;

    public boolean hasFallenOfPlatform = false;
    public float fallenOffPlatformShadowHidePercent = 0f;

    public WorldUnit(BallConfig config, ModelInstance3D modelInstance, ModelInstance3D outlineModelInstance, ModelInstance3D shadowModelInstance, int tubeRowIndex, Vector3 pos) {
        super();

        this.config = config;

        this.modelInstance = modelInstance;
        this.outlineModelInstance = outlineModelInstance;
        this.shadowModelInstance = shadowModelInstance;

        this.tubeRowIndex = tubeRowIndex;

        this.pos = pos;
    }

    public void syncVisuals() {
        float sX = config.rendering3DSize.x / config.model3DSize.x;
        float sY = config.rendering3DSize.y / config.model3DSize.y;
        float sZ = config.rendering3DSize.z / config.model3DSize.z;
        modelInstance.transform
                .idt()
                .translate(pos)
                .rotate(Vector3.X, angRX)
                .scale(
                        sX,
                        sY,
                        sZ
                );

        outlineModelInstance.transform
                .idt()
                .translate(pos)
                .scale(
                        sX + config.outlineThickness / config.rendering3DSize.x,
                        sY + config.outlineThickness / config.rendering3DSize.y,
                        sZ + config.outlineThickness / config.rendering3DSize.z
                );

        float finalShadowScale = config.shadowScale;
        if(jumpOffsetZ > 0) {
            finalShadowScale = finalShadowScale * (1 - Math.min(1, jumpOffsetZ / 1f));
        }
        if(hasFallenOfPlatform) {
            // This will fade out in 20 frames.
            fallenOffPlatformShadowHidePercent = Math.min(1, fallenOffPlatformShadowHidePercent + 1f / 20f);

            finalShadowScale = finalShadowScale * (1 - fallenOffPlatformShadowHidePercent);
        }

        shadowModelInstance.transform
                .idt()
                .translate(pos.x, pos.y, pos.z + config.shadowOffsetZ)
                .scale(
                    finalShadowScale,
                    finalShadowScale,
                    finalShadowScale
                );
    }

}
