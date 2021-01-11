package local.playgames.achievements;

import base.view.popupmenu.util.PopupConfig;
import base.view.popupmenu.util.PopupEventListener;
import local.app.AppLocal;
import local.playgames.util.PlayGamesEventListener;
import local.playgames.util.PlayGamesStatus;

/**
 * Created by Adrin on 8/9/2016.
 */
public class AchievementsWrapperOpen extends AchievementsWrapperBase {

    private boolean isAllAchievementsOpenWaiting = false;
    private long allAchievementsShowInitializeTime = 0;

    private static final long delayedAchievementsOpenThreshold = 1 * 60 * 1000;

    private PlayGamesEventListener playGamesEventListener = new PlayGamesEventListener() {
        @Override
        public void playGamesStatusChanged(PlayGamesStatus status) {
            super.playGamesStatusChanged(status);

            if (isAllAchievementsOpenWaiting && (status == PlayGamesStatus.LOGGED_IN || status == PlayGamesStatus.OFFLINE)) {
                isAllAchievementsOpenWaiting = false;

                if (status == PlayGamesStatus.LOGGED_IN && System.currentTimeMillis() - allAchievementsShowInitializeTime <= delayedAchievementsOpenThreshold) {
                    if (!app.playGamesWrapper.getPlayGamesManager().showAchievements()) {
                        app.getPopupMenu().showPopup(PopupConfig.ACHIEVEMENTS_SHOW_FAILED, null, true);
                    }
                }
            }
        }

        @Override
        public void playerInitiatedDisconnect() {
            super.playerInitiatedDisconnect();

            isAllAchievementsOpenWaiting = false;
        }

        @Override
        public void playerCanceledLoginAccountSelection() {
            super.playerCanceledLoginAccountSelection();

            isAllAchievementsOpenWaiting = false;
        }

    };

    public AchievementsWrapperOpen(AppLocal app) {
        super(app);

        //void
    }

    public void cancelAllDelayedAchievementsOpen() {
        isAllAchievementsOpenWaiting = false;
    }

    @Override
    public void create() {
        super.create();

        app.playGamesWrapper.getPlayGamesManager().addEventsListener(playGamesEventListener);
    }

    public void managedShowAchievements() {
        PlayGamesStatus pgs = app.playGamesWrapper.getPlayGamesManager().getPlayGamesStatus();
        if(pgs == PlayGamesStatus.LOGGED_IN) {
            if(!app.playGamesWrapper.getPlayGamesManager().showAchievements()) {
                app.getPopupMenu().showPopup(PopupConfig.ACHIEVEMENTS_SHOW_FAILED, null, true);
            }
        }else if(pgs == PlayGamesStatus.CONNECTING) {
            if(!isAllAchievementsOpenWaiting) {
                isAllAchievementsOpenWaiting = true;
                allAchievementsShowInitializeTime = System.currentTimeMillis();

                app.leaderboardsWrapper.cancelAllDelayedLeaderboardsOpen();
            }

            // Show popup waiting for connection.
            app.getPopupMenu().showPopup(PopupConfig.WAITING_FOR_CONNECTION, null, true);
        }else if(pgs == PlayGamesStatus.OFFLINE) {
            // Show popup waiting for connection.
            app.getPopupMenu().showPopup(PopupConfig.PLAY_GAMES_CONNECTION_REQUIRED, new PopupEventListener() {
                @Override
                public void popupActionActivated() {
                    super.popupActionActivated();

                    if(app.playGamesWrapper.getPlayGamesManager().getPlayGamesStatus() == PlayGamesStatus.OFFLINE && !isAllAchievementsOpenWaiting) {
                        isAllAchievementsOpenWaiting = true;
                        allAchievementsShowInitializeTime = System.currentTimeMillis();

                        app.leaderboardsWrapper.cancelAllDelayedLeaderboardsOpen();

                        app.playGamesWrapper.getPlayGamesManager().startConnect(true);
                    }
                }
            }, true);
        }else {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }
    }
    
}
