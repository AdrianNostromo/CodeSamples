package local.gameworld;

import local.app.AppLocal;
import local.playgames.achievements.config.AchievementConfig;
import local.playgames.leaderboards.config.LeaderboardConfig;
import local.profile.config.ProfileDataIDs;

// This class handles all content related to the actual game systems.
public class GameWorld extends GameWorldUserInterface {

    private boolean isLevelEndMenuActivated = false;
    private float levelEndMenuDelayS = 0;

    public GameWorld(AppLocal app) {
        super(app);

        //void
    }

    @Override
    protected void gameLoop(float deltaS, int deltaMS) {
        super.gameLoop(deltaS, deltaMS);

        app.fxManager.gameLoop(deltaS, deltaMS);

        if(getIsLevelEnded()) {
            if(!isLevelEndMenuActivated) {
                levelEndMenuDelayS = Math.max(0, levelEndMenuDelayS - deltaS);

                if(levelEndMenuDelayS <= 0) {
                    isLevelEndMenuActivated = true;

                    int score = getPointsCollected();

                    app.profileData.addDataWithID_int(ProfileDataIDs.ROUNDS_PLAYED, 1);

                    app.profileData.putDataWithIDIfHigher_int(ProfileDataIDs.BEST_SCORE, score);

                    if(score > 0) {
                        app.leaderboardsWrapper.bufferLeaderboardValueUpload(LeaderboardConfig.TotalScore.leaderboardID, score);
                    }

                    // RUNNER_I is not incrementable.
                    app.achievementsWrapper.bufferAchievementUpload(AchievementConfig.RUNNER_I.achievementId);
                    app.achievementsWrapper.bufferAchievementUpload(AchievementConfig.RUNNER_II.achievementId, 1);
                    app.achievementsWrapper.bufferAchievementUpload(AchievementConfig.RUNNER_III.achievementId, 1);

                    if(score >= 1) {
                        app.achievementsWrapper.bufferAchievementUpload(AchievementConfig.GEMOMATIC_I.achievementId);
                    }
                    if(score >= 5) {
                        app.achievementsWrapper.bufferAchievementUpload(AchievementConfig.GEMOMATIC_II.achievementId);
                    }
                    if(score >= 10) {
                        app.achievementsWrapper.bufferAchievementUpload(AchievementConfig.GEMOMATIC_III.achievementId);
                    }

                    app.getLevelEndView().show(true, null, score);

                    app.adsManager.showInterstitialAdIfPossible(false);
                }
            }

            return ;
        }
    }

    @Override
    protected void levelEnded(int levelEndType) {
        super.levelEnded(levelEndType);

        levelEndMenuDelayS = 0.4f;

        doCameraDropAnimation();
    }

}
