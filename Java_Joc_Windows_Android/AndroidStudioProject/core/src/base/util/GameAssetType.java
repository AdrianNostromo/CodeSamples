package base.util;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by Adrin on 8/29/2016.
 */
public class GameAssetType {

    public static final Map<String, GameAssetType> entriesMap = new HashMap<String, GameAssetType>();

    // NOTE : These types are mirrored on the server. When editing, also update the server ones.
    // NOTE : Don't change the assets fxId's, when new ones are required, append them.

    public static final GameAssetType PLAYER = new GameAssetType(1, "PLAYER", "Player", "player");
    public static final GameAssetType PLATFORM = new GameAssetType(2, "PLATFORM", "Platform", "platform");
    public static final GameAssetType BACKGROUND = new GameAssetType(3, "BACKGROUND", "Background", "background");
    public static final GameAssetType SPIKE = new GameAssetType(4, "SPIKE", "Spike", "spike");
    public static final GameAssetType TEXT = new GameAssetType(5, "TEXT", "Text", "text");
    public static final GameAssetType GEM = new GameAssetType(6, "GEM", "Gem", "gem");
    public static final GameAssetType THEME = new GameAssetType(7, "THEME", "Theme", "theme");

    public int id;

    public String name;

    public String displayName;

    public String iconID;

    public GameAssetType(int id, String name, String displayName, String iconID) {
        super();

        this.id = id;

        this.name = name;

        this.displayName = displayName;

        this.iconID = iconID;

        entriesMap.put(Integer.toString(id), this);
    }

}
