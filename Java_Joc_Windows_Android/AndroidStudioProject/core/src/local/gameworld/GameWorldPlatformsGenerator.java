package local.gameworld;

import base.util.MathUtil;
import local.app.AppLocal;
import local.gameworld.util.ItemConfig;
import local.gameworld.util.PlatformConfig;
import local.gameworld.util.WorldItem;
import local.gameworld.util.WorldPlatform;

public class GameWorldPlatformsGenerator extends GameWorldCamera {

    private int lastPlatformRow;
    private float nextPlatformPosY;
    private float timeUntilNextPlatformS;

    private int nextPlatformPoint = 5;

    private int currentPlatformsGroupSizeRemaining = 0;
    private PlatformConfig currentPlatformsGroupConfig = null;

    private final float interPlatformsSpacingMin = 0.0f;
    private final float interPlatformsSpacingMax = 0.0f;
    private final float platformsSpawnHorizon = 30f;

    private static final float interPlatformSpawnDelayS = 0.2f;
    private final PlatformConfig firstPlatformTemplate = PlatformConfig.Platform_2x6;
    private final PlatformConfig[] spawnedPlatformTemplates = {
            PlatformConfig.Platform_2x2,
            PlatformConfig.Platform_2x3,
            PlatformConfig.Platform_2x4
    };

    // The frequency of collectible sphere items in the range [0, 1];
    private final float itemSpawnFrequency = 0.2f;

    public GameWorldPlatformsGenerator(AppLocal app) {
        super(app);

        //void
    }

    @Override
    public void create() {
        super.create();

        nextPlatformPosY = -1f;

        handlePlatformSpawn(firstPlatformTemplate, 0, false);
    }

    private void handlePlatformSpawn(PlatformConfig platformTemplate, int tubeRowIndex, boolean doAnimation) {
        WorldPlatform entry = createPlatform(platformTemplate, tubeRowIndex, nextPlatformPosY, doAnimation);

        lastPlatformRow = tubeRowIndex;
        nextPlatformPosY = entry.posY + entry.config.rendering3DSize.y + interPlatformsSpacingMin + (interPlatformsSpacingMax - interPlatformsSpacingMin) * (float)Math.random();
        timeUntilNextPlatformS = interPlatformSpawnDelayS;
    }

    private void handleItemSpawn(ItemConfig itemConfig, int tubeRowIndex, float posY, boolean doAnimation) {
        WorldItem entry = createItem(itemConfig, tubeRowIndex, posY, doAnimation);
    }

    @Override
    protected void gameLoop(float deltaS, int deltaMS) {
        super.gameLoop(deltaS, deltaMS);

        if(!getIsLevelEnded()) {
            if(timeUntilNextPlatformS > 0) {
                timeUntilNextPlatformS = Math.max(0, timeUntilNextPlatformS - deltaS);
            }

            if (playerUnit.pos.y + platformsSpawnHorizon >= nextPlatformPosY && timeUntilNextPlatformS <= 0) {
                PlatformConfig platformConfig = null;

                currentPlatformsGroupSizeRemaining--;
                if(currentPlatformsGroupSizeRemaining > 0) {
                    platformConfig = currentPlatformsGroupConfig;
                }
                if(platformConfig == null) {
                    int platformTemplateIndex;
                    if (spawnedPlatformTemplates.length <= 1) {
                        platformTemplateIndex = 0;
                    } else {
                        platformTemplateIndex = MathUtil.randInt(0, spawnedPlatformTemplates.length - 1);
                    }

                    currentPlatformsGroupSizeRemaining = MathUtil.randInt(8, 14);
                    currentPlatformsGroupConfig = spawnedPlatformTemplates[platformTemplateIndex];

                    platformConfig = currentPlatformsGroupConfig;
                }

                int direction;
                float randomValue = (float)Math.random();
                if(randomValue > 0.5) {
                    direction = 1;
                }else {
                    direction = -1;
                }

                //asd_tst_001; int newPlatformTubeRow = lastPlatformRow + direction;
                int newPlatformTubeRow = lastPlatformRow + 1;//asd_tst_001; remove

                if(newPlatformTubeRow < 0) {
                    newPlatformTubeRow += TUBE_ROWS_COUNT;
                }
                newPlatformTubeRow = newPlatformTubeRow % TUBE_ROWS_COUNT;

                float spawnedPlatformPosY = nextPlatformPosY;
                handlePlatformSpawn(platformConfig, newPlatformTubeRow, true);

                if(!isTutorialModeActive) {
                    //asd_tst_001; nextPlatformPoint--;
                    if (nextPlatformPoint <= 0) {
                        // Create a item after 10 platforms.
                        nextPlatformPoint = 10;
                        //nextPlatformPoint = MathUtil.randInt(8, 18);

                        float itemSpawnPosY = spawnedPlatformPosY + platformConfig.rendering3DSize.y / 2;

                        handleItemSpawn(ItemConfig.PointItemA, newPlatformTubeRow, itemSpawnPosY, true);
                    }
                }
            }
        }
    }

}
