#include "EntityIds.h"

using namespace roomgeons_a_v1;

std::shared_ptr<std::string> EntityIds::nextZoneInSequenceTeleporter = std::make_shared<std::string>("nextZoneInSequenceTeleporter");
std::shared_ptr<std::string> EntityIds::homeTeleporter = std::make_shared<std::string>("homeTeleporter");
std::shared_ptr<std::string> EntityIds::respawnLocationPlatform = std::make_shared<std::string>("respawnLocationPlatform");
std::shared_ptr<std::string> EntityIds::zoneEntranceIndicator = std::make_shared<std::string>("zoneEntranceIndicator");
