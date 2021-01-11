#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/map/Map2D.h>
#include <base/assets/util/AssetType.h>

class AssetIdentifier {pub dCtor(AssetIdentifier);
    pub AssetType const* type;
    pub std::string id;

    priv static Map2D<int, std::string, std::shared_ptr<AssetIdentifier>> entriesMap;

    pub explicit AssetIdentifier(AssetType const* type, const std::string& id);

    pub static std::shared_ptr<AssetIdentifier> getOrCreate(AssetType const* type, std::string id);

    pub virtual ~AssetIdentifier();
};
