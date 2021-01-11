package local.playgames.achievements.config;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class AchievementConfig {

    public static final Map<String, AchievementConfig> entriesMap = new HashMap<String, AchievementConfig>();
    public static final Vector<AchievementConfig> entriesList = new Vector<AchievementConfig>();

    // Unlocked after finishing the tutorial.
    public static final AchievementConfig TUTORIALIZED = new AchievementConfig("TUTORIALIZED", false);

    // Incremented by 1 after a level end.
    public static final AchievementConfig RUNNER_I = new AchievementConfig("RUNNER_I", false);
    // Incremented by 1 after a level end.
    public static final AchievementConfig RUNNER_II = new AchievementConfig("RUNNER_II", true);
    // Incremented by 1 after a level end.
    public static final AchievementConfig RUNNER_III = new AchievementConfig("RUNNER_III", true);

    // Unlocked if collected 2 gems in a single run.
    public static final AchievementConfig GEMOMATIC_I = new AchievementConfig("GEMOMATIC_I", false);
    // Unlocked if collected 5 gems in a single run.
    public static final AchievementConfig GEMOMATIC_II = new AchievementConfig("GEMOMATIC_II", false);
    // Unlocked if collected 10 gems in a single run.
    public static final AchievementConfig GEMOMATIC_III = new AchievementConfig("GEMOMATIC_III", false);

    public static final AchievementConfig COLLECT_100_GEMS_ON_SINGLE_RUN = new AchievementConfig("achievement_gemomatic", false);

    public String achievementId;

    public boolean isIncremental;

    public AchievementConfig(String achievementId, boolean isIncremental) {
        super();

        this.achievementId = achievementId;

        this.isIncremental = isIncremental;

        entriesMap.put(achievementId, this);
        entriesList.add(this);
    }

}
