package local.ads.adnetworks.chartboost;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.backends.android.AndroidApplication;
import com.chartboost.sdk.Chartboost;
import com.chartboost.sdk.ChartboostDelegate;
import com.chartboost.sdk.Model.CBError;

import local.ads.IAdNetworkController;
import local.ads.config.AdsConfig;

public class ChartboostAdNetwork extends ChartboostAdNetworkInterstitial {

    private ChartboostDelegate chartboostEventListener = new ChartboostDelegate() {

        @Override
        public void didCacheInterstitial(String location) {
            super.didCacheInterstitial(location);

            isInterstitialAdCachingInProgress = false;

            Gdx.app.debug("Debug.101001", "Chartboost interstitial loaded.");
        }

        @Override
        public void didFailToLoadInterstitial(String location, CBError.CBImpressionError error) {
            super.didFailToLoadInterstitial(location, error);

            isInterstitialAdCachingInProgress = false;

            Gdx.app.debug("Debug.101001", "Chartboost interstitial load failed.");
        }

        @Override
        public void didDisplayInterstitial(String location) {
            super.didDisplayInterstitial(location);

            isChartboostInterstitialOpen = true;
        }

        @Override
        public void didCloseInterstitial(String location) {
            super.didCloseInterstitial(location);

            // The player clicked on the close button for the ad.
            // The method didDismissInterstitial is also called.
        }

        @Override
        public void didDismissInterstitial(String location) {
            super.didDismissInterstitial(location);

            // The ad is no longer active. This method is always called when the ad is no longer visible.

            isChartboostInterstitialOpen = false;
        }

        @Override
        public void didClickInterstitial(String location) {
            super.didClickInterstitial(location);

            // The player clicked on the advertisement message. The ad will close by its self.
            // The method didDismissInterstitial is also called.
        }

        @Override
        public void didCacheRewardedVideo(String location) {
            super.didCacheRewardedVideo(location);

            isRewardedVideoAdCachingInProgress = false;

            Gdx.app.debug("Debug.101001", "Chartboost reward video loaded.");
        }

        @Override
        public void didFailToLoadRewardedVideo(String location, CBError.CBImpressionError error) {
            super.didFailToLoadRewardedVideo(location, error);

            isRewardedVideoAdCachingInProgress = false;

            Gdx.app.debug("Debug.101001", "Chartboost reward video load failed.");
        }

        @Override
        public void didCompleteRewardedVideo(String location, int reward) {
            super.didCompleteRewardedVideo(location, reward);

            rewardedVideoComplete();
        }

        @Override
        public void didDisplayRewardedVideo(String location) {
            super.didDisplayRewardedVideo(location);

            isChartboostRewardVideoOpen = true;
        }

        @Override
        public void didDismissRewardedVideo(String location) {
            super.didDismissRewardedVideo(location);

            // The ad is no longer active. This method is always called when the ad is no longer visible.

            isChartboostRewardVideoOpen = false;
        }

        @Override
        public void didCloseRewardedVideo(String location) {
            super.didCloseRewardedVideo(location);

            // The player clicked on the close button for the ad.
            // The method didDismissRewardedVideo is also called.
        }

        @Override
        public void didClickRewardedVideo(String location) {
            super.didClickRewardedVideo(location);

            // The player clicked on the advertisement message. The ad wil not close by its self.
        }
    };

    public ChartboostAdNetwork(IAdNetworkController adsManager, AndroidApplication activity) {
        super(adsManager, activity);

        //void
    }

    @Override
    public void onCreate() {
        super.onCreate();

        if(isEnabled_interstitial || isEnabled_rewardedVideo) {
            // Initialize Chartboost.
            Chartboost.startWithAppId(activity, AdsConfig.CHARTBOOST_APP_ID, AdsConfig.CHARTBOOST_APP_SIGNATURE);
            Chartboost.setDelegate(chartboostEventListener);
            Chartboost.onCreate(activity);
        }
    }

    @Override
    public void onStart() {
        if(isEnabled_interstitial || isEnabled_rewardedVideo) {
            Chartboost.onStart(activity);
        }
    }

    @Override
    public void onResume() {
        if(isEnabled_interstitial || isEnabled_rewardedVideo) {
            Chartboost.onResume(activity);
        }
    }

    @Override
    public void onPause() {
        if(isEnabled_interstitial || isEnabled_rewardedVideo) {
            Chartboost.onPause(activity);
        }
    }

    @Override
    public void onStop() {
        if(isEnabled_interstitial || isEnabled_rewardedVideo) {
            Chartboost.onStop(activity);
        }
    }

    @Override
    public void onStop_pre() {
        //void
    }

    @Override
    public void onDestroy() {
        if(isEnabled_interstitial || isEnabled_rewardedVideo) {
            Chartboost.onDestroy(activity);
        }
    }

    @Override
    public boolean onBackPressed() {
        boolean ret = false;

        if(isEnabled_interstitial || isEnabled_rewardedVideo) {
            // If an interstitial is on screen, close it.
            if (Chartboost.onBackPressed()) {
                ret = true;
            }
        }

        return ret;
    }

}
