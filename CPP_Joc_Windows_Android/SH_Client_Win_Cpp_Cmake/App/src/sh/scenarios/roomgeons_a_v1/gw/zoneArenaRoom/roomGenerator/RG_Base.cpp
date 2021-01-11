#include "RG_Base.h"
#include "config/CfgRoomGenerator.h"
#include "../roomContent/RoomContent.h"
#include <base/math/Math.h>
#include <limits>

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

RG_Base::RG_Base(sp<CfgRoomGenerator> config, int seed)
	: config(config), seed(seed)
{
	//void
}

void RG_Base::generate() {
	if (seed < 0) {
		seed = Math::Rand::nextInt(0, std::numeric_limits<int>::max());
	}
	rng = new Random(seed);

	roomContent = new RoomContent{config->tileSizeCm};

	roomPrefabsCount_innerOnly.set(config->roomPrefabsCount);
	roomPrefabsCount_withExitHallways.set(config->roomPrefabsCount.x + 2, config->roomPrefabsCount.y + 2);
}

sp<RoomContent> RG_Base::getRoomContent() {
	return roomContent;
}

RG_Base::~RG_Base() {
	if (rng != nullptr) {
		delete rng;
		rng = nullptr;
	}
}
