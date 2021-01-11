#pragma once

#include <base/gh.h>
#include "../dungeonContent/DungeonContent.h"
#include "config/CfgDungeonGenerator.h"
#include <memory>

namespace randDungeons_a_v1 {
namespace RDungeonA {
class BG_Base {pub dCtor(BG_Base);
	prot std::shared_ptr<CfgDungeonGenerator> config = nullptr;
	prot std::shared_ptr<DungeonContent> dungeonContent = nullptr;

    pub explicit BG_Base();

    pub virtual void generate(std::shared_ptr<CfgDungeonGenerator> config, unsigned int seed);

	pub std::shared_ptr<DungeonContent> getDungeonContent();

    pub virtual ~BG_Base();
};
};
};
