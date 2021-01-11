#include "AssetIdentifier.h"
#include <base/exceptions/LogicException.h>

Map2D<int, std::string, std::shared_ptr<AssetIdentifier>> AssetIdentifier::entriesMap{};

AssetIdentifier::AssetIdentifier(AssetType const* type, const std::string& id)
	: type(type), id(id)
{
	//void
}

std::shared_ptr<AssetIdentifier> AssetIdentifier::getOrCreate(AssetType const* type, std::string id) {
    if (type == nullptr) {
        throw LogicException(LOC);
    }

    std::shared_ptr<AssetIdentifier> entry = entriesMap.getDirect(type->id, id, nullptr);
    if(entry == nullptr) {
        entry = std::make_shared<AssetIdentifier>(type, id);
        entriesMap.putDirect(type->id, id, entry);
    }

    return entry;
}

AssetIdentifier::~AssetIdentifier() {
    //void
}
