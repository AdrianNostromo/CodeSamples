#pragma once

#include <base/gh.h>
#include <base/memory/SharedPointer.h>
#include <base/math/Random.h>
#include <base/math/Vector2Int.h>

namespace roomgeons_a_v1 {
namespace ArenaRoom {
	class CfgRoomGenerator;
	class RoomContent;
};
};

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class RG_Base {pub dCtor(RG_Base);
	prot sp<CfgRoomGenerator> config = nullptr;
	prot sp<RoomContent> roomContent = nullptr;

	pub int seed;
	prot Random* rng = nullptr;

	pub Vector2Int roomPrefabsCount_innerOnly{};
	pub Vector2Int roomPrefabsCount_withExitHallways{};

	// seed == -1; Use a random seed.
    pub explicit RG_Base(sp<CfgRoomGenerator> config, int seed=-1);

    pub virtual void generate();

	pub sp<RoomContent> getRoomContent();

    pub virtual ~RG_Base();
};
};
};
