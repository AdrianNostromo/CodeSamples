package local.ads;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.backends.android.AndroidApplication;

import java.util.Vector;

import base.util.MathUtil;
import local.ads.adnetworks.IAdNetwork;
import local.ads.config.AdsConfig;
import local.ads.util.AdsEventListener;

/**
 * Created by adrin on 2/8/17.
 */

public abstract class AndroidAdsManagerRewardedVideo extends AndroidAdsManagerInterstitial {

    private long nextRewardVideoDelayMS = 0;
    private long lastRewardVideoTimeMS = -1;

    private boolean isRewardVideoAdDelayFinished = false;

    private long generalRewardVideoDelayMS = 0;

    private boolean isKeepRewardedVideoAdsCached = true;

    public AndroidAdsManagerRewardedVideo(AndroidApplication activity) {
        super(activity);

        //void
    }

    @Override
    public void onCreate() {
        super.onCreate();

        nextRewardVideoDelayMS = (long) Math.ceil(AdsConfig.rewardVideo_delayMinutes_first * 60 * 1000);

        generalRewardVideoDelayMS = (long) Math.ceil(AdsConfig.rewardVideo_delayMinutes * 60 * 1000);
    }

    @Override
    public void gameWorldReady() {
        super.gameWorldReady();

        lastRewardVideoTimeMS = System.currentTimeMillis();
        isKeepRewardedVideoAdsCached = true;
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(isKeepRewardedVideoAdsCached) {
            // Enumerate all networks and cache ads if needed.

            for (int i = 0; i < adNetworksList.size(); i++) {
                IAdNetwork adNetwork = adNetworksList.get(i);

                if (adNetwork.getIsNetworkInitialised()
                        && adNetwork.canSupportAndEnabledRewardedVideoAds()
                        && !adNetwork.isRewardedVideoAdAdCached()
                        && !adNetwork.getIsRewardedVideoAdCachingInProgress()) {
                    adNetwork.cacheRewardedVideoAd();
                }
            }
        }

        if(!isRewardVideoAdDelayFinished && lastRewardVideoTimeMS >= 0) {
            long tm = System.currentTimeMillis();
            long timeSinceLastVideo = tm - lastRewardVideoTimeMS;

            if(timeSinceLastVideo >= nextRewardVideoDelayMS) {
                isRewardVideoAdDelayFinished = true;

                rewardVideoAdDelayFinishedStateChanged(isRewardVideoAdDelayFinished);
            }
        }
    }

    @Override
    public boolean isAnyRewardVideoAdCached() {
        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            if(adNetwork.getIsNetworkInitialised() && adNetwork.canSupportAndEnabledRewardedVideoAds() && adNetwork.isRewardedVideoAdAdCached()) {
                return true;
            }
        }

        return false;
    }

    public boolean isRewardVideoAdDelayFinishedAndCached() {
        long tm = System.currentTimeMillis();
        long timeSinceLastAd = tm - lastRewardVideoTimeMS;
        if(timeSinceLastAd >= nextRewardVideoDelayMS) {
            Vector<IAdNetwork> adNetworks = getCachedRewardVideoAdNetworks();

            if(adNetworks.size() > 0) {
                return true;
            }
        }

        return false;
    }

    @Override
    public boolean showRewardVideoAdIfPossible(int callerID, boolean ignoreDelayTimer) {
        Gdx.app.debug("Dev-Log", "Log> Profiling. Rewarded video ad requested.");
    
        // Check if enough time passed.
        long tm = System.currentTimeMillis();
        long timeSinceLastAd = tm - lastRewardVideoTimeMS;
        if(ignoreDelayTimer || timeSinceLastAd >= nextRewardVideoDelayMS) {
            Vector<IAdNetwork> adNetworks = getCachedRewardVideoAdNetworks();

            if(adNetworks.size() <= 0) {
                return false;
            }

            int networkIndex = MathUtil.randInt(0, adNetworks.size() - 1);

            IAdNetwork finalAdNetwork = adNetworks.get(networkIndex);

            // Reset delay timers.
            lastRewardVideoTimeMS = tm;
    
            // Reset the timer only after rewarded video finished, in case it is cancelled.
            //nextRewardVideoDelayMS = generalRewardVideoDelayMS;

            if(isRewardVideoAdDelayFinished) {
                isRewardVideoAdDelayFinished = false;

                rewardVideoAdDelayFinishedStateChanged(isRewardVideoAdDelayFinished);
            }
    
            Gdx.app.debug("Dev-Log", "Log> Profiling. Rewarded video ad started.");
            
            // Show the ad.
            return finalAdNetwork.showRewardedVideoAd(callerID);
        }

        return false;
    }

    protected Vector<IAdNetwork> getCachedRewardVideoAdNetworks() {
        Vector<IAdNetwork> adNetworks = new Vector<>();

        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            if(adNetwork.getIsNetworkInitialised() && adNetwork.canSupportAndEnabledRewardedVideoAds() && adNetwork.isRewardedVideoAdAdCached()) {
                adNetworks.add(adNetwork);
            }
        }

        return adNetworks;
    }

    @Override
    public void rewardedVideoComplete(int rewardedVideoID) {
        // Reset the delay until the next ad is available here. This makes the show ad button stay on the screen if the player cancels the ad.
        nextRewardVideoDelayMS = generalRewardVideoDelayMS;
        
        for(int i=0;i<eventListenersList.size();i++) {
            AdsEventListener entry = eventListenersList.get(i);

            if(entry != null) {
                entry.rewardVideoComplete(rewardedVideoID);
            }
        }
    }

    protected final void rewardVideoAdDelayFinishedStateChanged(boolean isDelayFinished) {
        for(int i=0;i<eventListenersList.size();i++) {
            AdsEventListener entry = eventListenersList.get(i);
            if(entry != null) {
                entry.rewardVideoReadyStateStateChanged(isDelayFinished);
            }
        }
    }

}
