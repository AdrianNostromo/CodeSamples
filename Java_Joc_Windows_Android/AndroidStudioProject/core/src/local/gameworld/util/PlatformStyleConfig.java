package local.gameworld.util;

import com.badlogic.gdx.graphics.Color;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class PlatformStyleConfig {

    public static final Map<String, PlatformStyleConfig> entriesMap = new HashMap<String, PlatformStyleConfig>();
    public static final Vector<PlatformStyleConfig> entriesList = new Vector<PlatformStyleConfig>();

    public static final PlatformStyleConfig Style1 = new PlatformStyleConfig(
            "Style1",
            Color.valueOf("#ffeb2cff"),
            Color.valueOf("#776a15ff")
    );
    public static final PlatformStyleConfig Style2 = new PlatformStyleConfig(
            "Style2",
            Color.valueOf("#f32a00ff"),
            Color.valueOf("#792715ff")
    );

    public static final PlatformStyleConfig Style3 = new PlatformStyleConfig(
            "Style3",
            Color.valueOf("#da0000ff"),
            Color.valueOf("#861818ff")
    );
    public static final PlatformStyleConfig Style4 = new PlatformStyleConfig(
            "Style4",
            Color.valueOf("#b0002cff"),
            Color.valueOf("#5a1829ff")
    );
    public static final PlatformStyleConfig Style5 = new PlatformStyleConfig(
            "Style5",
            Color.valueOf("#b00067ff"),
            Color.valueOf("#631543ff")
    );
    public static final PlatformStyleConfig Style6 = new PlatformStyleConfig(
            "Style6",
            Color.valueOf("#6a135dff"),
            Color.valueOf("#3b1535ff")
    );
    public static final PlatformStyleConfig Style7 = new PlatformStyleConfig(
            "Style7",
            Color.valueOf("#4d136aff"),
            Color.valueOf("#31173dff")
    );

    public static final PlatformStyleConfig Style8 = new PlatformStyleConfig(
            "Style8",
            Color.valueOf("#60398aff"),
            Color.valueOf("#3c2e4cff")
    );
    public static final PlatformStyleConfig Style9 = new PlatformStyleConfig(
            "Style9",
            Color.valueOf("#4c398aff"),
            Color.valueOf("#3c325aff")
    );
    public static final PlatformStyleConfig Style10 = new PlatformStyleConfig(
            "Style10",
            Color.valueOf("#0000dcff"),
            Color.valueOf("#18184eff")
    );
    public static final PlatformStyleConfig Style11 = new PlatformStyleConfig(
            "Style11",
            Color.valueOf("#3bfdfdff"),
            Color.valueOf("#0c9eb3ff")
    );
    public static final PlatformStyleConfig Style12 = new PlatformStyleConfig(
            "Style12",
            Color.valueOf("#00817dff"),
            Color.valueOf("#144443ff")
    );
    public static final PlatformStyleConfig Style13 = new PlatformStyleConfig(
            "Style13",
            Color.valueOf("#008134ff"),
            Color.valueOf("#18462aff")
    );
    public static final PlatformStyleConfig Style14 = new PlatformStyleConfig(
            "Style14",
            Color.valueOf("#248e23ff"),
            Color.valueOf("#30592fff")
    );
    public static final PlatformStyleConfig Style15 = new PlatformStyleConfig(
            "Style15",
            Color.valueOf("#538e23ff"),
            Color.valueOf("#3b5329ff")
    );

    public static final PlatformStyleConfig Style16 = new PlatformStyleConfig(
            "Style16",
            Color.valueOf("#afd510ff"),
            Color.valueOf("#768a20ff")
    );
    public static final PlatformStyleConfig Style17 = new PlatformStyleConfig(
            "Style17",
            Color.valueOf("#ffffffff"),
            Color.valueOf("#000000ff")
    );
    public static final PlatformStyleConfig Style18 = new PlatformStyleConfig(
            "Style18",
            Color.valueOf("#ffca2cff"),
            Color.valueOf("#a6841eff")
    );

    public static final PlatformStyleConfig Style19 = new PlatformStyleConfig(
            "Style19",
            Color.valueOf("#ff7917ff"),
            Color.valueOf("#90511cff")
    );
    public static final PlatformStyleConfig Style20 = new PlatformStyleConfig(
            "Style20",
            Color.valueOf("#000000ff"),
            Color.valueOf("#ffffffff")
    );

    public static final PlatformStyleConfig DefaultStyle = Style1;

    public String styleID;

    public Color platformDiffuseColor;
    public Color outlineDiffuseColor;

    public PlatformStyleConfig(String styleID, Color platformDiffuseColor, Color outlineDiffuseColor) {
        super();

        this.styleID = styleID;

        this.platformDiffuseColor = platformDiffuseColor;
        this.outlineDiffuseColor = outlineDiffuseColor;

        entriesMap.put(styleID, this);
        entriesList.add(this);
    }

}
