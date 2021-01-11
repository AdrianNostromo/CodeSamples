package base.launcher.localdata;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;

import base.launcher.ILocalDataHandler;

// This class loads and saves key, value pairs for data for the player profile.
public class AndroidLocalDataHandler implements ILocalDataHandler {

    private Activity activity;

    private SharedPreferences sharedPref;

    public AndroidLocalDataHandler(Activity activity) {
        super();

        this.activity = activity;
    }

    // Get the default application preferences object.
    public void load() {
        sharedPref = activity.getPreferences(Context.MODE_PRIVATE);
    }

    // Get the int value of a data entry.
    @Override
    public int getInt(String key, int defaultValue) {
        int value = sharedPref.getInt(key, defaultValue);
        return value;
    }

    // Set the int value of a data entry.
    @Override
    public void putInt(String key, int value) {
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.putInt(key, value);

        editor.commit();
    }

    // Get the int value of a data entry.
    @Override
    public String getString(String key, String defaultValue) {
        String value = sharedPref.getString(key, null);
        return value;
    }

    // Set the int value of a data entry.
    @Override
    public void putString(String key, String value) {
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.putString(key, value);

        editor.commit();
    }

    @Override
    public boolean getBoolean(String key, boolean defaultValue) {
        boolean value = sharedPref.getBoolean(key, defaultValue);
        return value;
    }

    @Override
    public void putBoolean(String key, boolean value) {
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.putBoolean(key, value);

        editor.commit();
    }

    @Override
    public void removeEntry(String key) {
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.remove(key);

        editor.commit();
    }

    @Override
    public void clearData() {
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.clear();

        editor.commit();
    }
}
