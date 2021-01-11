package local.playgames.util;

// Interface for the local data handler.
public class PlayGamesEventListener {

    public PlayGamesEventListener() {
        super();

        //void
    }

    // States are: offline, connecting, logged_in
    public void playGamesStatusChanged(PlayGamesStatus status) {
        //void
    }

    public void playerIconLoaded(ConnectedPlayerData connectedPlayerData) {
        //void
    }

    public void leaderboardScoreUploadSucceeded(String leaderboardId, int scoreValue) {
        //void
    }

    public void leaderboardScoreUploadFailed(String leaderboardId, int scoreValue) {
        //void
    }

    public void playerInitiatedConnect() {
        //void
    }

    public void playerInitiatedDisconnect() {
        //void
    }

    public void playerCanceledLoginAccountSelection() {
        //void
    }

    public void leaderboardOpenSucceeded() {
        //void
    }

    public void leaderboardOpenFailed(int errorCode) {
        //void
    }

    public void leaderboardsSelectorOpenSucceeded() {
        //void
    }

    public void leaderboardsSelectorOpenFailed(int errorCode) {
        //void
    }

    public void achievementsOpenSucceeded() {
        //void
    }

    public void achievementsOpenFailed(int errorCode) {
        //void
    }

}
