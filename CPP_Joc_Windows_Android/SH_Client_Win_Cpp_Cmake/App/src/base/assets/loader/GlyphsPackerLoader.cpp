#include "GlyphsPackerLoader.h"
#include <base/assets/util/AssetType.h>
#include "../loader/TextureLoader.h"
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/DataFormatException.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/exceptions/BoundsException.h>
#include <base/fileSystem/fileHandler/async/IFileHandlerAsync.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/MM.h>
#include <base/assets/loader/util/IAssetLoaderHandler.h>
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <base/util/StringUtil.h>
#include <graphics/visual/TextureArray.h>
#include <base/container/BinPackingBinaryTree.h>

std::string GlyphsPackerLoader::CacheId_packingData = "/packingData.bin";

GlyphsPackerLoader::GlyphsPackerLoader(
    IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier,
    const std::string& assetsFolderPath, std::string cacheFolderPath,
    std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath),
	config(config),
	cacheFolderPath(cacheFolderPath)
{
    //void
}

std::string& GlyphsPackerLoader::getCacheFolderPath() {
	return cacheFolderPath;
}

int GlyphsPackerLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
	case LocalStatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::Loading_PreDeps;
        //break;
    }
	case LocalStatusCode::Loading_PreDeps: {
		if (!isStatusInit) {
            initStatus_loading_preDependencies();
		}

		if (tickStatus_loadingPredeps()) {
			return LocalStatusCode::WaitingForDependencies;
		}
		
		break;
    }
	case LocalStatusCode::WaitingForDependencies: {
		if (!isStatusInit) {
			//void
		}

		if (isDependenciesLoaded()) {
			return LocalStatusCode::Loading_PostDeps;
		}

		break;
    }
	case LocalStatusCode::Loading_PostDeps: {
		if (!isStatusInit) {
			initStatus_loading_postDependencies();

			return StatusCode::Finished_Success;
		}

		throw LogicException(LOC);

		//break;
	}
	default: {
		return -1;
		//break;
    }}

	return status;
}

/*
Return:
0 - Not finished.
1 - Finished.
<0 - Error code.
*/
void GlyphsPackerLoader::initStatus_loading_preDependencies() {
	std::string cacheId = getIdentifier()->id + CacheId_packingData;

	fhAsyncRead = getHandler()->getCacheGroupsHandler()->getCacheGroup(base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL)
		->getCacheFileHandlerAsync(cacheId);
	if (fhAsyncRead == nullptr) {
		// No packing information, use a empty GlyphsPacker.

		return;
	}

	fhReadListenersList.clear();
	fhAsyncRead->addEventListener(
		base::IFileHandlerAsync::EVENT_read_binary_success->type, base::IFileHandlerAsync::EVENT_IO_failed->type,
		std::bind(&GlyphsPackerLoader::onAsyncFHEvent, this, std::placeholders::_1, std::placeholders::_2),
		fhReadListenersList
	);

	isAsyncReadInProgress = true;
	fhAsyncRead->readBinarySPAsync();
}

void GlyphsPackerLoader::onAsyncFHEvent(IEventListener& eventListener, base::AsyncFHEvent& event) {
	fhReadListenersList.clear();

	if (base::IFileHandlerAsync::EVENT_IO_failed->type == event.type) {
		// For now, just do a error.
		throw LogicException(LOC);
	} else if (base::IFileHandlerAsync::EVENT_read_binary_success->type == event.type) {
		asyncReadBuff = event.data_binary;
		event.data_binary = nullptr;
		
		fhAsyncRead = nullptr;

		isAsyncReadInProgress = false;
	} else {
		throw LogicException(LOC);
	}
}

bool GlyphsPackerLoader::tickStatus_loadingPredeps() {
	if (isAsyncReadInProgress) {
		return false;
	}

	if (asyncReadBuff == nullptr) {
		// There was no file, use a default glyphs packer.
		return true;
	}

	sp<base::StreamBuffer> sBuff = new base::StreamBuffer{ asyncReadBuff };
	asyncReadBuff = nullptr;

	// The first line is a tuple (eg: "packingTextureIndex:2").
	packingTextureIndex = sBuff->readInt();

	// Find a compatible texture array for packing.
	// Find the texture that has the required params.
	graphics::TextureArray* compatibleTexArray = nullptr;
	for (int i = 0; i < config->globalTextures->count(); i++) {
		graphics::TextureArray* ta = config->globalTextures->getDirect(i);

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

	binPackingStructure = new BinPackingBinaryTree{ compatibleTexArray->getWidth(), compatibleTexArray->getHeight() };
	try {
		binPackingStructure->parseBinary(*sBuff.get());
	} catch (BoundsException& x) {
		throw AssetMissingOrInvalidException();
	} catch (DataFormatException& x) {
		throw AssetMissingOrInvalidException();
	}

	if (binPackingStructure->getIsAnythingPacked()) {
		// The packer
		// A texture is used and needs to be laoded as a dep.
		std::shared_ptr<TextureLoader::LoaderConfig> loaderConfig = std::make_shared<TextureLoader::LoaderConfig>(
			compatibleTexArray->minFilter,
			compatibleTexArray->magFilter,
			compatibleTexArray->wrapS,
			compatibleTexArray->wrapT,
			config->globalTextures,
			// Keep the data unpacked because glyphs insertion requires that.
			false/*isDataPacked*/
		);

		std::string textureAssetId = std::string("glyphs/glyphsAtlas") + std::to_string(packingTextureIndex) + ".png";

		// getAssetsFolderPath() is not used because this asset works with the cache system only so it is never used.
		std::string depRootFolderOverride = getCacheFolderPath() + base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL + "/";

		textureDep = std::make_shared<AssetLoaderDependency>(
			AssetIdentifier::getOrCreate(AssetType::Texture, textureAssetId), getFsTarget(),
			loaderConfig,
			depRootFolderOverride
		);
		pushDependency(textureDep);
	} else {
		// There is no texture.
		textureDep = nullptr;
	}

	return true;
}

std::string GlyphsPackerLoader::computeAtlasFolderPath() {
	const std::string& atlasPath = getSubPath();

	std::size_t found = atlasPath.find_last_of("/\\");

	std::string atlasFolderPath = atlasPath.substr(0, found + 1);

	return atlasFolderPath;
}

/*
Return:
0 - Not finished.
1 - Finished.
<0 - Error code.
*/
void GlyphsPackerLoader::initStatus_loading_postDependencies() {
	data = new base::GlyphsPacker(getIdentifier()->id, getHandler(), config->globalTextures, getLoadingAsset()->asset.get());

	if (packingTextureIndex >= 0) {
		if (textureDep->loadStatus != AssetLoaderDependency::DepStatus::Finished_Success) {
			throw AssetMissingOrInvalidException();
		}

		data->initPackingTexture(packingTextureIndex, binPackingStructure, textureDep->asset);
	}
}

void* GlyphsPackerLoader::extractData() {
	base::GlyphsPacker* t = data;

	data = nullptr;

	return t;
}

void GlyphsPackerLoader::disposeMain() {
	//void

    super::disposeMain();
}

GlyphsPackerLoader::~GlyphsPackerLoader() {
	if (data) {
		delete data;

		data = nullptr;
	}
}
