#include "Asset.h"
#include "AssetType.h"
#include <base/exceptions/LogicException.h>
#include <base/assets/glyphsPacker/GlyphsPacker.h>

using namespace base;

Asset::Asset(std::shared_ptr<AssetIdentifier>& identifier, bool isTopLevelAsset)
    : identifier(identifier), isTopLevelAsset(isTopLevelAsset)
{
    //void
}

void Asset::initData(void* data) {
    if(this->data != nullptr || data == nullptr) {
        throw LogicException(LOC);
    }

    this->data = data;
}

bool Asset::getIsTopLevelAsset() {
    return isTopLevelAsset;
}

ArrayList<std::shared_ptr<AssetIdentifier>>* Asset::getSubDependenciesList() {
    return &subDependenciesList;
}

ArrayList<std::shared_ptr<AssetIdentifier>>* Asset::getSuperDependantsList() {
    return &superDependantsList;
}

std::shared_ptr<AssetIdentifier> Asset::getSingleSubDependencYWithType(AssetType const* assetType, bool mustExist) {
    std::shared_ptr<AssetIdentifier> ret = nullptr;

    for (int i = 0; i < subDependenciesList.count(); i++) {
        std::shared_ptr<AssetIdentifier>& depAssetIdentifier = subDependenciesList.getReference(i);

        if (depAssetIdentifier->type == assetType) {
            if (ret != nullptr) {
                // Already found a asset. Multiple exist but single is expected.
                throw LogicException(LOC);
            }

            ret = depAssetIdentifier;
        }
    }

    if (mustExist && ret == nullptr) {
        throw LogicException(LOC);
    }

    return ret;
}

void Asset::connectToSubDependencyAsset(std::shared_ptr<base::Asset> subDependencyAsset) {
    if(subDependencyAsset->getSuperDependantsList()->contains(this->identifier)
    || this->getSubDependenciesList()->contains(subDependencyAsset->identifier))
    {
        throw LogicException(LOC);
    }
    // Add the super_dependant to the asset.
    subDependencyAsset->getSuperDependantsList()->appendDirect(this->getIdentifier());

    // Add the asset as a sub_dependancy to "superDependantAsset";
    this->getSubDependenciesList()->appendDirect(subDependencyAsset->getIdentifier());
}

Texture* Asset::getAsTexture() {
    if(identifier->type != AssetType::Texture) {
        throw LogicException(LOC);
    }

    return static_cast<Texture*>(data);
}

TextureAtlas* Asset::getAsTextureAtlas() {
    if(identifier->type != AssetType::TextureAtlas) {
        throw LogicException(LOC);
    }

    return static_cast<TextureAtlas*>(data);
}

GlyphsPacker* Asset::getAsGlyphsPacker() {
    if(identifier->type != AssetType::GlyphsPacker) {
        throw LogicException(LOC);
    }

    return static_cast<GlyphsPacker*>(data);
}

base::String* Asset::getAsText() {
    if(identifier->type != AssetType::Text) {
        throw LogicException(LOC);
    }

    return static_cast<base::String*>(data);
}

base::Buffer* Asset::getAsBuffer() {
    if(identifier->type != AssetType::BinaryData) {
        throw LogicException(LOC);
    }

    return static_cast<base::Buffer*>(data);
}

base::Image* Asset::getAsImage() {
    if(identifier->type != AssetType::Image) {
        throw LogicException(LOC);
    }

    return static_cast<base::Image*>(data);
}

base::BitmapFont* Asset::getAsFont() {
    if(identifier->type != AssetType::Font) {
        throw LogicException(LOC);
    }

    return static_cast<base::BitmapFont*>(data);
}

base::audio::Sound* Asset::getAsSound() {
    if(identifier->type != AssetType::Sound) {
        throw LogicException(LOC);
    }

    return static_cast<base::audio::Sound*>(data);
}

Asset::~Asset() {
    if (data != nullptr) {
        if (identifier->type == AssetType::Texture) {
            delete static_cast<Texture*>(data);
        }else if (identifier->type == AssetType::TextureAtlas) {
            delete static_cast<TextureAtlas*>(data);
        }else if (identifier->type == AssetType::GlyphsPacker) {
            delete static_cast<base::GlyphsPacker*>(data);
        }else if (identifier->type == AssetType::Text) {
            delete static_cast<base::String*>(data);
        }else if (identifier->type == AssetType::BinaryData) {
            delete static_cast<base::Buffer*>(data);
        } else if (identifier->type == AssetType::Image) {
            delete static_cast<Image*>(data);
        } else if (identifier->type == AssetType::Font) {
            delete static_cast<BitmapFont*>(data);
        } else if (identifier->type == AssetType::Sound) {
            delete static_cast<base::audio::Sound*>(data);
        }else {
            assert(false);
        }

        data = nullptr;
    }
}
