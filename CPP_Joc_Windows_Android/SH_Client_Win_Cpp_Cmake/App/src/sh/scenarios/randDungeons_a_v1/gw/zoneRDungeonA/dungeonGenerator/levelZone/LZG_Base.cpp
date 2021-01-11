#include "LZG_Base.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

LZG_Base::LZG_Base(
	std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
	Random* rng)
	: config(config), zoneIndex(zoneIndex),
	rng(rng)
{
	levelZone = std::make_shared<DungeonContent::LevelZone>();

	//void
}

LZG_Base::~LZG_Base() {
	//void
}
