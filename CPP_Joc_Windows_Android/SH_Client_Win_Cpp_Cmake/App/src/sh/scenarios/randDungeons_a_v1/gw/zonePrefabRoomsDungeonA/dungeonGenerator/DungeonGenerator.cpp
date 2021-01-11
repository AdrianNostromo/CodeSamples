#include "DungeonGenerator.h"
#include "levelZone/LevelZoneGenerator.h"
#include <sh/scenarios/randDungeons_a_v1/gw/config/entity/EntityIds.h>
#include <base/math/Random.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::PrefabRoomsDungeonA;

DungeonGenerator::DungeonGenerator() {
	//void
}

void DungeonGenerator::generate(std::shared_ptr<CfgDungeonGenerator> config, unsigned int seed) {
	super::generate(config, seed);

	Random rng{ seed };

	for (int levelZoneIndex = 0; levelZoneIndex < config->zoneFloorsCount; levelZoneIndex++) {
		LevelZoneGenerator levelZoneGenerator{
			config, levelZoneIndex,
			&rng
		};

		std::shared_ptr<DungeonContent::LevelZone> levelZoneContent = levelZoneGenerator.generate();

		dungeonContent->levelZonesList.appendReference(levelZoneContent);
	}

	dungeonContent->seed = seed;
	dungeonContent->entryPointLevelZonIndex = 0;
	dungeonContent->entryPointPosEntityInstanceId = EntityIds::teleportDungeonEntryPoint;
}

DungeonGenerator::~DungeonGenerator() {
	//void
}
