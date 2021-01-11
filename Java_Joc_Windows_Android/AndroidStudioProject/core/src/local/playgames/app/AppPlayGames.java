package local.playgames.app;

import base.app.App3D;
import base.launcher.ILocalDataHandler;
import base.launcher.IWebManager;
import local.app.AppLocal;
import local.playgames.PlayGamesWrapper;
import local.playgames.achievements.AchievementsWrapper;
import local.playgames.launcher.IPlayGamesManager;
import local.playgames.leaderboards.LeaderboardsWrapper;

/**
 * Created by adrin on 11/29/16.
 */

public abstract class AppPlayGames extends App3D {

    // Variable should not be accessed, use playGamesWrapper instead.
    private IPlayGamesManager _playGamesManager;

    public PlayGamesWrapper playGamesWrapper;

    public LeaderboardsWrapper leaderboardsWrapper;
    public AchievementsWrapper achievementsWrapper;

    public AppPlayGames(base.launcher.IAppLauncher appLauncher, ILocalDataHandler localDataHandler, IWebManager webManager, IPlayGamesManager playGamesManager) {
        super(appLauncher, localDataHandler, webManager);

        this._playGamesManager = playGamesManager;
    }

    @Override
    public void create() {
        super.create();

        appComponentsList.add(playGamesWrapper = new PlayGamesWrapper((AppLocal)this, _playGamesManager));
        playGamesWrapper.create();

        appComponentsList.add(leaderboardsWrapper = new LeaderboardsWrapper((local.app.AppLocal)this));
        leaderboardsWrapper.create();

        appComponentsList.add(achievementsWrapper = new AchievementsWrapper((local.app.AppLocal)this));
        achievementsWrapper.create();
    }
}
