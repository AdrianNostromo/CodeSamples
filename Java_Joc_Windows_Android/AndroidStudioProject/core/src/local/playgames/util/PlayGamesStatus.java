package local.playgames.util;

/**
 * Created by Adrin on 8/7/2016.
 */
public class PlayGamesStatus {

    public static final PlayGamesStatus OFFLINE = new PlayGamesStatus(1);
    public static final PlayGamesStatus CONNECTING = new PlayGamesStatus(2);
    public static final PlayGamesStatus LOGGED_IN = new PlayGamesStatus(3);

    public int id;

    public PlayGamesStatus(int id) {
        super();

        this.id = id;
    }
}
