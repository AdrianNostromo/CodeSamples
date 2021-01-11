package local.profile;

import base.launcher.ILocalDataHandler;
import base.profile.PlayerProfileData;
import local.app.AppLocal;

//The player profile handles persistent game information.
public class PlayerProfileDataLocalPlayerGems extends PlayerProfileData {

	public static final String CURRENCY_PLAYER_GEMS = "CURRENCY_PLAYER_GEMS";

    //asd_002;// dispatch a animated currency value, it is separate: CURRENCY_PLAYER_GEMS_ANIMATED.

    public PlayerProfileDataLocalPlayerGems(AppLocal app, ILocalDataHandler localDataHandler) {
        super(app, localDataHandler);

	    //void
    }

    public int getPlayerGems() {
	    return localDataHandler.getInt(CURRENCY_PLAYER_GEMS, 0);
    }

    public void putPlayerGems(int gems) {
	    localDataHandler.putInt(CURRENCY_PLAYER_GEMS, gems);

	    dispatchEvent_profileDataChanged(CURRENCY_PLAYER_GEMS);
    }

    public void addPlayerGems(int gemsAddValue) {
	    putPlayerGems(getPlayerGems() + gemsAddValue);
    }

}
