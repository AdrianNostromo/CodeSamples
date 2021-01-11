#include "EntityIds.h"

using namespace rpg3D;

std::shared_ptr<std::string> EntityIds::playerUnit = std::make_shared<std::string>("playerUnit");

std::shared_ptr<std::string> EntityIds::groundTiledXY = std::make_shared<std::string>("groundTiledXY");
std::shared_ptr<std::string> EntityIds::underGroundTiledXY = std::make_shared<std::string>("underGroundTiledXY");
std::shared_ptr<std::string> EntityIds::wallsFullTileSizeTiledXY = std::make_shared<std::string>("wallsFullTileSizeTiledXY");

std::shared_ptr<std::string> EntityIds::defaultZoneEntrance = std::make_shared<std::string>("defaultZoneEntrance");

std::shared_ptr<std::string> EntityIds::gridOriginIndicator = std::make_shared<std::string>("gridOriginIndicator");

std::string EntityIds::asset_prefix = "asset_";

std::string EntityIds::gwFlagName_zoneEditorAssetInstanceIdCounter = "zoneEditorAssetInstanceIdCounter";
std::string EntityIds::gwFlagName_zoneEditorTileCellSizeCm = "zoneEditorTileCellSizeCm";
