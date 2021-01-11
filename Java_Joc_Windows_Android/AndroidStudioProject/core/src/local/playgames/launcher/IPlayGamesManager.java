package local.playgames.launcher;

import local.playgames.util.ConnectedPlayerData;
import local.playgames.util.PlayGamesEventListener;
import local.playgames.util.PlayGamesStatus;

// Interface for ads control.
public interface IPlayGamesManager {

    void addEventsListener(PlayGamesEventListener eventsListener);
    void removeEventsListener(PlayGamesEventListener eventsListener);

	void startConnect(boolean isPlayerDirectUIRequest);
    void startDisconnect(boolean isPlayerDirectUIRequest);
    PlayGamesStatus getPlayGamesStatus();
    ConnectedPlayerData getConnectedPlayerData();

    boolean showLeaderboardWithId(String leaderboardId);
    boolean showLeaderboardsSelector();

    boolean submitLeaderboardScore(String leaderboardId, int scoreValue);

	boolean showAchievements();
    boolean activateAchievementWithId(String achievementId, int incrementAmount);

}
