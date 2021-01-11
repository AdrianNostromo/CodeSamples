package local.playgames.achievements;

import local.app.AppLocal;

/**
 * Created by Adrin on 8/9/2016.
 */
public class AchievementsWrapperBase {

    protected AppLocal app;

    public AchievementsWrapperBase(AppLocal app) {
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
