#include "EntityIds.h"

using namespace randDungeons_a_v1;

std::shared_ptr<std::string> EntityIds::enterRandomDungeonA = std::make_shared<std::string>("enterRandomDungeonA");
std::shared_ptr<std::string> EntityIds::enterTurnBasedRandomDungeonA = std::make_shared<std::string>("enterTurnBasedRandomDungeonA");

std::shared_ptr<std::string> EntityIds::enterPrefabRoomsDungeonA = std::make_shared<std::string>("enterPrefabRoomsDungeonA");

std::shared_ptr<std::string> EntityIds::teleportDungeonEntryPoint = std::make_shared<std::string>("teleportDungeonEntryPoint");
std::shared_ptr<std::string> EntityIds::teleporterExitDungeon = std::make_shared<std::string>("teleporterExitDungeon");
std::string EntityIds::teleportPrefix = "teleport:";
