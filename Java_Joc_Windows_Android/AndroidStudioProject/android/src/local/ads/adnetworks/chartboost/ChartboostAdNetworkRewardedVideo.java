package local.ads.adnetworks.chartboost;

import com.badlogic.gdx.backends.android.AndroidApplication;
import com.chartboost.sdk.CBLocation;
import com.chartboost.sdk.Chartboost;

import local.ads.IAdNetworkController;
import local.ads.config.AdsConfig;

/**
 * Created by adrin on 2/8/17.
 */

public abstract class ChartboostAdNetworkRewardedVideo extends ChartboostAdNetworkBase {

    protected boolean isChartboostRewardVideoOpen = false;

    protected boolean isEnabled_rewardedVideo;

    private int rewardedVideoID = 0;

    protected boolean isRewardedVideoAdCachingInProgress = false;

    public ChartboostAdNetworkRewardedVideo(IAdNetworkController adsManager, AndroidApplication activity) {
        super(adsManager, activity);

        //void
    }

    @Override
    public void onCreate() {
        super.onCreate();

        isEnabled_rewardedVideo = AdsConfig.ENABLE_CHARTBOOST_REWARDED_VIDEO;
    }

    @Override
    public boolean canSupportAndEnabledRewardedVideoAds() {
        return isEnabled_rewardedVideo;
    }

    @Override
    public boolean getIsRewardedVideoAdCachingInProgress() {
        return isRewardedVideoAdCachingInProgress;
    }

    @Override
    public boolean isRewardedVideoAdAdCached() {
        if(Chartboost.hasRewardedVideo(CBLocation.LOCATION_DEFAULT)) {
            return true;
        }

        return false;
    }

    @Override
    public void cacheRewardedVideoAd() {
        isRewardedVideoAdCachingInProgress = true;
        Chartboost.cacheRewardedVideo(CBLocation.LOCATION_DEFAULT);
    }

    @Override
    public boolean showRewardedVideoAd(int rewardedVideoID) {
        this.rewardedVideoID = rewardedVideoID;

        if(!Chartboost.hasRewardedVideo(CBLocation.LOCATION_DEFAULT)) {
            cacheRewardedVideoAd();

            this.rewardedVideoID = 0;

            return false;
        }

        Chartboost.showRewardedVideo(CBLocation.LOCATION_DEFAULT);

        return true;
    }

    protected void rewardedVideoComplete() {
        adsManager.rewardedVideoComplete(rewardedVideoID);

        if(rewardedVideoID == 0) {
            throw new Error("Error. 1.");
        }

        rewardedVideoID = 0;
    }

}
