package base.launcher;

import android.os.Bundle;
import android.widget.RelativeLayout;

import base.launcher.web.AndroidWebManager;

/**
 * Created by adrin on 2/2/17.
 */

public abstract class AndroidLauncherWeb extends AndroidLauncherLocalData {

    private AndroidWebManager webManager;

    @Override
    protected void onCreate_preAppMainView(Bundle savedInstanceState, RelativeLayout layout) {
        super.onCreate_preAppMainView(savedInstanceState, layout);

        webManager = new AndroidWebManager(this);
        webManager.onCreate();
    }

    protected AndroidWebManager getWebManager() {
        return webManager;
    }

}
