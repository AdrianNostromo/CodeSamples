#include <base/math/Math.h>
#include "AM_AssetsMap.h"
#include <base/exceptions/LogicException.h>
#include <base/opengl/util/GLUtil.h>
#include <base/util/StringUtil.h>
#include <base/app/IApp.h>
#include <base/util/AppStyles.h>
#include <base/visual2D/util/VCC_BitmapLabel.h>

AM_AssetsMap::AM_AssetsMap(IApp* app)
	:super(app)
{
	//void
}

void AM_AssetsMap::createMain() {
    super::createMain();

    //void
}

std::shared_ptr<base::Asset> AM_AssetsMap::getAsset(AssetType const* type, std::string& id) {
    if (type == nullptr) {
        throw LogicException(LOC);
    }

	std::string preparedId = prepareAssetId(type, id);
    
    std::shared_ptr<base::Asset> asset = assetsMap.getDirect(type->id, preparedId, nullptr);

    return asset;
}

std::shared_ptr<base::Asset> AM_AssetsMap::getAsset(AssetIdentifier& assetIdentifier) {
    return getAsset(assetIdentifier.type, assetIdentifier.id);
}

std::string AM_AssetsMap::prepareAssetId(AssetType const* type, std::string& id) {
    if (type == AssetType::Font) {
        if (StringUtil::contains(id, "_dpFont_")) {
            //"fonts/Roboto-Medium-AaNSS-14-_dpFont_"
            std::shared_ptr<ArrayList<std::string>> idPartsList = StringUtil::split(id, "-", false);
			if (idPartsList->size() != 5) {
				throw LogicException(LOC);
			}
			std::string& fontSizeText = idPartsList->getReference(3);
			int unscaledFontSize = StringUtil::parseInt(fontSizeText);

			int bakedFontSize = (int)Math::floor(unscaledFontSize * app->getScreen()->getScreenDensityMultiplier_fonts());

            //"fonts/Roboto-Medium-AaNSS-16"
            std::string bakedId = idPartsList->getReference(0) + "-" + idPartsList->getReference(1) + "-" + idPartsList->getReference(2) + "-" + std::to_string(bakedFontSize);

			return bakedId;
		}
	}

	return id;
}

void AM_AssetsMap::setAsset(std::shared_ptr<base::Asset> asset) {
    if (!asset->getIsTopLevelAsset() && asset->getSuperDependantsList()->count() < 1) {
        // Not allowed to insert dependency assets without any dependants connected.
        throw LogicException(LOC);
    }

    if (getAsset(asset->getType(), asset->getId())) {
        throw LogicException(LOC);
    }

    AssetType const* type = asset->getType();
    assetsMap.putDirect(type->id, asset->getId(), asset);
}

bool AM_AssetsMap::removeAsset(std::shared_ptr<AssetIdentifier> identifier) {
    bool b = assetsMap.removeOptional(identifier->type->id, identifier->id);
    if(!b) {
        throw LogicException(LOC);
    }

    return true;
}

Texture* AM_AssetsMap::getTexture(std::string& textureID) {
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::Texture, textureID);
	if (!asset) {
		return nullptr;
	}

	return asset->getAsTexture();
}

base::String* AM_AssetsMap::getText(std::string& textID) {
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::Text, textID);
	if (!asset) {
		return nullptr;
	}

    return asset->getAsText();
}

base::Buffer* AM_AssetsMap::getBinaryData(std::string& id) {
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::BinaryData, id);
	if (!asset) {
		return nullptr;
	}

    return asset->getAsBuffer();
}

base::Image* AM_AssetsMap::getImage(std::string& id) {
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::Image, id);
	if (asset == nullptr) {
		return nullptr;
	}

    return asset->getAsImage();
}

base::BitmapFont* AM_AssetsMap::getFont(std::string& assetId) {
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::Font, assetId);
	if (asset == nullptr) {
		return nullptr;
	}

    return asset->getAsFont();
}

base::audio::Sound* AM_AssetsMap::getSound(std::string& assetId) {
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::Sound, assetId);
	if (asset == nullptr) {
		return nullptr;
	}

    return asset->getAsSound();
}

TextureAtlas* AM_AssetsMap::getTextureAtlas(std::string& atlasID) {
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::TextureAtlas, atlasID);
	if (!asset) {
		return nullptr;
	}

    return asset->getAsTextureAtlas();
}

base::GlyphsPacker* AM_AssetsMap::getGlyphsPacker(std::string& atlasID) {
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::GlyphsPacker, atlasID);
	if (!asset) {
		return nullptr;
	}

    return asset->getAsGlyphsPacker();
}

TextureRegion* AM_AssetsMap::getTextureAtlasRegion(std::string& atlasID, std::string& regionID) {
	TextureAtlas* textureAtlas = getTextureAtlas(atlasID);
	if (textureAtlas) {
		return textureAtlas->getRegion(regionID);
	}

	return nullptr;
}

std::shared_ptr<ArrayList<TextureRegion*>> AM_AssetsMap::getTextureAtlasRegionsArray(
    std::string& atlasID,
    std::string& regionIDPrefix, std::string& regionIDSuffix, int regionIndiceStart, int regionIndiceEnd, int indiceZeroPadding)
{
    TextureAtlas* textureAtlas = getTextureAtlas(atlasID);
    if(textureAtlas == nullptr) {
        throw LogicException(LOC);
    }

    std::shared_ptr<ArrayList<TextureRegion*>> kfList = std::make_shared<ArrayList<TextureRegion*>>();

    int currentIndex = (regionIndiceStart >= 0) ? regionIndiceStart - 1 : -1;
    while(true) {
        currentIndex++;

        std::string frameIndex = Math::intToPaddedString(currentIndex, indiceZeroPadding);
        std::string frameRegionId = regionIDPrefix + frameIndex + regionIDSuffix;

        TextureRegion* kf = textureAtlas->getRegion(frameRegionId);
        if(kf == nullptr) {
            if(regionIndiceStart < 0 && currentIndex == 0) {
                // The animation with automatic_start_index looks like it starts from frame 1 (it can start from 0 or 1).
                continue;
            }else if(regionIndiceEnd < 0 && kfList->size() > 0) {
                // The animation with automatic_end_index finished. At least on frame was loaded.
                break;
            }

            throw LogicException(LOC);
        }

        kfList->appendDirect(kf);
    }

    return kfList;
}

base::BitmapFont* AM_AssetsMap::getFont(
    std::string& fontName, int fontSize, int fontWeight, std::string& charactersRange,
    bool retNullOnBadParams, bool mustExits)
{
    if (fontName.length() <= 0 || fontSize < 1 || fontSize > 220 || fontWeight < 1 || charactersRange.length() <= 0) {
        if (retNullOnBadParams) {
            return nullptr;
        }

        throw LogicException(LOC);
    }

    //asd_01;// after this becomes the function to get fonts, modify the container to not require this name baking.

    //"fonts/Roboto-Medium-AaNSS-14"
    std::string s = fontName + "-" + VCC_BitmapLabel::GetFontWeightNameFromWeightValue(fontWeight) + "-" + charactersRange + "-" + std::to_string(fontSize);
    std::shared_ptr<base::Asset> asset = getAsset(AssetType::Font, s);
    if (asset == nullptr) {
        if (mustExits) {
            throw LogicException(LOC);
        }

        return nullptr;
    }

    return asset->getAsFont();
}


void AM_AssetsMap::disposeMain() {
    //void

    super::disposeMain();
}
