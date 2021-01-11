package base.view.popupmenu.util;

/**
 * Created by Adrin on 6/23/2016.
 */
public class PopupConfig {

    public static final PopupConfig SCREENSHOT_SAVE_ERROR = new PopupConfig("SCREENSHOT_SAVE_ERROR", "Failed To Save Screenshot", null, 0, false);
    public static final PopupConfig WAITING_FOR_CONNECTION = new PopupConfig("WAITING_FOR_CONNECTION", "Waiting For Play Games Connection", null, 0, false);
    public static final PopupConfig LEADERBOARD_SHOW_FAILED = new PopupConfig("LEADERBOARD_SHOW_FAILED", "Leaderboard Error Occurred", null, 0, false);
    public static final PopupConfig ACHIEVEMENTS_SHOW_FAILED = new PopupConfig("ACHIEVEMENTS_SHOW_FAILED", "Achievements Error Occurred", null, 0, false);
    public static final PopupConfig PLAY_GAMES_CONNECTION_REQUIRED = new PopupConfig("PLAY_GAMES_CONNECTION_REQUIRED", "Play Games Connection Required", "CONNECT", 88f, true);
    public static final PopupConfig SELECT_A_COLOR = new PopupConfig("SELECT_A_COLOR", "Select a color", null, 0, false);
    public static final PopupConfig UPLOADED_WORKSHOP_ASSET_IS_LOCKED = new PopupConfig("UPLOADED_WORKSHOP_ASSET_IS_LOCKED", "Locked. Make a duplicate", null, 0, false);
    public static final PopupConfig WORKSHOP_ASSET_FILES_SAVE_FAILED = new PopupConfig("WORKSHOP_ASSET_FILES_SAVE_FAILED", "Error saving asset image.", null, 0, false);
    public static final PopupConfig WORKSHOP_ASSET_IMAGE_LOAD_ERROR = new PopupConfig("WORKSHOP_ASSET_IMAGE_LOAD_ERROR", "Asset image load error.", null, 0, false);
    public static final PopupConfig INTERNET_ERROR = new PopupConfig("INTERNET_ERROR", "Internet not found (-.-)Zzz", null, 0, false);

    public static final PopupConfig STOP_MATCH_FIND_FIRST = new PopupConfig("STOP_MATCH_FIND_FIRST", "Stop game find first", null, 0, false);

    public String id;

    public String msg;

    //asd_001001;// use a DynamicTextButtonConfig object for this template and the touch_area_width template.
    public String actionText;
    //asd_001001;// refactor to use dpm width, not screen pixels width. On certain devices, the touch area will have a wrong rendering3DSize.
    public float touchAreaWidth;

    public boolean isYellowActionEnabled;

    public PopupConfig(String id, String msg, String actionText, float touchAreaWidth, boolean isYellowActionEnabled) {
        super();

        this.id = id;

        this.msg = msg;

        this.actionText = actionText;
        this.touchAreaWidth = touchAreaWidth;

        this.isYellowActionEnabled = isYellowActionEnabled;
    }

}
