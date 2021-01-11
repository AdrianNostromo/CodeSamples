package base.launcher;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.RelativeLayout;

import com.badlogic.gdx.backends.android.AndroidApplication;
import com.badlogic.gdx.backends.android.AndroidApplicationConfiguration;

import local.MainLocal;

/**
 * Created by adrin on 2/2/17.
 */

public abstract class AndroidLauncherBase extends AndroidApplication implements IAppLauncher {

    @Override
    protected final void onCreate (Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Create a new android application configuration object.
        AndroidApplicationConfiguration config = new AndroidApplicationConfiguration();
        // Use full screen mode. This hides the status bar and on screen navigation buttons.
        config.useImmersiveMode = true;
        config.hideStatusBar = true;
        config.useWakelock = true;

        config.useCompass = false;
        config.useAccelerometer = false;

        // Create the layout
        RelativeLayout layout = new RelativeLayout(this);

		/*
		// Manual window features can be changed here.
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
		*/

        onCreate_preAppMainView(savedInstanceState, layout);

        // Create the libgdx View.
        View gameView = initializeForView(
                createAppCoreMain(),
                config
        );

        // Add the libgdx view.
        layout.addView(gameView);

        // Hook it all up.
        setContentView(layout);
    }

    protected void onCreate_preAppMainView(Bundle savedInstanceState, RelativeLayout layout) {
        //void
    }

    // Fully extendable.
    protected MainLocal createAppCoreMain() {
        return null;
    }

    @Override
    public void appLoop(float deltaS, int deltaMS) {
        //void
    }

    @Override
    public void onStart() {
        super.onStart();

        //void
    }

    @Override
    public void onResume() {
        super.onResume();

        //void
    }

    @Override
    public void onPause() {
        super.onPause();

        //void
    }

    @Override
    public void onStop() {
        onStop_pre();

        super.onStop();

        //void
    }

    protected void onStop_pre() {
        //void
    }

    // This method may not be called because a extender can inhibit this.
    @Override
    public void onBackPressed() {
        super.onBackPressed();

        //void
    }

    @Override
    public void onDestroy() {
        //void

        super.onDestroy();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        //void
    }

}
