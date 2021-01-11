package base.desktop;

import com.badlogic.gdx.backends.lwjgl.LwjglApplication;
import com.badlogic.gdx.backends.lwjgl.LwjglApplicationConfiguration;

import java.util.HashMap;
import java.util.Map;

import base.launcher.IAppLauncher;
import base.launcher.ILocalDataHandler;
import base.launcher.IWebManager;
import local.MainLocal;
import local.ads.launcher.IAdsManager;
import local.ads.util.AdsEventListener;
import local.playgames.launcher.IPlayGamesManager;
import local.playgames.util.ConnectedPlayerData;
import local.playgames.util.PlayGamesEventListener;
import local.playgames.util.PlayGamesStatus;

public class DesktopLauncherBase {

    private static IAppLauncher appLauncher = new IAppLauncher() {
        @Override
        public void appLoop(float deltaS, int deltaMS) {
            //void
        }
    };

    private static ILocalDataHandler localDataHandler = new ILocalDataHandler() {

        private Map<String, Object> dataMap = new HashMap<String, Object>();

        @Override
        public int getInt(String key, int defaultValue) {
            if(!dataMap.containsKey(key)) {
                return defaultValue;
            }

            return (Integer)dataMap.get(key);
        }

        @Override
        public void putInt(String key, int value) {
            dataMap.put(key, value);
        }

        @Override
        public String getString(String key, String defaultValue) {
            if(!dataMap.containsKey(key)) {
                return defaultValue;
            }

            return (String)dataMap.get(key);
        }

        @Override
        public void putString(String key, String value) {
            dataMap.put(key, value);
        }

        @Override
        public void removeEntry(String key) {
            dataMap.remove(key);
        }

        @Override
        public void clearData() {
            dataMap.clear();
        }

        @Override
        public boolean getBoolean(String key, boolean defaultValue) {
            if(!dataMap.containsKey(key)) {
                return defaultValue;
            }

            return (Boolean)dataMap.get(key);
        }

        @Override
        public void putBoolean(String key, boolean value) {
            dataMap.put(key, value);
        }

    };

    private static IPlayGamesManager playGamesManager = new IPlayGamesManager() {

        @Override
        public void addEventsListener(PlayGamesEventListener eventsListener) {
            //void
        }

        @Override
        public void removeEventsListener(PlayGamesEventListener eventsListener) {
            //void
        }

        @Override
        public boolean showLeaderboardsSelector() {
            return false;
        }

        @Override
        public boolean showLeaderboardWithId(String leaderboardId) {
            return false;
        }

        @Override
        public void startConnect(boolean isPlayerDirectUIRequest) {
            //void
        }

        @Override
        public void startDisconnect(boolean isPlayerDirectUIRequest) {
            //void
        }

        @Override
        public ConnectedPlayerData getConnectedPlayerData() {
            return null;
        }

        @Override
        public PlayGamesStatus getPlayGamesStatus() {
            return PlayGamesStatus.OFFLINE;
        }

        @Override
        public boolean submitLeaderboardScore(String leaderboardId, int scoreValue) {
            return false;
        }

        @Override
        public boolean showAchievements() {
            return false;
        }

        @Override
        public boolean activateAchievementWithId(String achievementId, int incrementAmount) {
            return false;
        }
    };

    private static IWebManager webManager = new IWebManager() {
        @Override
        public void openUrl(String url) {
            //void
        }

        @Override
        public void openUrlWithID(String urlID) {
            //void
        }

        @Override
        public void shareScreenshot(String screenshotImgPath) {
            //void
        }
    };

    private static IAdsManager adsManager = new IAdsManager() {

        @Override
        public void gameWorldReady() {
            //void
        }

        @Override
        public void addEventsListener(AdsEventListener eventsListener) {
            //void
        }

        @Override
        public void removeEventsListener(AdsEventListener eventsListener) {
            //void
        }

        @Override
        public boolean isAnyInterstitialAdCached() {
            return false;
        }

        @Override
        public boolean isInterstitialAdDelayFinishedAndCached() {
            return false;
        }

        @Override
        public boolean showInterstitialAdIfPossible(boolean ignoreDelayTimer) {
            return false;
        }

        @Override
        public boolean isAnyRewardVideoAdCached() {
            return false;
        }

        @Override
        public boolean isRewardVideoAdDelayFinishedAndCached() {
            return false;
        }

        @Override
        public boolean showRewardVideoAdIfPossible(int callerID, boolean ignoreDelayTimer) {
            return false;
        }
    };

    public static void main (String[] arg) {
        LwjglApplicationConfiguration config = new LwjglApplicationConfiguration();
        config.width = 360;
        config.height = 570;
        config.samples = 2;

        // Start the game code that is located in the "core" part of the project.
        new LwjglApplication(
                new MainLocal(
                        appLauncher,
                        localDataHandler,
                        adsManager,
                        playGamesManager,
                        webManager
                ),
                config
        );
    }

}
