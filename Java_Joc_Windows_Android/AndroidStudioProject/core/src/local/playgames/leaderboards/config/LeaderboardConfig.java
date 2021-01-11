package local.playgames.leaderboards.config;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

/**
 * Created by adrin on 2/6/17.
 */

public class LeaderboardConfig {

    public static final Map<String, LeaderboardConfig> entriesMap = new HashMap<String, LeaderboardConfig>();
    public static final Vector<LeaderboardConfig> entriesList = new Vector<LeaderboardConfig>();

    public static final int TYPE_HIGHEST_VALUE = 1;
    public static final int TYPE_ADDITIVE = 2;
    public static final int TYPE_SINGLE_INCREMENT_BY_1 = 3;

    public static final String PROFILE_DATA_ID_LEADERBOARD_TO_UPLOAD_TotalScore = "_leaderboardUploadData_TotalScore";

    public static final LeaderboardConfig TotalScore = new LeaderboardConfig(
            "TotalScore",
            TYPE_ADDITIVE,
            PROFILE_DATA_ID_LEADERBOARD_TO_UPLOAD_TotalScore
    );

    public String leaderboardID;

    public int type;

    public String activeUploadProfileDataId;

    public LeaderboardConfig(String leaderboardID, int type, String activeUploadProfileDataId) {
        super();

        this.leaderboardID = leaderboardID;

        this.type = type;

        this.activeUploadProfileDataId = activeUploadProfileDataId;

        entriesMap.put(leaderboardID, this);
        entriesList.add(this);
    }

}
