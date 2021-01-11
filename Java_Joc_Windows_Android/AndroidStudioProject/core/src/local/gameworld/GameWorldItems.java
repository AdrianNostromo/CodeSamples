package local.gameworld;

import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.g3d.attributes.ColorAttribute;
import com.badlogic.gdx.math.Vector3;

import java.util.Stack;

import base.ease.pennereasing.EaseFunctionCubic;
import base.visual3d.ModelInstance3D;
import local.app.AppLocal;
import local.fx.config.FXConfig;
import local.fx.config.VFXGroupIDs;
import local.gameworld.util.ItemConfig;
import local.gameworld.util.WorldItem;

public class GameWorldItems extends GameWorldPlatforms {

    protected Stack<WorldItem> worldItems = new Stack<WorldItem>();

    public GameWorldItems(AppLocal app) {
        super(app);

        //void
    }

    protected WorldItem createItem(ItemConfig itemConfig, int tubeRowIndex, float posY, boolean doAnimation) {
        ModelInstance3D modelInstance = app.embeddedAssetsManager.getModelInstance(itemConfig.modelID);
        modelInstance.materials.get(0).set(ColorAttribute.createDiffuse(Color.RED));
        modelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));
        modelInstance.materials.get(0).set(sunLight3DAttribute);

        ModelInstance3D outlineModelInstance = app.embeddedAssetsManager.getModelInstance(itemConfig.outlineModelID);
        outlineModelInstance.materials.get(0).set(ColorAttribute.createDiffuse(Color.BLACK));
        outlineModelInstance.materials.get(0).set(new ColorAttribute(ColorAttribute.AmbientLight, 0.7f, 0.7f, 0.7f, 1.0f));

        WorldItem item = new WorldItem(itemConfig, modelInstance, outlineModelInstance, tubeRowIndex, posY);

        if(doAnimation) {
            item.isInAnimation = true;
            item.inAnimationCompletionPercent = 0f;
        }else {
            item.isInAnimation = false;
            item.inAnimationCompletionPercent = 0f;
        }

        updateItemTransform(item);

        worldItems.add(item);

        stage3D.addChild(item.modelInstance);
        stage3D.addChild(item.outlineModelInstance);

        return item;
    }

    @Override
    protected void setTubeRotation(float angDegrees) {
        super.setTubeRotation(angDegrees);

        updateAllItemsTransform();
    }

    private void updateAllItemsTransform() {
        for(int i=0;i< worldItems.size();i++) {
            WorldItem item = worldItems.get(i);
            if (item != null) {
                updateItemTransform(item);
            }
        }
    }

    private void updateItemTransform(WorldItem item) {
        float posX, posY, posZ, angYRad;

        if(item.isInAnimation) {
            angYRad = -(float)(-Math.PI / 2 + ((Math.PI * 2) / TUBE_ROWS_COUNT) * item.tubeRowIndex) + (float)Math.toRadians(tubeGlobalRotationDegreesY);
            float easedValue = EaseFunctionCubic.easeOut(item.inAnimationCompletionPercent, platformInAnimationRadius, -platformInAnimationRadius, 1);

            posX = (float)Math.cos(angYRad) * ((tubeRadius - item.config.collisionRadius) + easedValue);
            posY = item.rowOffsetY;
            posZ = -(float)Math.sin(angYRad) * ((tubeRadius - item.config.collisionRadius) + easedValue);
        }else {
            angYRad = -(float)(-Math.PI / 2 + ((Math.PI * 2) / TUBE_ROWS_COUNT) * item.tubeRowIndex) + (float)Math.toRadians(tubeGlobalRotationDegreesY);
            posX = (float)Math.cos(angYRad) * (tubeRadius - item.config.collisionRadius);
            posY = item.rowOffsetY;
            posZ = -(float)Math.sin(angYRad) * (tubeRadius - item.config.collisionRadius);
        }

        item.modelInstance.transform.idt().translate(posX, posY, posZ).rotate(Vector3.Y, (float) Math.toDegrees(angYRad) - 90);
        item.outlineModelInstance.transform.idt().translate(posX, posY, posZ).rotate(Vector3.Y, (float) Math.toDegrees(angYRad) - 90).scale(1 + item.config.outlineThickness / item.config.size.x, 1 + item.config.outlineThickness / item.config.size.y, 1 + item.config.outlineThickness / item.config.size.z);

        item.pos.set(posX, posY, posZ);
        item.angYRad = angYRad;
    }

    protected void removeItemsWhereBackLowerThan(float backPosY) {
        for(int i=0;i< worldItems.size();i++) {
            WorldItem item = worldItems.get(i);

            if (item != null && item.rowOffsetY + item.config.collisionRadius < backPosY) {
                worldItems.remove(i);

                recycleItem(item);
            }
        }
    }

    @Override
    protected void gameLoop(float deltaS, int deltaMS) {
        super.gameLoop(deltaS, deltaMS);

        for(int i=0;i< worldItems.size();i++) {
            WorldItem item = worldItems.get(i);
            if (item != null && item.isInAnimation) {
                item.inAnimationCompletionPercent = Math.min(1, item.inAnimationCompletionPercent + deltaS * 2);

                if(item.inAnimationCompletionPercent >= 1) {
                    // In animation finished.

                    item.isInAnimation = false;
                    item.inAnimationCompletionPercent = 0;
                }

                updateItemTransform(item);
            }
        }
    }

    private void recycleItem(WorldItem worldItem) {
        if(worldItem.modelInstance != null) {
            stage3D.removeChild(worldItem.modelInstance);

            app.embeddedAssetsManager.releaseModelInstance(worldItem.modelInstance);

            worldItem.modelInstance = null;
        }
        if(worldItem.outlineModelInstance != null) {
            stage3D.removeChild(worldItem.outlineModelInstance);

            app.embeddedAssetsManager.releaseModelInstance(worldItem.outlineModelInstance);

            worldItem.outlineModelInstance = null;
        }
    }

    protected WorldItem getCollidingItemToCollect(Vector3 targetPosition, float targetCollisionRadius) {
        for(int i=0;i< worldItems.size();i++) {
            WorldItem item = worldItems.get(i);
            if (item != null) {
                float dist = targetPosition.dst(item.pos);
                if(dist <= item.config.collisionRadius + targetCollisionRadius) {
                    return item;
                }
            }
        }

        return null;
    }

    protected void collectItem(WorldItem item) {
        // Remove item from the list.
        int i = worldItems.indexOf(item);
        worldItems.remove(i);

        recycleItem(item);

        // Show a visual fx particle.
        app.fxManager.playFX(FXConfig.itemCollected.fxId, layerVFX, app.templateSize.width * 0.5f, app.templateSize.height * 0.38f, VFXGroupIDs.IN_GAME);
    }

    @Override
    public void dispose() {
        while(worldItems.size() > 0) {
            WorldItem worldItem = worldItems.pop();
            if(worldItem != null) {
                recycleItem(worldItem);
            }
        }

        super.dispose();
    }

}
