package local.ads;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.backends.android.AndroidApplication;

import java.util.Vector;

import local.ads.util.AdsEventListener;

/**
 * Created by adrin on 2/8/17.
 */

public abstract class AndroidAdsManagerListeners extends AndroidAdsManagerBase implements IAdNetworkController {

    protected Vector<AdsEventListener> eventListenersList = new Vector<AdsEventListener>();

    public AndroidAdsManagerListeners(AndroidApplication activity) {
        super(activity);

        //void
    }

    public void addEventsListener(AdsEventListener eventsListener) {
        if(eventListenersList.indexOf(eventsListener) >= 0) {
            Gdx.app.debug("Error", "Events listener add error.");

            return ;
        }

        eventListenersList.add(eventsListener);
    }

    public void removeEventsListener(AdsEventListener eventsListener) {
        int i = eventListenersList.indexOf(eventsListener);
        if(i < 0) {
            Gdx.app.debug("Error", "Events listener remove error.");

            return ;
        }

        eventListenersList.remove(i);
    }

}
