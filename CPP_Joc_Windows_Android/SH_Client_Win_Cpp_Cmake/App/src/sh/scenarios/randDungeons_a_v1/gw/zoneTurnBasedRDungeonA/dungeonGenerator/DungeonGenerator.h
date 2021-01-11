#pragma once

#include <base/gh.h>
#include "BG_Base.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class DungeonGenerator : public BG_Base {priv typedef BG_Base super;pub dCtor(DungeonGenerator);
	pub explicit DungeonGenerator();

	pub void generate(std::shared_ptr<CfgDungeonGenerator> config, unsigned int seed) override;

	pub ~DungeonGenerator() override;
};
};
};
