#include "LoaderType.h"
#include <base/assets/util/AssetType.h>
#include <base/exceptions/LogicException.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <base/statics/StaticsInit.h>

bool LoaderType::IsStaticsInit = base::StaticsInit::AddCbGeneral<bool>(0, []() {
    IsStaticsInit = true;

    LoaderType::Texture = newt LoaderType(1, AssetType::Texture, AssetType::Texture);
    LoaderType::TextureAtlas = newt LoaderType(2, AssetType::TextureAtlas, AssetType::TextureAtlas);
    LoaderType::GlyphsPacker = newt LoaderType(3, AssetType::GlyphsPacker, AssetType::GlyphsPacker);
    LoaderType::Text = newt LoaderType(4, AssetType::Text, AssetType::Text);
    LoaderType::BinaryData = newt LoaderType(5, AssetType::BinaryData, AssetType::BinaryData);
    LoaderType::Image = newt LoaderType(5, AssetType::Image, AssetType::Image);

    LoaderType::FileSystemBitmapFont = newt LoaderType(7, AssetType::Font, nullptr);
    LoaderType::RasterisingTrueTypeFont = newt LoaderType(8, AssetType::Font, nullptr);
    LoaderType::CacheableRasterisingFont = newt LoaderType(9, AssetType::Container_Font, AssetType::Font);

    LoaderType::FileSystemSound = newt LoaderType(10, AssetType::Sound, nullptr);
    LoaderType::GeneratedSound = newt LoaderType(11, AssetType::Sound, nullptr);
    LoaderType::CacheableGeneratingSound = newt LoaderType(12, AssetType::Container_Sound, AssetType::Sound);
});

ArrayList<LoaderType const*> LoaderType::entriesList{};

LoaderType const* LoaderType::Texture = nullptr;
LoaderType const* LoaderType::TextureAtlas = nullptr;
LoaderType const* LoaderType::GlyphsPacker = nullptr;
LoaderType const* LoaderType::Text = nullptr;
LoaderType const* LoaderType::BinaryData = nullptr;
LoaderType const* LoaderType::Image = nullptr;

LoaderType const* LoaderType::FileSystemBitmapFont = nullptr;
LoaderType const* LoaderType::RasterisingTrueTypeFont = nullptr;
LoaderType const* LoaderType::CacheableRasterisingFont = nullptr;

LoaderType const* LoaderType::FileSystemSound = nullptr;
LoaderType const* LoaderType::GeneratedSound = nullptr;
LoaderType const* LoaderType::CacheableGeneratingSound = nullptr;

LoaderType::LoaderType(int id, AssetType const* producedAssetType, AssetType const* defaultLoaderForAssetType)
    : id(id), producedAssetType(producedAssetType), defaultLoaderForAssetType(defaultLoaderForAssetType)
{
    entriesList.appendDirect(this);
}

LoaderType const* LoaderType::getDefaultLoaderForAssetType(AssetType const* assetType) {
    LoaderType const* ret = nullptr;
    for(int i=0;i<entriesList.size();i++) {
        LoaderType const* entry = entriesList.getDirect(i);
        if(entry->defaultLoaderForAssetType == assetType) {
            ret = entry;

            break;
        }
    }
    if(ret == nullptr) {
        throw LogicException(LOC);
    }

    return ret;
}

LoaderType::~LoaderType() {
    //void
}
