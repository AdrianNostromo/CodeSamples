package base.event;

import com.badlogic.gdx.Gdx;

import java.util.Vector;

/**
 * Created by adrin on 11/29/16.
 */

public class EventDispatcher {

    private Vector<IEventListener> eventListenersList = new Vector<IEventListener>();

    public EventDispatcher() {
        super();

        //void
    }

    public void addEventsListener(IEventListener eventsListener) {
        if(eventListenersList.indexOf(eventsListener) >= 0) {
            Gdx.app.debug("Error", "Events listener add error.");

            return ;
        }

        eventListenersList.add(eventsListener);
    }

    public void removeEventsListener(IEventListener eventsListener) {
        int i = eventListenersList.indexOf(eventsListener);
        if(i < 0) {
            Gdx.app.debug("Error", "Events listener remove error.");

            return ;
        }

        eventListenersList.remove(i);
    }

    protected void dispatchEvent(IEvent event) {
        for(int i=0;i<eventListenersList.size();i++) {
            IEventListener entry = eventListenersList.get(i);
            if(entry != null) {
                entry.onEvent(event);
            }
        }
    }

}
