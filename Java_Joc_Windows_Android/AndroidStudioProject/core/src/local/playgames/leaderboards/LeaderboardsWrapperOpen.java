package local.playgames.leaderboards;

import base.view.popupmenu.util.PopupConfig;
import base.view.popupmenu.util.PopupEventListener;
import local.app.AppLocal;
import local.playgames.util.PlayGamesEventListener;
import local.playgames.util.PlayGamesStatus;

/**
 * Created by Adrin on 8/8/2016.
 */
public class LeaderboardsWrapperOpen extends LeaderboardsWrapperBase {

    private boolean isAllLeaderboardsOpenWaiting = false;
    private String targetLeaderboardId = null;
    private long allLeaderboardsShowInitializeTime = 0;

    private static final long delayedLeaderboardsOpenThreshold = 1 * 60 * 1000;

    private PlayGamesEventListener playGamesEventListener = new PlayGamesEventListener() {
        @Override
        public void playGamesStatusChanged(PlayGamesStatus status) {
            super.playGamesStatusChanged(status);

            if (isAllLeaderboardsOpenWaiting && (status == PlayGamesStatus.LOGGED_IN || status == PlayGamesStatus.OFFLINE)) {
                if (status == PlayGamesStatus.LOGGED_IN && System.currentTimeMillis() - allLeaderboardsShowInitializeTime <= delayedLeaderboardsOpenThreshold) {
                    boolean b;
                    if(targetLeaderboardId != null) {
                        b = app.playGamesWrapper.getPlayGamesManager().showLeaderboardWithId(targetLeaderboardId);
                    }else {
                        b = app.playGamesWrapper.getPlayGamesManager().showLeaderboardsSelector();
                    }
                    if (!b) {
                        app.getPopupMenu().showPopup(PopupConfig.LEADERBOARD_SHOW_FAILED, null, true);
                    }
                }

                isAllLeaderboardsOpenWaiting = false;
            }
        }

        @Override
        public void playerInitiatedDisconnect() {
            super.playerInitiatedDisconnect();

            isAllLeaderboardsOpenWaiting = false;
        }

        @Override
        public void playerCanceledLoginAccountSelection() {
            super.playerCanceledLoginAccountSelection();

            isAllLeaderboardsOpenWaiting = false;
        }

    };

    public LeaderboardsWrapperOpen(AppLocal app) {
        super(app);

        //void
    }

    public void cancelAllDelayedLeaderboardsOpen() {
        isAllLeaderboardsOpenWaiting = false;
    }

    @Override
    public void create() {
        super.create();

        app.playGamesWrapper.getPlayGamesManager().addEventsListener(playGamesEventListener);
    }

    /**
     *
     * @param leaderboardId
     * Contains the leaderboard to show. If null, the leaderboards selector will be shown.
     */
    public void managedShowLeaderboard(String leaderboardId) {
        targetLeaderboardId = leaderboardId;

        PlayGamesStatus pgs = app.playGamesWrapper.getPlayGamesManager().getPlayGamesStatus();
        if(pgs == PlayGamesStatus.LOGGED_IN) {
            boolean b;
            if(targetLeaderboardId != null) {
                b = app.playGamesWrapper.getPlayGamesManager().showLeaderboardWithId(targetLeaderboardId);
            }else {
                b = app.playGamesWrapper.getPlayGamesManager().showLeaderboardsSelector();
            }
            if(!b) {
                app.getPopupMenu().showPopup(PopupConfig.LEADERBOARD_SHOW_FAILED, null, true);
            }
        }else if(pgs == PlayGamesStatus.CONNECTING) {
            if(!isAllLeaderboardsOpenWaiting) {
                isAllLeaderboardsOpenWaiting = true;
                allLeaderboardsShowInitializeTime = System.currentTimeMillis();

                app.achievementsWrapper.cancelAllDelayedAchievementsOpen();
            }

            // Show popup waiting for connection.
            app.getPopupMenu().showPopup(PopupConfig.WAITING_FOR_CONNECTION, null, true);
        }else if(pgs == PlayGamesStatus.OFFLINE) {
            // Show popup waiting for connection.
            app.getPopupMenu().showPopup(PopupConfig.PLAY_GAMES_CONNECTION_REQUIRED, new PopupEventListener() {
                @Override
                public void popupActionActivated() {
                    super.popupActionActivated();

                    if(app.playGamesWrapper.getPlayGamesManager().getPlayGamesStatus() == PlayGamesStatus.OFFLINE && !isAllLeaderboardsOpenWaiting) {
                        isAllLeaderboardsOpenWaiting = true;
                        allLeaderboardsShowInitializeTime = System.currentTimeMillis();

                        app.achievementsWrapper.cancelAllDelayedAchievementsOpen();

                        app.playGamesWrapper.getPlayGamesManager().startConnect(true);
                    }
                }
            }, true);
        }else {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }
    }

    public void managedShowAllLeaderboards() {
        managedShowLeaderboard(null);
    }

}
