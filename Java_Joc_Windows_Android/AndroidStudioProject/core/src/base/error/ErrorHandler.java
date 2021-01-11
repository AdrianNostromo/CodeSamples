package base.error;

import local.app.AppLocal;

/**
 * Created by Adrin on 4/22/2016.
 */
public class ErrorHandler implements IErrorHandler {

    protected AppLocal app;

    public ErrorHandler(AppLocal app) {
        super();

        this.app = app;
    }

    public void create() {
        //void
    }

    @Override
    public void handleAppCriticalError(String locationID, String msg) {
        app.log.postError(locationID, msg);

        //asd_002;// handle it;
    }
}
