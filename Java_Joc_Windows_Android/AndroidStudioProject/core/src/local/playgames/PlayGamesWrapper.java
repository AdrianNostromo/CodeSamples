package local.playgames;

import base.IBaseAppComponent;
import local.app.AppLocal;
import local.playgames.launcher.IPlayGamesManager;

public class PlayGamesWrapper implements IBaseAppComponent {

    private AppLocal app;

    private IPlayGamesManager playGamesManager;

    public PlayGamesWrapper(AppLocal app, IPlayGamesManager playGamesManager) {
        super();

        this.app = app;

        this.playGamesManager = playGamesManager;
    }

    public void create() {
        //void
    }

    public IPlayGamesManager getPlayGamesManager() {
        return playGamesManager;
    }

    public void appLoop(float deltaS, int deltaMS) {
        //void
    }

    @Override
    public void dispose() {
        //void
    }
}
