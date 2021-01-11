package local.ads;

import com.badlogic.gdx.backends.android.AndroidApplication;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import local.ads.adnetworks.IAdNetwork;
import local.ads.adnetworks.chartboost.ChartboostAdNetwork;

/**
 * Created by adrin on 2/8/17.
 */

public abstract class AndroidAdsManagerNetworks extends AndroidAdsManagerListeners implements IAdNetworkController {

    protected Vector<IAdNetwork> adNetworksList = new Vector<IAdNetwork>();
    protected Map<String, IAdNetwork> adNetworksMap = new HashMap<>();

    public AndroidAdsManagerNetworks(AndroidApplication activity) {
        super(activity);

        //void
    }

    @Override
    public void onCreate() {
        super.onCreate();

        onCreate_chartboost();
    }

    private void onCreate_chartboost() {
        IAdNetwork adNetwork = new ChartboostAdNetwork(this, activity);
        adNetworksList.add(adNetwork);
        adNetworksMap.put(adNetwork.getAdNetworkId(), adNetwork);
        adNetwork.onCreate();
    }

    @Override
    public void onStart() {
        super.onStart();

        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            adNetwork.onStart();
        }
    }

    @Override
    public void onResume() {
        super.onResume();

        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            adNetwork.onResume();
        }
    }

    @Override
    public void onPause() {
        super.onPause();

        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            adNetwork.onPause();
        }
    }

    @Override
    public void onStop_pre() {
        super.onStop_pre();

        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            adNetwork.onStop_pre();
        }
    }

    @Override
    public void onStop() {
        super.onStop();

        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            adNetwork.onStop();
        }
    }

    @Override
    public boolean onBackPressed() {
        boolean ret = super.onBackPressed();

        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            if(adNetwork.onBackPressed()) {
                ret = true;
            }
        }

        return ret;
    }

    @Override
    public void onDestroy() {
        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            adNetwork.onDestroy();
        }

        super.onDestroy();
    }

    protected IAdNetwork getAdsNetworkWithId(String adNetworkID) {
        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            if(adNetwork.getAdNetworkId().equals(adNetworkID)) {
                return adNetwork;
            }
        }

        return null;
    }

}
