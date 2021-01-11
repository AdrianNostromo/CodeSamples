package base.profile;

import com.badlogic.gdx.Gdx;

import java.util.Vector;

import base.launcher.ILocalDataHandler;
import base.profile.util.PlayerProfileDataEventListener;
import local.app.AppLocal;

//The player profile handles persistent game information.
public abstract class PlayerProfileDataBase {

	private AppLocal app;

	protected ILocalDataHandler localDataHandler;

    protected Vector<PlayerProfileDataEventListener> eventListenersList = new Vector<PlayerProfileDataEventListener>();

    public PlayerProfileDataBase(AppLocal app, ILocalDataHandler localDataHandler) {
        super();

	    this.app = app;

        this.localDataHandler = localDataHandler;
    }

    public void addEventsListener(PlayerProfileDataEventListener eventsListener) {
        if(eventListenersList.indexOf(eventsListener) >= 0) {
            Gdx.app.debug("Error", "Events listener add error.");

            return ;
        }

        eventListenersList.add(eventsListener);
    }

    public void removeEventsListener(PlayerProfileDataEventListener eventsListener) {
        int i = eventListenersList.indexOf(eventsListener);
        if(i < 0) {
            Gdx.app.debug("Error", "Events listener remove error.");

            return ;
        }

        eventListenersList.remove(i);
    }

    public int getDataWithID_int(String dataID) {
        return localDataHandler.getInt(dataID, 0);
    }

    public void putDataWithID_int(String dataID, int value) {
        localDataHandler.putInt(dataID, value);

        dispatchEvent_profileDataChanged(dataID);
    }

    public void addDataWithID_int(String dataID, int addValue) {
        putDataWithID_int(dataID, getDataWithID_int(dataID) + addValue);
    }

    public void putDataWithIDIfHigher_int(String dataID, int value) {
        if(value > getDataWithID_int(dataID)) {
            putDataWithID_int(dataID, value);
        }
    }

    public boolean getDataWithID_boolean(String dataID) {
        return localDataHandler.getBoolean(dataID, false);
    }

    public void putDataWithID_boolean(String dataID, boolean value) {
        localDataHandler.putBoolean(dataID, value);

        dispatchEvent_profileDataChanged(dataID);
    }

    public String getDataWithID_String(String dataID) {
        return localDataHandler.getString(dataID, null);
    }

    public void putDataWithID_String(String dataID, String value) {
        localDataHandler.putString(dataID, value);

        dispatchEvent_profileDataChanged(dataID);
    }

    public void removeDataWithID(String dataID) {
        localDataHandler.removeEntry(dataID);

        dispatchEvent_profileDataChanged(dataID);
    }

    public void clearAllData() {
        localDataHandler.clearData();
    }

    protected void dispatchEvent_profileDataChanged(String dataID) {
        for(int i=0;i<eventListenersList.size();i++) {
            PlayerProfileDataEventListener entry = eventListenersList.get(i);
            if(entry != null) {
                entry.profileDataChanged(dataID);
            }
        }
    }

}
