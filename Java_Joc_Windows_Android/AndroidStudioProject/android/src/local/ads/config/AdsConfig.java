package local.ads.config;

public class AdsConfig {

    public static final boolean ENABLE_CHARTBOOST_INTERSTITIAL = true;
    public static final boolean ENABLE_CHARTBOOST_REWARDED_VIDEO = true;

    public static final String CHARTBOOST_APP_ID = "589afbbdf6cd4542fa27991f";
    public static final String CHARTBOOST_APP_SIGNATURE = "e0245c88912aeaee51353855fa614b80edcb4b58";
    
    // Delay before the first interstitial ad is shown in minutes.
    public static final float interstitial_delayMinutes_first = 0.5f;// 0.5 = half a minute = 30 seconds;
    // Delay between interstitial ads in minutes.
    public static final float interstitial_delayMinutes = 2.0f;
    
    // Delay before the first rewarded video ad button is available in minutes.
    public static final float rewardVideo_delayMinutes_first = 0.5f;
    // Delay between rewarded videos availability in minutes.
    public static final float rewardVideo_delayMinutes = 2.0f;
    
}
