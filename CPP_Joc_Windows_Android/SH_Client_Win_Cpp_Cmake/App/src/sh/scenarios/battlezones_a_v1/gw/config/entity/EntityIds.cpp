#include "EntityIds.h"

using namespace battlezones_a_v1;

std::shared_ptr<std::string> EntityIds::defaultSpawnLocation = std::make_shared<std::string>("defaultSpawnLocation");
std::shared_ptr<std::string> EntityIds::platform_Teleporter_Home = std::make_shared<std::string>("platform_Teleporter_Home");

std::shared_ptr<std::string> EntityIds::platform_zonesGroupStart = std::make_shared<std::string>("platform_zonesGroupStart");

std::shared_ptr<std::string> EntityIds::platform_zoneTeleporter_next = std::make_shared<std::string>("platform_zoneTeleporter_next");
std::shared_ptr<std::string> EntityIds::platform_zoneTeleporter_previous = std::make_shared<std::string>("platform_zoneTeleporter_previous");
