package base.profile;

import base.launcher.ILocalDataHandler;
import local.app.AppLocal;

//The player profile handles persistent game information.
public abstract class PlayerProfileData extends PlayerProfileDataBase {

    public PlayerProfileData(AppLocal app, ILocalDataHandler localDataHandler) {
        super(app, localDataHandler);

	    //void
    }

}
