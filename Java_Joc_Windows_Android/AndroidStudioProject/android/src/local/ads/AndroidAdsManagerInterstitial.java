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

public abstract class AndroidAdsManagerInterstitial extends AndroidAdsManagerNetworks {

    private long nextInterstitialDelayMS = 0;
    private long lastInterstitialTimeMS = -1;

    private boolean isInterstitialAdDelayFinished = false;

    private long generalInterstitialDelayMS = 0;

    private boolean isKeepInterstitialAdsCached = true;

    public AndroidAdsManagerInterstitial(AndroidApplication activity) {
        super(activity);

        //void
    }

    @Override
    public void onCreate() {
        super.onCreate();

        nextInterstitialDelayMS = (long) Math.ceil(AdsConfig.interstitial_delayMinutes_first * 60 * 1000);

        generalInterstitialDelayMS = (long)Math.ceil(AdsConfig.interstitial_delayMinutes * 60 * 1000);
    }

    @Override
    public void gameWorldReady() {
        super.gameWorldReady();

        lastInterstitialTimeMS = System.currentTimeMillis();
        isKeepInterstitialAdsCached = true;
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        super.appLoop(deltaS, deltaMS);

        if(isKeepInterstitialAdsCached) {
            // Enumerate all networks and cache ads if needed.

            for(int i = 0; i< adNetworksList.size(); i++) {
                IAdNetwork adNetwork = adNetworksList.get(i);

                if (adNetwork.getIsNetworkInitialised()
                        && adNetwork.canSupportAndEnabledInterstitialAds()
                        && !adNetwork.isInterstitialAdCached()
                        && !adNetwork.getIsInterstitialAdCachingInProgress()) {
                    adNetwork.cacheInterstitialAd();
                }
            }
        }

        if(!isInterstitialAdDelayFinished && lastInterstitialTimeMS >= 0) {
            long tm = System.currentTimeMillis();
            long timeSinceLastVideo = tm - lastInterstitialTimeMS;

            if(timeSinceLastVideo >= nextInterstitialDelayMS) {
                isInterstitialAdDelayFinished = true;

                interstitialAdDelayFinishedStateChanged(isInterstitialAdDelayFinished);
            }
        }
    }

    @Override
    public boolean isAnyInterstitialAdCached() {
        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            if(adNetwork.getIsNetworkInitialised() && adNetwork.canSupportAndEnabledInterstitialAds() && adNetwork.isInterstitialAdCached()) {
                return true;
            }
        }

        return false;
    }

    @Override
    public boolean isInterstitialAdDelayFinishedAndCached() {
        long tm = System.currentTimeMillis();
        long timeSinceLastAd = tm - lastInterstitialTimeMS;
        if(timeSinceLastAd >= nextInterstitialDelayMS) {
            Vector<IAdNetwork> adNetworks = getCachedInterstitialAdNetworks();

            if (adNetworks.size() > 0) {
                return true;
            }
        }

        return false;
    }

    @Override
    public boolean showInterstitialAdIfPossible(boolean ignoreDelayTimer) {
        Gdx.app.debug("Dev-Log", "Log> Profiling. Interstitial ad requested.");
        
        // Check if enough time passed.
        long tm = System.currentTimeMillis();
        long timeSinceLastAd = tm - lastInterstitialTimeMS;
        if(ignoreDelayTimer || timeSinceLastAd >= nextInterstitialDelayMS) {
            Vector<IAdNetwork> adNetworks = getCachedInterstitialAdNetworks();

            if(adNetworks.size() <= 0) {
                return false;
            }

            int networkIndex = MathUtil.randInt(0, adNetworks.size() - 1);

            IAdNetwork finalAdNetwork = adNetworks.get(networkIndex);

            // Reset delay timers.
            lastInterstitialTimeMS = tm;

            nextInterstitialDelayMS = generalInterstitialDelayMS;

            if(isInterstitialAdDelayFinished) {
                isInterstitialAdDelayFinished = false;

                interstitialAdDelayFinishedStateChanged(isInterstitialAdDelayFinished);
            }
    
            Gdx.app.debug("Dev-Log", "Log> Profiling. Interstitial ad started.");
            
            return finalAdNetwork.showInterstitialAd();
        }

        return false;
    }

    protected Vector<IAdNetwork> getCachedInterstitialAdNetworks() {
        Vector<IAdNetwork> adNetworks = new Vector<>();

        for(int i = 0; i< adNetworksList.size(); i++) {
            IAdNetwork adNetwork = adNetworksList.get(i);

            if(adNetwork.getIsNetworkInitialised() && adNetwork.canSupportAndEnabledInterstitialAds() && adNetwork.isInterstitialAdCached()) {
                adNetworks.add(adNetwork);
            }
        }

        return adNetworks;
    }

    protected final void interstitialAdDelayFinishedStateChanged(boolean isDelayFinished) {
        for(int i=0;i<eventListenersList.size();i++) {
            AdsEventListener entry = eventListenersList.get(i);
            if(entry != null) {
                entry.interstitialAdDelayFinishedStateChanged(isDelayFinished);
            }
        }
    }

}
