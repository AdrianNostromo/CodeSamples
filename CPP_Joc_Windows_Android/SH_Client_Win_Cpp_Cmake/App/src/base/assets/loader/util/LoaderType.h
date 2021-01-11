#pragma once

#include <base/gh.h>
#include <base/list/ArrayList.h>
#include <base/assets/util/AssetType.h>

class LoaderType {pub dCtor(LoaderType);
    priv static bool IsStaticsInit;

    pub static ArrayList<LoaderType const*> entriesList;

    // Textures create an Image dependency and are uploaded to VRam directly.
    pub static LoaderType const* Texture;
    pub static LoaderType const* TextureAtlas;
    pub static LoaderType const* GlyphsPacker;
    pub static LoaderType const* Text;
    pub static LoaderType const* BinaryData;
    pub static LoaderType const* Image;

    pub static LoaderType const* FileSystemBitmapFont;
    pub static LoaderType const* RasterisingTrueTypeFont;
    pub static LoaderType const* CacheableRasterisingFont;

    pub static LoaderType const* FileSystemSound;
    pub static LoaderType const* GeneratedSound;
    pub static LoaderType const* CacheableGeneratingSound;

    pub int id;
    pub AssetType const* producedAssetType;

    pub AssetType const* defaultLoaderForAssetType;

    pub explicit LoaderType(int id, AssetType const* producedAssetType, AssetType const* defaultLoaderForAssetType);

    pub static LoaderType const* getDefaultLoaderForAssetType(AssetType const* assetType);

    pub virtual ~LoaderType();
};
