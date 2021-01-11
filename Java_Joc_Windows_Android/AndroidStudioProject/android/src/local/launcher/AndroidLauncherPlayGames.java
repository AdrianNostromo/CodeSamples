package local.launcher;

import android.content.Intent;
import android.os.Bundle;
import android.widget.RelativeLayout;

import base.launcher.AndroidLauncherWeb;
import local.playgames.AndroidPlayGamesManager;
import local.playgames.launcher.IPlayGamesManager;

/**
 * Created by adrin on 2/2/17.
 */

public abstract class AndroidLauncherPlayGames extends AndroidLauncherWeb {

    private AndroidPlayGamesManager playGamesManager;

    @Override
    protected void onCreate_preAppMainView(Bundle savedInstanceState, RelativeLayout layout) {
        super.onCreate_preAppMainView(savedInstanceState, layout);

        playGamesManager = new AndroidPlayGamesManager(this);
        if(playGamesManager != null) {
            playGamesManager.onCreate();
        }
    }

    @Override
    public void onStart() {
        super.onStart();

        if(playGamesManager != null) {
            playGamesManager.onStart();
        }
    }

    @Override
    public void onStop() {
        super.onStop();

        if(playGamesManager != null) {
            playGamesManager.onStop();
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if(playGamesManager != null) {
            playGamesManager.onActivityResult(requestCode, resultCode, data);
        }
    }

    protected IPlayGamesManager getPlayGamesManager() {
        return playGamesManager;
    }

}
