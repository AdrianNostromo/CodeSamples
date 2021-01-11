#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <rpg3D/gw/config/entity/EntityIds.h>

namespace roomgeons_a_v1 {
class EntityIds : public rpg3D::EntityIds {
	pub static std::shared_ptr<std::string> nextZoneInSequenceTeleporter;
	pub static std::shared_ptr<std::string> homeTeleporter;
	pub static std::shared_ptr<std::string> respawnLocationPlatform;
	pub static std::shared_ptr<std::string> zoneEntranceIndicator;

};
};
