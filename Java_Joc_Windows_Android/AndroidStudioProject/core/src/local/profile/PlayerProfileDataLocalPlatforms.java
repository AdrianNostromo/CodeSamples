package local.profile;

import base.launcher.ILocalDataHandler;
import local.app.AppLocal;

//The player profile handles persistent game information.
public class PlayerProfileDataLocalPlatforms extends PlayerProfileDataLocalBalls {

    public static final String ACTIVE_PLATFORM_STYLE_ID = "ACTIVE_PLATFORM_STYLE_ID";

    public static final String IS_UNLOCKED_PLATFORM_ID_ = "IS_UNLOCKED_PLATFORM_ID_";

    public PlayerProfileDataLocalPlatforms(AppLocal app, ILocalDataHandler localDataHandler) {
        super(app, localDataHandler);

	    //void
    }

    public String getActivePlatformStyleId() {
        return localDataHandler.getString(ACTIVE_PLATFORM_STYLE_ID, null);
    }

    public void putActivePlatformStyleId(String platformStyleId) {
        localDataHandler.putString(ACTIVE_PLATFORM_STYLE_ID, platformStyleId);

        dispatchEvent_profileDataChanged(ACTIVE_PLATFORM_STYLE_ID);
    }

    public boolean getIsPlatformStyleUnlocked(String platformStyleId) {
        return localDataHandler.getBoolean(IS_UNLOCKED_PLATFORM_ID_ + platformStyleId, false);
    }

    public void putIsPlatformStyleUnlocked(String platformStyleId, boolean isUnlocked) {
        localDataHandler.putBoolean(IS_UNLOCKED_PLATFORM_ID_ + platformStyleId, isUnlocked);

        dispatchEvent_profileDataChanged(IS_UNLOCKED_PLATFORM_ID_ + platformStyleId);
    }

}
