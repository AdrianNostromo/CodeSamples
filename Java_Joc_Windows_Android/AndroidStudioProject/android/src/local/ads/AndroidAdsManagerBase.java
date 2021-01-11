package local.ads;

import com.badlogic.gdx.backends.android.AndroidApplication;

import local.ads.launcher.IAdsManager;

/**
 * Created by adrin on 2/8/17.
 */

public abstract class AndroidAdsManagerBase implements IAdsManager {

    protected AndroidApplication activity;

    public AndroidAdsManagerBase(AndroidApplication activity) {
        super();

        this.activity = activity;
    }

    public void appLoop(float deltaS, int deltaMS) {
        //void
    }

    public void onCreate() {
        //void
    }

    public void gameWorldReady() {
        //void
    }

    public void onStart() {
        //void
    }

    public void onResume() {
        //void
    }

    public void onPause() {
        //void
    }

    public void onStop_pre() {
        //void
    }

    public void onStop() {
        //void
    }

    public boolean onBackPressed() {
        //void

        return false;
    }

    public void onDestroy() {
        //void
    }

}
