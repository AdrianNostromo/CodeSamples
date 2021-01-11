#include "BG_Base.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

BG_Base::BG_Base() {
	//void
}

void BG_Base::generate(std::shared_ptr<CfgDungeonGenerator> config, unsigned int seed) {
	this->config = config;

	dungeonContent = std::make_shared<DungeonContent>();
}

std::shared_ptr<DungeonContent> BG_Base::getDungeonContent() {
	return dungeonContent;
}

BG_Base::~BG_Base() {
	//void
}
