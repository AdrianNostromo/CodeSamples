package base.launcher;

// Interface for the local data handler.
public interface ILocalDataHandler {

    int getInt(String key, int defaultValue);
    void putInt(String key, int value);

    boolean getBoolean(String key, boolean defaultValue);
    void putBoolean(String key, boolean value);

    String getString(String key, String defaultValue);
    void putString(String key, String value);

    void removeEntry(String key);
    void clearData();

}
