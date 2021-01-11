package local.playgames.leaderboards;

import local.app.AppLocal;

/**
 * Created by Adrin on 8/8/2016.
 */
public class LeaderboardsWrapperBase {

    protected AppLocal app;

    public LeaderboardsWrapperBase(AppLocal app) {
        super();

        this.app = app;
    }

    public void create() {
        //void
    }

    public void appLoop(float deltaS, int deltaMS) {
        //void
    }

    public void dispose() {
        //void
    }

}
