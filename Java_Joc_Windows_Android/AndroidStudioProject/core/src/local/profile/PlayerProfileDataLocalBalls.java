package local.profile;

import base.launcher.ILocalDataHandler;
import local.app.AppLocal;

//The player profile handles persistent game information.
public class PlayerProfileDataLocalBalls extends PlayerProfileDataLocalPlayerGems {

    public static final String ACTIVE_BALL_STYLE_ID = "ACTIVE_BALL_STYLE_ID";
    
    public static final String IS_UNLOCKED_BALL_ID_ = "IS_UNLOCKED_BALL_ID_";

    public PlayerProfileDataLocalBalls(AppLocal app, ILocalDataHandler localDataHandler) {
        super(app, localDataHandler);

	    //void
    }
    
    public String getActiveBallStyleId() {
        return localDataHandler.getString(ACTIVE_BALL_STYLE_ID, null);
    }
    
    public void putActiveBallStyleId(String ballStyleId) {
        localDataHandler.putString(ACTIVE_BALL_STYLE_ID, ballStyleId);
        
        dispatchEvent_profileDataChanged(ACTIVE_BALL_STYLE_ID);
    }
    
    public boolean getIsBallStyleUnlocked(String ballStyleId) {
        return localDataHandler.getBoolean(IS_UNLOCKED_BALL_ID_ + ballStyleId, false);
    }
    
    public void putIsBallStyleUnlocked(String ballStyleId, boolean isUnlocked) {
        localDataHandler.putBoolean(IS_UNLOCKED_BALL_ID_ + ballStyleId, isUnlocked);
        
        dispatchEvent_profileDataChanged(IS_UNLOCKED_BALL_ID_ + ballStyleId);
    }
    
}
