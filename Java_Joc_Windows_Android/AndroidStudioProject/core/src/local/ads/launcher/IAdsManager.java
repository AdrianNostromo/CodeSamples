package local.ads.launcher;

import local.ads.util.AdsEventListener;

// Interface for ads control.
public interface IAdsManager {

    void gameWorldReady();

	void addEventsListener(AdsEventListener eventsListener);
	void removeEventsListener(AdsEventListener eventsListener);

    boolean isAnyInterstitialAdCached();
	boolean isInterstitialAdDelayFinishedAndCached();
	boolean showInterstitialAdIfPossible(boolean ignoreDelayTimer);

    boolean isAnyRewardVideoAdCached();
	boolean isRewardVideoAdDelayFinishedAndCached();
	boolean showRewardVideoAdIfPossible(int callerID, boolean ignoreDelayTimer);

}
