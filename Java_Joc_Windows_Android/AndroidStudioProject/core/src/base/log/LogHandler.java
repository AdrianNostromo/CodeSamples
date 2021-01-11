package base.log;

import com.badlogic.gdx.Application;
import com.badlogic.gdx.Gdx;

/**
 * Created by Adrin on 4/22/2016.
 */
public class LogHandler implements ILogHandler {

    public LogHandler() {
        super();

        //void
    }

    public void create() {
        Gdx.app.setLogLevel(Application.LOG_DEBUG);
    }

    @Override
    public void postLog(String locationID, String msg) {
        Gdx.app.debug("Dev-Log", "Dev-Log. Msg: " + msg + ". Location fxId: " + locationID);
        Gdx.app.debug("Dev-Log", "Dev-Log. Msg: " + msg + ". Location fxId: " + locationID);
    }

    public void postLog(String msg) {
        Gdx.app.debug("Dev-Log", "Log> " + msg);
    }

    @Override
    public void postWarning(String locationID, String msg) {
        Gdx.app.debug("Dev-Warning", "Dev-Warning. Msg: " + msg + ". Location fxId: " + locationID);
    }

    @Override
    public void postError(String locationID, String msg) {
        Gdx.app.debug("Dev-Error", "Dev-Error. Msg: " + msg + ". Location fxId: " + locationID);
    }

}
