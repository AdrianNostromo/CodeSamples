#pragma once

#include <base/gh.h>
#include <string>
#include <memory>

namespace rpg3D {
class EntityIds {
    pub static std::shared_ptr<std::string> playerUnit;

    // Multiple ground entities can have this instanceId if they are layered on the z direction.
    pub static std::shared_ptr<std::string> groundTiledXY;
    pub static std::shared_ptr<std::string> underGroundTiledXY;
    // These have the thickness of a ground tile.
    pub static std::shared_ptr<std::string> wallsFullTileSizeTiledXY;

    // This is where the player is spawned by default (unless otherwise specified).
    pub static std::shared_ptr<std::string> defaultZoneEntrance;

    pub static std::shared_ptr<std::string> gridOriginIndicator;

    //asd_01;// use this;
    pub static std::string asset_prefix;

    pub static std::string gwFlagName_zoneEditorAssetInstanceIdCounter;
    pub static std::string gwFlagName_zoneEditorTileCellSizeCm;

};
};
