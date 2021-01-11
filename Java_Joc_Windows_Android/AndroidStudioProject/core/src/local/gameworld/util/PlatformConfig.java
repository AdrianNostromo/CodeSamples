package local.gameworld.util;

import com.badlogic.gdx.math.Vector3;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class PlatformConfig {

    public static final Map<String, PlatformConfig> entriesMap = new HashMap<String, PlatformConfig>();
    public static final Vector<PlatformConfig> entriesList = new Vector<PlatformConfig>();

    public static final PlatformConfig Platform_ShopPreview = new PlatformConfig(
            "Platform_ShopPreview",
            "visual3D/platforms/Platform_ShopPreview.g3db",
            "visual3D/platforms/Platform_ShopPreview_Outline.g3db",
            0.09f,
            new Vector3(2f, 2f, 0.7f),
            new Vector3(2f, 2f, 0.7f)
    );

    public static final PlatformConfig Platform_2x2 = new PlatformConfig(
            "Platform_2x2",
            "visual3D/platforms/Platform_2x2.g3db",
            "visual3D/platforms/Platform_2x2_Outline.g3db",
            0.11f,
            new Vector3(2f, 2f, 0.35f),
            new Vector3(2f, 2f, 0.35f)
    );
    public static final PlatformConfig Platform_2x3 = new PlatformConfig(
            "Platform_2x3",
            "visual3D/platforms/Platform_2x3.g3db",
            "visual3D/platforms/Platform_2x3_Outline.g3db",
            0.11f,
            new Vector3(2f, 3f, 0.35f),
            new Vector3(2f, 3f, 0.35f)
    );
    public static final PlatformConfig Platform_2x4 = new PlatformConfig(
            "Platform_2x4",
            "visual3D/platforms/Platform_2x4.g3db",
            "visual3D/platforms/Platform_2x4_Outline.g3db",
            0.11f,
            new Vector3(2f, 4f, 0.35f),
            new Vector3(2f, 4f, 0.35f)
    );
    public static final PlatformConfig Platform_2x6 = new PlatformConfig(
            "Platform_2x6",
            "visual3D/platforms/Platform_2x6.g3db",
            "visual3D/platforms/Platform_2x6_Outline.g3db",
            0.11f,
            new Vector3(2f, 6f, 0.35f),
            new Vector3(2f, 6f, 0.35f)
    );

    public String platformID;

    public String modelID;

    public String outlineModelID;
    public float outlineThickness;

    public Vector3 model3DSize;
    public Vector3 rendering3DSize;

    public PlatformConfig(String platformID, String modelID, String outlineModelID, float outlineThickness, Vector3 model3DSize, Vector3 rendering3DSize) {
        super();

        this.platformID = platformID;

        this.modelID = modelID;

        this.outlineModelID = outlineModelID;
        this.outlineThickness = outlineThickness;

        this.model3DSize = model3DSize;
        this.rendering3DSize = rendering3DSize;

        entriesMap.put(platformID, this);
        entriesList.add(this);
    }

}
