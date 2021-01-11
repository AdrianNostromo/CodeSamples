#pragma once

#include <base/gh.h>
#include "LZG_Base.h"
#include <base/list/Array1D.h>
#include <base/math/Vector2Int.h>
#include <base/math/Vector2.h>
#include <base/math/Vector3.h>

class Random;

namespace randDungeons_a_v1 {
namespace PrefabRoomsDungeonA {
	class CfgDungeonGenerator;
	class PrefabRoom;

class LevelZoneGenerator : public LZG_Base {priv typedef LZG_Base super; pub dCtor(LevelZoneGenerator);
	
    pub explicit LevelZoneGenerator(
		std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
		Random* rng);

	pub std::shared_ptr<DungeonContent::LevelZone> generate();
	
	priv void placePrefabRoom(
		PrefabRoom* prefabRoom,
		bool hasEntryPointStairs, bool hasExitStairs, bool hasStairsUp, bool hasStairsDown,
		std::shared_ptr<DungeonContent::LevelZone::Room> roomContent);

    pub ~LevelZoneGenerator() override;
};
};
};
