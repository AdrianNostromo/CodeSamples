package base.launcher.web;

import android.content.Intent;
import android.net.Uri;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.backends.android.AndroidApplication;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import base.launcher.IWebManager;
import local.playgames.config.PlayGamesConfig;
import local.web.UrlIDs;

//Play games manager.
public class AndroidWebManager implements IWebManager {

	private AndroidApplication activity;

    public static final Map<String, String> validatedUrlsMap = new HashMap<String, String>();

    public AndroidWebManager(AndroidApplication activity) {
        super();

	    this.activity = activity;
    }

    public void onCreate() {
        if(UrlIDs.CUSTOM_RULS_COUNT != 1) {
            Gdx.app.debug("Error", "Error. Custom urls count not implemented in the loader.");

            return ;
        }

        String url = PlayGamesConfig.URL_PLAY_STORE_APP_PAGE;
        validatedUrlsMap.put(UrlIDs.PLAY_STORE_APP_PAGE, url);
    }

    @Override
    public void openUrl(String url) {
        Uri uri = Uri.parse(url); // missing 'http://' will cause crashed
        Intent intent = new Intent(Intent.ACTION_VIEW, uri);
        activity.startActivity(intent);
    }

    @Override
    public void openUrlWithID(String urlID) {
        String url = validatedUrlsMap.get(urlID);
        if(url == null) {
            Gdx.app.debug("Error", "Error. Custom url not found in the custom urls map.");

            return ;
        }

        Uri uri = Uri.parse(url); // missing 'http://' will cause crashed
        Intent intent = new Intent(Intent.ACTION_VIEW, uri);
        activity.startActivity(intent);
    }

    @Override
    public void shareScreenshot(String externalImgPath) {
        File fh;
        try {
            fh = new File(externalImgPath);
            if(!fh.exists()) {
                Gdx.app.debug("Error", "Error. Screenshot not found.");

                return ;
            }
        }catch (Exception e) {
            Gdx.app.debug("Error", "Error while saving the screenshot.");

            return ;
        }

        Uri imageUri = Uri.fromFile(fh);
        Intent shareIntent = new Intent();
        shareIntent.setAction(Intent.ACTION_SEND);
        //shareIntent.putExtra(Intent.EXTRA_TEXT, "Can you beat my score?");
        shareIntent.putExtra(Intent.EXTRA_STREAM, imageUri);
        shareIntent.setType("image/png");
        shareIntent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
        activity.startActivity(Intent.createChooser(shareIntent, "Share"));
    }
}
