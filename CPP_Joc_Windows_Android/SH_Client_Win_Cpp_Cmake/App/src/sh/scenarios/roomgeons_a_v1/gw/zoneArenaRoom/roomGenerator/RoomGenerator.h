#pragma once

#include <base/gh.h>
#include "RG_GeneratedToEntities.h"
#include <base/math/Vector3.h>
#include <memory>
#include <string>
#include "../../../gw/config/entity/EntityIds.h"

class EntityTemplate;
namespace roomgeons_a_v1 {
namespace ArenaRoom {
	class CfgRoomGenerator;
	class RoomContent;
};
};

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class RoomGenerator : public RG_GeneratedToEntities {priv typedef RG_GeneratedToEntities super;pub dCtor(RoomGenerator);
	// seed == -1; Use a random seed.
	pub explicit RoomGenerator(sp<CfgRoomGenerator> config, int seed = -1);

	pub void generate() override;
	priv void placePrefabs();
	priv void placeItems();
	priv void placeMobs();

	priv void createConditionalZoneChangeTeleporter(
		Vector3& pos, float rotZDeg,
		EntityTemplate* teleporterPlatformTemplate, std::shared_ptr<std::string> instanceId,
		EntityTemplate* brokenTeleporterPlatformTemplate);

	pub ~RoomGenerator() override;
};
};
};
