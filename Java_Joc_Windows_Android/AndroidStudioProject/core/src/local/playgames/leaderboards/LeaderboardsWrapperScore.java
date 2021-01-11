package local.playgames.leaderboards;

import java.util.HashMap;
import java.util.Map;

import local.app.AppLocal;
import local.playgames.leaderboards.config.LeaderboardConfig;
import local.playgames.util.PlayGamesEventListener;
import local.playgames.util.PlayGamesStatus;

/**
 * Created by Adrin on 8/8/2016.
 */
public class LeaderboardsWrapperScore extends LeaderboardsWrapperOpen {

    private float nextLeaderboardUploadUploadDelayS = 0;

    private Map<String, Boolean> activeLeaderboardUploadsFlagsMap = new HashMap<String, Boolean>();

    private PlayGamesEventListener playGamesEventListener = new PlayGamesEventListener() {
        @Override
        public void playGamesStatusChanged(PlayGamesStatus status) {
            super.playGamesStatusChanged(status);

            if(status == PlayGamesStatus.LOGGED_IN) {
                for(int i=0;i<LeaderboardConfig.entriesList.size();i++) {
                    LeaderboardConfig leaderboardConfig = LeaderboardConfig.entriesList.get(i);

                    uploadLeaderboardScoreIfAny(leaderboardConfig);
                }
            }
        }

        @Override
        public void leaderboardScoreUploadSucceeded(String leaderboardId, int scoreValue) {
            super.leaderboardScoreUploadSucceeded(leaderboardId, scoreValue);

            LeaderboardConfig leaderboardConfig = LeaderboardConfig.entriesMap.get(leaderboardId);
            if(leaderboardConfig == null) {
                app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

                throw new Error("Error. 1.");
            }

            if(leaderboardConfig.type == LeaderboardConfig.TYPE_HIGHEST_VALUE) {
                if(scoreValue > app.profileData.getDataWithID_int(leaderboardConfig.activeUploadProfileDataId)) {
                    app.profileData.putDataWithID_int(leaderboardConfig.activeUploadProfileDataId, 0);
                }
            }else if(leaderboardConfig.type == LeaderboardConfig.TYPE_ADDITIVE
                    || leaderboardConfig.type == LeaderboardConfig.TYPE_SINGLE_INCREMENT_BY_1)
            {
                app.profileData.addDataWithID_int(leaderboardConfig.activeUploadProfileDataId, -scoreValue);
            }else {
                app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

                throw new Error("Error. 1.");
            }
            activeLeaderboardUploadsFlagsMap.put(leaderboardConfig.leaderboardID, false);

            //uploadLeaderboardScoreIfAny(leaderboardConfig);
        }

        @Override
        public void leaderboardScoreUploadFailed(String leaderboardId, int scoreValue) {
            super.leaderboardScoreUploadFailed(leaderboardId, scoreValue);

            //asd_001;// Check if the parameter scoreValuePoints will contain actually valid data and refactor code to remove active uploads from logic.

            activeLeaderboardUploadsFlagsMap.put(leaderboardId, false);
        }
    };

    public LeaderboardsWrapperScore(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        app.playGamesWrapper.getPlayGamesManager().addEventsListener(playGamesEventListener);

        for(int i=0;i<LeaderboardConfig.entriesList.size();i++) {
            LeaderboardConfig leaderboardConfig = LeaderboardConfig.entriesList.get(i);

            activeLeaderboardUploadsFlagsMap.put(leaderboardConfig.leaderboardID, false);
        }
    }

    public void bufferLeaderboardValueUpload(String leaderboardId, int value) {
        LeaderboardConfig leaderboardConfig = LeaderboardConfig.entriesMap.get(leaderboardId);
        if(leaderboardConfig == null) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }

        if(leaderboardConfig.type == LeaderboardConfig.TYPE_HIGHEST_VALUE) {
            app.profileData.putDataWithIDIfHigher_int(leaderboardConfig.activeUploadProfileDataId, value);
        }else if(leaderboardConfig.type == LeaderboardConfig.TYPE_ADDITIVE) {
            app.profileData.addDataWithID_int(leaderboardConfig.activeUploadProfileDataId, value);
        }else if(leaderboardConfig.type == LeaderboardConfig.TYPE_SINGLE_INCREMENT_BY_1) {
            app.profileData.addDataWithID_int(leaderboardConfig.activeUploadProfileDataId, 1);
        }else {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }
    }

    private boolean uploadLeaderboardScoreIfAny(LeaderboardConfig leaderboardConfig) {
        if(!activeLeaderboardUploadsFlagsMap.get(leaderboardConfig.leaderboardID)) {
            int scoreToUpload = app.profileData.getDataWithID_int(leaderboardConfig.activeUploadProfileDataId);

            if(scoreToUpload > 0) {
                activeLeaderboardUploadsFlagsMap.put(leaderboardConfig.leaderboardID, true);

                if(!app.playGamesWrapper.getPlayGamesManager().submitLeaderboardScore(leaderboardConfig.leaderboardID, scoreToUpload)) {
                    activeLeaderboardUploadsFlagsMap.put(leaderboardConfig.leaderboardID, false);
                }else {
                    return true;
                }
            }
        }

        return false;
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        // Send 5 achievement every 1 second.
        nextLeaderboardUploadUploadDelayS = Math.max(0, nextLeaderboardUploadUploadDelayS - deltaS);
        if(nextLeaderboardUploadUploadDelayS <= 0) {
            nextLeaderboardUploadUploadDelayS = 0.2f;

            if(app.getCurrentGameWorld() == null
                    || !app.getCurrentGameWorld().getIsLevelStarted()
                    || app.getCurrentGameWorld().getIsLevelEnded()
                    || !app.getCurrentGameWorld().getIsPlayEnabled())
            {
                for(int i=0;i<LeaderboardConfig.entriesList.size();i++) {
                    LeaderboardConfig leaderboardConfig = LeaderboardConfig.entriesList.get(i);

                    if(uploadLeaderboardScoreIfAny(leaderboardConfig)) {
                        break;
                    }
                }
            }
        }
    }

}
