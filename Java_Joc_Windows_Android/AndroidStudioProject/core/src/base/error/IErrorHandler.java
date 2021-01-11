package base.error;

/**
 * Created by Adrin on 4/22/2016.
 */
public interface IErrorHandler {

    void create();

    void handleAppCriticalError(String locationID, String msg);

}
