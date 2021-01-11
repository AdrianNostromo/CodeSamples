package local.playgames.util;

/**
 * Created by Adrin on 8/7/2016.
 */
public class ConnectedPlayerData {

    public String playerId;

    public String displayName;
    public String playerTitle;
    public int levelNumber;

    // Async data, it is populated when data is available.
    public int[] iconImagePixelsARGB8888 = null;
    public int iconImageWidth;
    public int iconImageHeight;

    public ConnectedPlayerData(String playerId, String displayName, String playerTitle, int levelNumber) {
        super();

        this.playerId = playerId;

        this.displayName = displayName;
        this.playerTitle = playerTitle;
        this.levelNumber = levelNumber;
    }

}
