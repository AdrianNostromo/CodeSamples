package local.gameworld.util;

import com.badlogic.gdx.graphics.Color;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class BallStyleConfig {

    public static final Map<String, BallStyleConfig> entriesMap = new HashMap<String, BallStyleConfig>();
    public static final Vector<BallStyleConfig> entriesList = new Vector<BallStyleConfig>();

    public static final BallStyleConfig Style1 = new BallStyleConfig(
            "Style1",
            "visual/taTextures.atlas", "modelTextures/balls/style1",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style2 = new BallStyleConfig(
            "Style2",
            "visual/taTextures.atlas", "modelTextures/balls/style2",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style3 = new BallStyleConfig(
            "Style3",
            "visual/taTextures.atlas", "modelTextures/balls/style3",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style4 = new BallStyleConfig(
            "Style4",
            "visual/taTextures.atlas", "modelTextures/balls/style4",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style5 = new BallStyleConfig(
            "Style5",
            "visual/taTextures.atlas", "modelTextures/balls/style5",
            Color.valueOf("#000000ff")
    );

    public static final BallStyleConfig Style6 = new BallStyleConfig(
            "Style6",
            "visual/taTextures.atlas", "modelTextures/balls/style6",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style7 = new BallStyleConfig(
            "Style7",
            "visual/taTextures.atlas", "modelTextures/balls/style7",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style8 = new BallStyleConfig(
            "Style8",
            "visual/taTextures.atlas", "modelTextures/balls/style8",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style9 = new BallStyleConfig(
            "Style9",
            "visual/taTextures.atlas", "modelTextures/balls/style9",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style10 = new BallStyleConfig(
            "Style10",
            "visual/taTextures.atlas", "modelTextures/balls/style10",
            Color.valueOf("#000000ff")
    );

    public static final BallStyleConfig Style11 = new BallStyleConfig(
            "Style11",
            "visual/taTextures.atlas", "modelTextures/balls/style11",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style12 = new BallStyleConfig(
            "Style12",
            "visual/taTextures.atlas", "modelTextures/balls/style12",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style13 = new BallStyleConfig(
            "Style13",
            "visual/taTextures.atlas", "modelTextures/balls/style13",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style14 = new BallStyleConfig(
            "Style14",
            "visual/taTextures.atlas", "modelTextures/balls/style14",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style15 = new BallStyleConfig(
            "Style15",
            "visual/taTextures.atlas", "modelTextures/balls/style15",
            Color.valueOf("#000000ff")
    );

    public static final BallStyleConfig Style16 = new BallStyleConfig(
            "Style16",
            "visual/taTextures.atlas", "modelTextures/balls/style16",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style17 = new BallStyleConfig(
            "Style17",
            "visual/taTextures.atlas", "modelTextures/balls/style17",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style18 = new BallStyleConfig(
            "Style18",
            "visual/taTextures.atlas", "modelTextures/balls/style18",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style19 = new BallStyleConfig(
            "Style19",
            "visual/taTextures.atlas", "modelTextures/balls/style19",
            Color.valueOf("#000000ff")
    );
    public static final BallStyleConfig Style20 = new BallStyleConfig(
            "Style20",
            "visual/taTextures.atlas", "modelTextures/balls/style20",
            Color.valueOf("#000000ff")
    );

    public static final BallStyleConfig DefaultStyle = Style1;

    public String styleID;

    public String textureAtlasID;
    public String textureRegionID;

    public Color outlineDiffuseColor;

    public BallStyleConfig(String styleID, String textureAtlasID, String textureRegionID, Color outlineDiffuseColor) {
        super();

        this.styleID = styleID;

        this.textureAtlasID = textureAtlasID;
        this.textureRegionID = textureRegionID;

        this.outlineDiffuseColor = outlineDiffuseColor;

        entriesMap.put(styleID, this);
        entriesList.add(this);
    }

}
