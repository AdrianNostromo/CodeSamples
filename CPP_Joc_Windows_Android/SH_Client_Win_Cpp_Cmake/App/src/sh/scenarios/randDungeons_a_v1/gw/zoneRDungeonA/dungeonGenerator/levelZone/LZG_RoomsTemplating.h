#pragma once

#include <base/gh.h>
#include "LZG_CustomQueries.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class LZG_RoomsTemplating : public LZG_CustomQueries {priv typedef LZG_CustomQueries super;pub dCtor(LZG_RoomsTemplating);
	pub explicit LZG_RoomsTemplating(
        std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
        Random* rng);

	prot void buildRoomConfigs();
	
    pub ~LZG_RoomsTemplating() override;
};
};
};
