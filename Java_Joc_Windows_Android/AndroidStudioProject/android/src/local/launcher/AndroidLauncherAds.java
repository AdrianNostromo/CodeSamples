package local.launcher;

import android.os.Bundle;
import android.widget.RelativeLayout;

import java.util.Vector;

import local.ads.AndroidAdsManager;
import local.ads.adnetworks.IAdNetwork;
import local.ads.launcher.IAdsManager;

/**
 * Created by adrin on 2/2/17.
 */

public abstract class AndroidLauncherAds extends AndroidLauncherPlayGames {

    private AndroidAdsManager adsManager;

    private Vector<IAdNetwork> adNetworksList = new Vector<IAdNetwork>();

    @Override
    protected void onCreate_preAppMainView(Bundle savedInstanceState, RelativeLayout layout) {
        super.onCreate_preAppMainView(savedInstanceState, layout);

        adsManager = new  AndroidAdsManager(this);
        if(adsManager != null) {
            adsManager.onCreate();
        }
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(adsManager != null) {
            adsManager.appLoop(deltaS, deltaMS);
        }
    }

    @Override
    public void onStart() {
        super.onStart();

        if(adsManager != null) {
            adsManager.onStart();
        }
    }

    @Override
    public void onResume() {
        super.onResume();

        if(adsManager != null) {
            adsManager.onResume();
        }
    }

    @Override
    public void onPause() {
        super.onPause();

        if(adsManager != null) {
            adsManager.onPause();
        }
    }

    @Override
    protected void onStop_pre() {
        super.onStop_pre();

        if(adsManager != null) {
            adsManager.onStop_pre();
        }
    }

    @Override
    public void onStop() {
        super.onStop();

        if(adsManager != null) {
            adsManager.onStop();
        }
    }

    @Override
    public void onBackPressed() {
        if(adsManager != null) {
            if(!adsManager.onBackPressed()) {
                super.onBackPressed();
            }
        }
    }

    protected IAdsManager getAdsManager() {
        return adsManager;
    }

    @Override
    public void onDestroy() {
        if(adsManager != null) {
            adsManager.onDestroy();
        }

        super.onDestroy();
    }

}
