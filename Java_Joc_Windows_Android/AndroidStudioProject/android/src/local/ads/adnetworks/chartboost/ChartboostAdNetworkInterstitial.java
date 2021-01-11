package local.ads.adnetworks.chartboost;

import com.badlogic.gdx.backends.android.AndroidApplication;
import com.chartboost.sdk.CBLocation;
import com.chartboost.sdk.Chartboost;

import local.ads.IAdNetworkController;
import local.ads.config.AdsConfig;

/**
 * Created by adrin on 2/8/17.
 */

public abstract class ChartboostAdNetworkInterstitial extends ChartboostAdNetworkRewardedVideo {

    protected boolean isChartboostInterstitialOpen = false;

    protected boolean isEnabled_interstitial;

    protected boolean isInterstitialAdCachingInProgress = false;

    public ChartboostAdNetworkInterstitial(IAdNetworkController adsManager, AndroidApplication activity) {
        super(adsManager, activity);

        //void
    }

    @Override
    public void onCreate() {
        super.onCreate();

        isEnabled_interstitial = AdsConfig.ENABLE_CHARTBOOST_INTERSTITIAL;
    }

    @Override
    public boolean canSupportAndEnabledInterstitialAds() {
        return isEnabled_interstitial;
    }

    @Override
    public boolean getIsInterstitialAdCachingInProgress() {
        return isInterstitialAdCachingInProgress;
    }

    @Override
    public boolean isInterstitialAdCached() {
        if(Chartboost.hasInterstitial(CBLocation.LOCATION_DEFAULT)) {
            return true;
        }

        return false;
    }

    @Override
    public void cacheInterstitialAd() {
        isInterstitialAdCachingInProgress = true;
        Chartboost.cacheInterstitial(CBLocation.LOCATION_DEFAULT);
    }

    @Override
    public boolean showInterstitialAd() {
        if(!Chartboost.hasInterstitial(CBLocation.LOCATION_DEFAULT)) {
            cacheInterstitialAd();

            return false;
        }

        Chartboost.showInterstitial(CBLocation.LOCATION_DEFAULT);

        return true;
    }

}
