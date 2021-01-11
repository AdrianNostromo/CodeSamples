package local.launcher;

import local.MainLocal;

/**
 * Created by adrin on 2/2/17.
 */

public class AndroidLauncherTop extends AndroidLauncherAds {

    @Override
    protected MainLocal createAppCoreMain() {
        // return super.createAppCoreMain();

        MainLocal ret = new MainLocal(
                this,
                getLocalDataHandler(),
                getAdsManager(),
                getPlayGamesManager(),
                getWebManager()
        );

        return ret;
    }

}
