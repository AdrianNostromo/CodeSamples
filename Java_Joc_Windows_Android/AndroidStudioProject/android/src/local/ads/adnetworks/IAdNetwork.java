package local.ads.adnetworks;

/**
 * Created by adrin on 2/8/17.
 */

public interface IAdNetwork {

    String getAdNetworkId();
    boolean getIsNetworkInitialised();

    boolean canSupportAndEnabledInterstitialAds();
    void cacheInterstitialAd();
    boolean showInterstitialAd();
    boolean getIsInterstitialAdCachingInProgress();
    boolean isInterstitialAdCached();

    boolean canSupportAndEnabledRewardedVideoAds();
    void cacheRewardedVideoAd();
    boolean showRewardedVideoAd(int rewardedVideoID);
    boolean getIsRewardedVideoAdCachingInProgress();
    boolean isRewardedVideoAdAdCached();

    void onCreate();

    void onStart();

    void onResume();

    void onPause();

    void onStop_pre();

    void onStop();

    boolean onBackPressed();

    void onDestroy();


}
