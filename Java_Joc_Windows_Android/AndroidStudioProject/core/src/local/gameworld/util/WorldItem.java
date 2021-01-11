package local.gameworld.util;

import com.badlogic.gdx.math.Vector3;

import base.visual3d.ModelInstance3D;

public class WorldItem {

    public ItemConfig config;

    public ModelInstance3D modelInstance;
    public ModelInstance3D outlineModelInstance;

    public int tubeRowIndex;
    // This is the y offset on the row, it never changes.
    public float rowOffsetY;

    public boolean isInAnimation = false;
    public float inAnimationCompletionPercent = 0f;

    public float angYRad = 0;
    // This is the actual 3d position of the 3d model. It is updated when the 3d mesh moves.
    public Vector3 pos = new Vector3(0, 0, 0);

    public WorldItem(ItemConfig config, ModelInstance3D modelInstance, ModelInstance3D outlineModelInstance, int tubeRowIndex, float rowOffsetY) {
        super();

        this.config = config;

        this.modelInstance = modelInstance;
        this.outlineModelInstance = outlineModelInstance;

        this.tubeRowIndex = tubeRowIndex;

        this.rowOffsetY = rowOffsetY;
    }

}
