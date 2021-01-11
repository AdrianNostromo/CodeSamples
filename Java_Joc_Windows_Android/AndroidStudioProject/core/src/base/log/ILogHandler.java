package base.log;

/**
 * Created by Adrin on 4/22/2016.
 */
public interface ILogHandler {

    void create();

    void postLog(String locationID, String msg);
    void postLog(String msg);

    void postWarning(String locationID, String msg);

    void postError(String locationID, String msg);

}
