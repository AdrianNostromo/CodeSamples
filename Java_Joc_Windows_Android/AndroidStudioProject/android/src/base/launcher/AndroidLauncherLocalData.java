package base.launcher;

import android.os.Bundle;
import android.widget.RelativeLayout;

import base.launcher.localdata.AndroidLocalDataHandler;

/**
 * Created by adrin on 2/2/17.
 */

public abstract class AndroidLauncherLocalData extends AndroidLauncherBase {

    private AndroidLocalDataHandler localDataHandler = null;

    @Override
    protected void onCreate_preAppMainView(Bundle savedInstanceState, RelativeLayout layout) {
        super.onCreate_preAppMainView(savedInstanceState, layout);

        // This object will handle the loading and saving of the player profile data.
        localDataHandler = new AndroidLocalDataHandler(this);
        // Load the player profile data.
        localDataHandler.load();

    }

    protected AndroidLocalDataHandler getLocalDataHandler() {
        return localDataHandler;
    }

}
