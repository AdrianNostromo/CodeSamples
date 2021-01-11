package base;

import local.app.AppLocal;

/**
 * Created by Adrin on 6/8/2016.
 */
public class BaseAppComponent implements IBaseAppComponent {

    protected AppLocal app;

    public BaseAppComponent(AppLocal app) {
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