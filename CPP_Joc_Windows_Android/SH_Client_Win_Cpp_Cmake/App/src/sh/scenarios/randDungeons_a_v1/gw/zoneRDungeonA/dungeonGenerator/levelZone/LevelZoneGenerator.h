#pragma once

#include <base/gh.h>
#include "LZG_RoomsTemplating.h"
#include <base/list/Array1D.h>
#include <base/math/Vector2Int.h>
#include <base/math/Vector2.h>
#include <base/math/Vector3.h>

class Random;

namespace randDungeons_a_v1 {
namespace RDungeonA {
	class CfgDungeonGenerator;

class LevelZoneGenerator : public LZG_RoomsTemplating {priv typedef LZG_RoomsTemplating super; pub dCtor(LevelZoneGenerator);
	
    pub explicit LevelZoneGenerator(
		std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
		Random* rng);

	pub std::shared_ptr<DungeonContent::LevelZone> generate();
	
    pub ~LevelZoneGenerator() override;
};
};
};
