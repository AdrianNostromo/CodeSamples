#include "GlyphsPacker.h"
#include <string>
#include <graphics/visual/Texture.h>
#include <base/assets/util/AssetIdentifier.h>
#include <base/assets/util/Asset.h>
#include <base/assets/util/AssetType.h>
#include <graphics/visual/TextureArray.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/app/IApp.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <base/assets/IAssetsManager.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/util/StringUtil.h>
#include <vendor/stb/stb_image_write.h>
#include <memory>
#include <base/app/config/AppConfig.h>

using namespace base;

std::string GlyphsPacker::CacheId_packingData = "glyphs/packingData.bin";

GlyphsPacker::PackingTexture::PackingTexture(int packingTextureIndex, std::shared_ptr<base::Asset> fontDepTextureAsset, Texture* texture, sp<BinPackingBinaryTree> customGlyphsPackingTree)
	: packingTextureIndex(packingTextureIndex), fontDepTextureAsset(fontDepTextureAsset), texture(texture)
{
	if (customGlyphsPackingTree != nullptr) {
		this->glyphsPackingTree = customGlyphsPackingTree;
	} else {
		glyphsPackingTree = new BinPackingBinaryTree{ texture->getWidth(), texture->getHeight() };
	}
}

GlyphsPacker::GlyphsPacker(std::string& cacheId, IAssetsManager* assetsManager, ArrayList<graphics::TextureArray*>* globalTextures, Asset* selfAsset)
	: super(cacheId, assetsManager, globalTextures, selfAsset)
{
	//void
}

void GlyphsPacker::initPackingTexture(int packingTextureIndex, sp<BinPackingBinaryTree> customGlyphsPackingTree, std::shared_ptr<Asset> textureAsset) {
	if (packingTexturesList.count() > 0) {
		throw LogicException(LOC);
	}

	createNewPackingTexture(&packingTextureIndex, customGlyphsPackingTree, textureAsset);
}

sp<IGlyphsPacker::ComposedGlyphRegion> GlyphsPacker::getFontTextureRegion(int glyphWidth, int glyphHeight, int padding) {
	PackingTexture* packingTexture = getOrCreatePackingTexture();
	
	BinPackingBinaryTree::Node* packingRegionInfo = packingTexture->glyphsPackingTree->insert(glyphWidth + padding, glyphHeight + padding);
	if (packingRegionInfo == nullptr) {
		// Texture is full, try a new one.
		packingTexture = createNewPackingTexture();
	}
	packingRegionInfo = packingTexture->glyphsPackingTree->insert(glyphWidth + padding, glyphHeight + padding);
	if (packingRegionInfo == nullptr) {
		// Packing error occured.
		throw LogicException(LOC);
	}
	int posX = packingRegionInfo->bounds.x;
	int posY = packingRegionInfo->bounds.y;

	TextureRegion* textureRegion = newt TextureRegion(
		packingTexture->texture,
		posX + padding, posY + padding, glyphWidth, glyphHeight,
		true/*flipTextureY*/, true/*flipRegionY*/
	);

	return msp<ComposedGlyphRegion>(textureRegion, packingTexture->fontDepTextureAsset);
}

GlyphsPacker::PackingTexture* GlyphsPacker::getOrCreatePackingTexture() {
	if (packingTexturesList.count() > 0) {
		return packingTexturesList.getReference(packingTexturesList.count() - 1).get();
	}

	return createNewPackingTexture();
}

GlyphsPacker::PackingTexture* GlyphsPacker::createNewPackingTexture(int* override_packingTextureIndex, sp<BinPackingBinaryTree> customGlyphsPackingTree, std::shared_ptr<Asset> override_textureAsset) {
	if (globalTextures == nullptr) {
		throw LogicException(LOC);
	}
	
	int newPackingTextureIndex = 0;
	if (override_packingTextureIndex != nullptr) {
		if (packingTexturesList.count() > 0) {
			throw LogicException(LOC);
		}

		newPackingTextureIndex = *override_packingTextureIndex;
	} else {
		// Because the last packing texture entry is never removed (it is just replaced by a new last entry), can grab the last index from it and increment it.
		if (packingTexturesList.count() > 0) {
			newPackingTextureIndex = packingTexturesList.getReference(packingTexturesList.count() - 1)->packingTextureIndex + 1;
		}
	}

	std::shared_ptr<base::Asset> fontDepTextureAsset = nullptr;
	Texture* texture = nullptr;
	if (override_textureAsset != nullptr) {
		fontDepTextureAsset = override_textureAsset;
		
		texture = fontDepTextureAsset->getAsTexture();

		// Make the texture be unpacked because the packing logic requires unpacked textures.
		texture->setIsDataPacked(false);
	} else {
		// Find the texture that has the required params.
		graphics::TextureArray* compatibleTexArray = nullptr;
		for (int i = 0; i < globalTextures->count(); i++) {
			graphics::TextureArray* ta = globalTextures->getDirect(i);

			if (ta->minFilter == &Texture::TextureFilterMin::Nearest && ta->magFilter == &Texture::TextureFilterMag::Nearest
				&& ta->wrapS == &Texture::TextureWrap::ClampToEdge && ta->wrapT == &Texture::TextureWrap::ClampToEdge)
			{
				compatibleTexArray = ta;

				break;
			}
		}
		if (compatibleTexArray == nullptr) {
			throw LogicException(LOC);
		}

		int width = compatibleTexArray->getWidth();
		int height = compatibleTexArray->getHeight();

		base::Buffer textureData{ width * height * compatibleTexArray->getBytesPerPixel_NOT_BITS() };
		textureData.memsetData(0);

		// Use unpacked texture data for now to allow for glyphs pixels writing.
		texture = newt Texture(
			textureData, false/*isTextureDataPacked*/,
			width, height, 4/*channelsCount*/, compatibleTexArray->getBitsPerPixel_NOT_BYTES(),
			1/*mipMapLevels*/, IGL::RGBA8/*sizedInternalFormat*/,
			&Texture::TextureFilterMin::Nearest, &Texture::TextureFilterMag::Nearest,
			&Texture::TextureWrap::ClampToEdge, &Texture::TextureWrap::ClampToEdge,
			globalTextures
		);
		texture->reservedCreate();

		std::string textureAssetId = std::string("glyphs/glyphsAtlas") + std::to_string(newPackingTextureIndex) + ".png";

		std::shared_ptr<AssetIdentifier> identifier = AssetIdentifier::getOrCreate(AssetType::Texture, textureAssetId);
		fontDepTextureAsset = std::make_shared<base::Asset>(identifier, false/*isTopLevelAsset*/);
		fontDepTextureAsset->initData(texture);

		selfAsset->connectToSubDependencyAsset(fontDepTextureAsset);

		assetsManager->setAsset(fontDepTextureAsset);
	}

	sp<PackingTexture> packingTexture = new PackingTexture(newPackingTextureIndex, fontDepTextureAsset, texture, customGlyphsPackingTree);
	packingTexturesList.appendDirect(packingTexture);

	return packingTexture.get();
}

void GlyphsPacker::saveGlyphsAtlases(ArrayList<Texture*>* usedTexturesArray) {
	// Save the current packing texture if it is used in the list. This is needed in case the list texture is already saved because it got filled.
	for (int i = 0; i < usedTexturesArray->count(); i++) {
		Texture* tex = usedTexturesArray->getDirect(i);
		if (tex == nullptr) {
			continue;
		}

		sp<PackingTexture> packingTexture = getPackingTextureWithTextureIfExists(tex);
		if (packingTexture != nullptr) {
			cachePackingTexture(packingTexture);
		}
	}
}

sp<GlyphsPacker::PackingTexture> GlyphsPacker::getPackingTextureWithTextureIfExists(Texture* tex) {
	for (int i = 0; i < packingTexturesList.count(); i++) {
		sp<PackingTexture>& packingTexture = packingTexturesList.getReference(i);

		if (packingTexture->texture == tex) {
			return packingTexture;
		}
	}

	return nullptr;
}

void GlyphsPacker::cachePackingTexture(sp<PackingTexture> packingTexture) {
	{
		// Write the texture.

		// Need to get teh asset of the texture.
		// Get the texture asset.
		std::string& pageTextureAssetId = packingTexture->fontDepTextureAsset->getIdentifier()->id;

		// Make sure the image is saved as 4 channels (RGBA) png.
		stbi_flip_vertically_on_write(1);
		stbi_write_png_compression_level = 1;
		stbi_write_force_png_filter = 0;

		base::Buffer packedData = packingTexture->texture->getDataPacked(true/*packIfNeeded*/, true/*mustExist*/);

		// Create the subfolders path as needed.
		assetsManager->getCacheGroupsHandler()->getCacheGroup(base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL)
			->setCache(pageTextureAssetId, packedData);
	}

	if (packingTexturesList.getReference(packingTexturesList.count() - 1) == packingTexture) {
		// This is the active packing texture.
		// Write the .dat.
		Buffer buf = packingTexture->glyphsPackingTree->serialiseBinary(4/*prefixReservedBitesCount*/);
		buf.writeInt(0, true, packingTexture->packingTextureIndex);

		// Create the subfolders path as needed.
		assetsManager->getCacheGroupsHandler()->getCacheGroup(base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL)
			->setCache(CacheId_packingData, buf);
	} else {
		// Remove the texture dep and list entry.
		packingTexture->texture->setIsDataPacked(AppConfig::usePackedTextureDataInRam);
		assetsManager->removeAssetDependency(selfAsset->getIdentifier(), packingTexture->fontDepTextureAsset->getIdentifier());
	}
}

GlyphsPacker::~GlyphsPacker() {
	//void
}
