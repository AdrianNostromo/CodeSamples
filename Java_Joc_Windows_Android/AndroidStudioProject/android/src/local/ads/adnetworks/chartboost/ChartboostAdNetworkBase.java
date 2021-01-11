package local.ads.adnetworks.chartboost;

import com.badlogic.gdx.backends.android.AndroidApplication;

import local.ads.IAdNetworkController;
import local.ads.adnetworks.IAdNetwork;

public abstract class ChartboostAdNetworkBase implements IAdNetwork {

    private static final String AD_NETWORK_ID = "Chartboost";

    protected IAdNetworkController adsManager;

    protected AndroidApplication activity;

    protected boolean isNetworkInitialised = true;

    public ChartboostAdNetworkBase(IAdNetworkController adsManager, AndroidApplication activity) {
        super();

        this.adsManager = adsManager;

        this.activity = activity;
    }

    @Override
    public String getAdNetworkId() {
        return AD_NETWORK_ID;
    }

    @Override
    public void onCreate() {
        //void
    }

    @Override
    public boolean getIsNetworkInitialised() {
        return isNetworkInitialised;
    }
}
