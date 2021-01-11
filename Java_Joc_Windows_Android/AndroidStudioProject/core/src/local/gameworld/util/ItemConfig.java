package local.gameworld.util;

import com.badlogic.gdx.math.Vector3;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class ItemConfig {

    public static final Map<String, ItemConfig> entriesMap = new HashMap<String, ItemConfig>();
    public static final Vector<ItemConfig> entriesList = new Vector<ItemConfig>();

    public static final ItemConfig PointItemA = new ItemConfig(
            "CollectibleSphere",
            "visual3D/items/PointItemA.g3db", "visual3D/items/PointItemA_Outline.g3db",
            0.06f, 0.3f, new Vector3(0.5f, 0.5f, 0.5f)
    );

    public String itemID;

    public String modelID;

    public String outlineModelID;
    public float outlineThickness;

    public float collisionRadius;

    public Vector3 size;

    public ItemConfig(String itemID, String modelID, String outlineModelID, float outlineThickness, float collisionRadius, Vector3 size) {
        super();

        this.itemID = itemID;

        this.modelID = modelID;

        this.outlineModelID = outlineModelID;
        this.outlineThickness = outlineThickness;

        this.collisionRadius = collisionRadius;

        this.size = size;

        entriesMap.put(itemID, this);
        entriesList.add(this);
    }

}
