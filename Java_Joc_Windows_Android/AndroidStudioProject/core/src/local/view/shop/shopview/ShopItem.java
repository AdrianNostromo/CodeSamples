package local.view.shop.shopview;

import com.badlogic.gdx.math.Vector3;

import base.visual.IContainer2D;
import base.visual.Sprite2D;
import base.visual3d.ModelInstance3D;
import local.gameworld.util.BallConfig;
import local.gameworld.util.PlatformConfig;

public class ShopItem {

    public ShopItemConfig config;

    private PlatformConfig platformConfig;
    private BallConfig ballConfig;

    public IContainer2D container2D;
    public Sprite2D touchImage;
    public Sprite2D imgLockTooExpensive;
    public Sprite2D imgLockUnlockable;
    public Sprite2D unlockCostImage;

    private ModelInstance3D platformModelInstance;
    private ModelInstance3D platformOutlineModelInstance;

    private ModelInstance3D unitModelInstance;
    private ModelInstance3D unitOutlineModelInstance;
    private ModelInstance3D unitShadowModelInstance;

    public float platformScale;
    public float unitScale;

    private Vector3 pos3D = new Vector3(0f, 0f, 0f);
    private float ang3DRX = 0f;

    public ShopItem(ShopItemConfig config, PlatformConfig platformConfig, BallConfig ballConfig, IContainer2D container2D, Sprite2D touchImage, Sprite2D imgLockTooExpensive, Sprite2D imgLockUnlockable, Sprite2D unlockCostImage, ModelInstance3D platformModelInstance, ModelInstance3D platformOutlineModelInstance, ModelInstance3D unitModelInstance, ModelInstance3D unitOutlineModelInstance, ModelInstance3D unitShadowModelInstance, float platformScale, float unitScale) {
        super();

        this.config = config;

        this.platformConfig = platformConfig;
        this.ballConfig = ballConfig;

        this.container2D = container2D;
        this.touchImage = touchImage;
        this.imgLockTooExpensive = imgLockTooExpensive;
        this.imgLockUnlockable = imgLockUnlockable;
        this.unlockCostImage = unlockCostImage;

        this.platformModelInstance = platformModelInstance;
        this.platformOutlineModelInstance = platformOutlineModelInstance;

        this.unitModelInstance = unitModelInstance;
        this.unitOutlineModelInstance = unitOutlineModelInstance;
        this.unitShadowModelInstance = unitShadowModelInstance;

        this.platformScale = platformScale;
        this.unitScale = unitScale;
    }

    public ModelInstance3D getPlatformModelInstance() {
        return platformModelInstance;
    }

    public ModelInstance3D getPlatformOutlineModelInstance() {
        return platformOutlineModelInstance;
    }

    public ModelInstance3D getUnitModelInstance() {
        return unitModelInstance;
    }

    public ModelInstance3D getUnitOutlineModelInstance() {
        return unitOutlineModelInstance;
    }

    public ModelInstance3D getUnitShadowModelInstance() {
        return unitShadowModelInstance;
    }

    public float getAng3DRX() {
        return ang3DRX;
    }

    public void setAng3DRX(float ang3DRX, boolean doSyncVisuals) {
        this.ang3DRX = ang3DRX;

        if(doSyncVisuals) {
            syncVisuals();
        }
    }

    public Vector3 getPos3D() {
        return pos3D;
    }

    public void setPos3D(float x, float y, float z, boolean doSyncVisuals) {
        this.pos3D.set(x, y, z);

        if(doSyncVisuals) {
            syncVisuals();
        }
    }

    public void syncVisuals() {
        platformModelInstance.transform
                .idt()
                .translate(
                        pos3D.x,
                        pos3D.y,
                        pos3D.z
                )
                .rotate(Vector3.X, 14)
                .scale(
                        platformScale,
                        platformScale,
                        platformScale
                );
    
        float platformOutlineScaleMultiplier = 2.2f;
        platformOutlineModelInstance.transform
                .idt()
                .translate(
                        pos3D.x,
                        pos3D.y,
                        pos3D.z
                )
                .rotate(Vector3.X, 14)
                .scale(
                        platformScale + (platformConfig.outlineThickness * platformOutlineScaleMultiplier) / (platformConfig.rendering3DSize.x * platformScale),
                        platformScale + (platformConfig.outlineThickness * platformOutlineScaleMultiplier) / (platformConfig.rendering3DSize.y * platformScale),
                        platformScale + (platformConfig.outlineThickness * platformOutlineScaleMultiplier) / (platformConfig.rendering3DSize.z * platformScale)
                );
    
        unitModelInstance.transform
                .idt()
                .translate(
                        pos3D.x,
                        pos3D.y,
                        pos3D.z + 1.1f
                )
                .rotate(Vector3.X, ang3DRX)
                .scale(
                        unitScale,
                        unitScale,
                        unitScale
                );
    
        float unitOutlineScaleMultiplier = 5.0f;
        // Override the value from (ballConfig.outlineThickness) so the shop is not affected by in game thickness changes.
        float unitOutlineThicknessOverride = 0.06f;
        unitOutlineModelInstance.transform
                .idt()
                .translate(
                        pos3D.x,
                        pos3D.y,
                        pos3D.z + 1.1f
                )
                .scale(
                        unitScale + (unitOutlineThicknessOverride * unitOutlineScaleMultiplier) / (ballConfig.rendering3DSize.x * unitScale),
                        unitScale + (unitOutlineThicknessOverride * unitOutlineScaleMultiplier) / (ballConfig.rendering3DSize.y * unitScale),
                        unitScale + (unitOutlineThicknessOverride * unitOutlineScaleMultiplier) / (ballConfig.rendering3DSize.z * unitScale)
                );
    
        unitShadowModelInstance.transform
                .idt()
                .translate(
                        pos3D.x,
                        pos3D.y,
                        pos3D.z + 1f + ballConfig.shadowOffsetZ * unitScale * 1.0f
                )
                .scale(
                        unitScale * ballConfig.shadowScale,
                        unitScale * ballConfig.shadowScale,
                        unitScale * ballConfig.shadowScale
                )
                .rotate(Vector3.X, 14);
    }

}
