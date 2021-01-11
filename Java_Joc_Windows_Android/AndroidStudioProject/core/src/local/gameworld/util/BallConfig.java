package local.gameworld.util;

import com.badlogic.gdx.math.Vector3;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class BallConfig {

    public static final Map<String, BallConfig> entriesMap = new HashMap<String, BallConfig>();
    public static final Vector<BallConfig> entriesList = new Vector<BallConfig>();

    //asdqqqq;// refactor all assets to use the new managed ones for auto-preloading.

    public static final BallConfig PlayerSphereA = new BallConfig(
            "PlayerSphereA",
            "visual3D/units/PlayerSphereA.g3db",
            "visual3D/units/PlayerSphereA_Outline.g3db", 0.08f,
            0.3f, 0.25f, new Vector3(0.5f, 0.5f, 0.5f), new Vector3(0.5f, 0.5f, 0.5f),
            "visual3D/units/ShadowPlane.g3db", "visual/taTextures.atlas", "modelTextures/balls/ballShadow", 1.0f, -0.25f + 0.01f);

    public String unitID;

    public String modelID;

    public String outlineModelID;
    public float outlineThickness;

    public float collisionRadius;
    public float groundElevation;

    public Vector3 model3DSize;
    public Vector3 rendering3DSize;

    public String shadowModelID;
    public String shadowTextureAtlasID;
    public String shadowTextureRegionID;
    public float shadowScale;
    public float shadowOffsetZ;

    public BallConfig(String unitID, String modelID, String outlineModelID, float outlineThickness, float collisionRadius, float groundElevation, Vector3 model3DSize, Vector3 rendering3DSize, String shadowModelID, String shadowTextureAtlasID, String shadowTextureRegionID, float shadowScale, float shadowOffsetZ) {
        super();

        this.unitID = unitID;

        this.modelID = modelID;

        this.outlineModelID = outlineModelID;
        this.outlineThickness = outlineThickness;

        this.collisionRadius = collisionRadius;

        this.groundElevation = groundElevation;

        this.model3DSize = model3DSize;
        this.rendering3DSize = rendering3DSize;

        this.shadowModelID = shadowModelID;
        this.shadowTextureAtlasID = shadowTextureAtlasID;
        this.shadowTextureRegionID = shadowTextureRegionID;
        this.shadowScale = shadowScale;
        this.shadowOffsetZ = shadowOffsetZ;

        entriesMap.put(unitID, this);
        entriesList.add(this);
    }

}
