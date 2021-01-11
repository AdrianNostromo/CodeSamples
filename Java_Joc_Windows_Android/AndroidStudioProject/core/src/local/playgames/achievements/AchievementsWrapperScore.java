package local.playgames.achievements;

import base.json.JSONObject;
import local.app.AppLocal;
import local.playgames.achievements.config.AchievementConfig;
import local.playgames.util.PlayGamesEventListener;
import local.playgames.util.PlayGamesStatus;


/**
 * Created by Adrin on 8/9/2016.
 */
public class AchievementsWrapperScore extends AchievementsWrapperOpen {

    public static final String PROFILE_DATA_ID_ACHIEVEMENTS_SEND_BUFFER = "PROFILE_DATA_ID_ACHIEVEMENTS_SEND_BUFFER";

    private JSONObject joSavedAchievementsStack = null;
    private float nextAchievementUploadUploadDelayS = 0;

    private PlayGamesEventListener playGamesEventListener = new PlayGamesEventListener() {
        @Override
        public void playGamesStatusChanged(PlayGamesStatus status) {
            super.playGamesStatusChanged(status);

            if(status == PlayGamesStatus.LOGGED_IN) {
                sendAchievementsIfAny();
            }
        }
    };

    public AchievementsWrapperScore(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        app.playGamesWrapper.getPlayGamesManager().addEventsListener(playGamesEventListener);

        // Load achievements buffer data;
        String savedDataString = app.profileData.getDataWithID_String(PROFILE_DATA_ID_ACHIEVEMENTS_SEND_BUFFER);
        if(savedDataString == null) {
            joSavedAchievementsStack = new JSONObject();
            joSavedAchievementsStack.put("entriesCount", 0);
        }else {
            joSavedAchievementsStack = new JSONObject(savedDataString);
        }
    }

    public void bufferAchievementUpload(String achievementId) {
        bufferAchievementUpload(achievementId, 0);
    }

    public void bufferAchievementUpload(String achievementId, int incrementAmount) {
        AchievementConfig achievementConfig = AchievementConfig.entriesMap.get(achievementId);

        if(achievementConfig.isIncremental && incrementAmount == 0) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }else if(!achievementConfig.isIncremental && incrementAmount != 0) {
            app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

            throw new Error("Error. 1.");
        }

        int entriesCount = joSavedAchievementsStack.getInt("entriesCount");

        joSavedAchievementsStack.put(Integer.toString(entriesCount) + "_name", achievementConfig.achievementId);
        joSavedAchievementsStack.put(Integer.toString(entriesCount) + "_isIncremental", achievementConfig.isIncremental);
        if (achievementConfig.isIncremental) {
            joSavedAchievementsStack.put(Integer.toString(entriesCount) + "_incrementAmount", incrementAmount);
        }

        joSavedAchievementsStack.put("entriesCount", entriesCount + 1);

        app.profileData.putDataWithID_String(PROFILE_DATA_ID_ACHIEVEMENTS_SEND_BUFFER, joSavedAchievementsStack.toString());
    }

    public void sendAchievementsIfAny() {
        if(app.playGamesWrapper.getPlayGamesManager().getPlayGamesStatus() == PlayGamesStatus.LOGGED_IN) {
            int entriesCount = joSavedAchievementsStack.getInt("entriesCount");

            if(entriesCount > 0) {
                int lastEntryIndex = entriesCount - 1;

                String name = joSavedAchievementsStack.getString(Integer.toString(lastEntryIndex) + "_name");
                boolean isIncremental = joSavedAchievementsStack.getBoolean(Integer.toString(lastEntryIndex) + "_isIncremental");
                int incrementAmount = 0;
                if(isIncremental) {
                    incrementAmount = joSavedAchievementsStack.getInt(Integer.toString(lastEntryIndex) + "_incrementAmount");
                }

                boolean removeEntry = false;
                if(isIncremental && incrementAmount == 0) {
                    app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

                    throw new Error("Error. 1.");
                }else if(!isIncremental && incrementAmount != 0) {
                    app.errorHandler.handleAppCriticalError("Error. 1.", "Error.1.");

                    throw new Error("Error. 1.");
                }else if(app.playGamesWrapper.getPlayGamesManager().activateAchievementWithId(name, incrementAmount)) {
                    removeEntry = true;
                }

                if(removeEntry) {
                    joSavedAchievementsStack.remove(Integer.toString(lastEntryIndex) + "_name");
                    joSavedAchievementsStack.remove(Integer.toString(lastEntryIndex) + "_isIncremental");
                    if(isIncremental) {
                        joSavedAchievementsStack.remove(Integer.toString(lastEntryIndex) + "_incrementAmount");
                    }

                    joSavedAchievementsStack.put("entriesCount", entriesCount - 1);

                    app.profileData.putDataWithID_String(PROFILE_DATA_ID_ACHIEVEMENTS_SEND_BUFFER, joSavedAchievementsStack.toString());
                }
            }
        }
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        // Send 5 achievement every 1 second.
        nextAchievementUploadUploadDelayS = Math.max(0, nextAchievementUploadUploadDelayS - deltaS);
        if(nextAchievementUploadUploadDelayS <= 0) {
            nextAchievementUploadUploadDelayS = 0.2f;

            if(app.getCurrentGameWorld() == null
                    || !app.getCurrentGameWorld().getIsLevelStarted()
                    || app.getCurrentGameWorld().getIsLevelEnded()
                    || !app.getCurrentGameWorld().getIsPlayEnabled())
            {
                sendAchievementsIfAny();
            }
        }
    }

}
