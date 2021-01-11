#include "TextureAtlasLoader.h"
#include <base/assets/util/AssetType.h>
#include "../loader/TextureLoader.h"
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/MM.h>
#include <base/app/config/AppConfig.h>

TextureAtlasLoader::TextureAtlasLoader(
    IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier,
    const std::string& assetsFolderPath,
    std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath),
	config(config)
{
    //void
}

int TextureAtlasLoader::tickStatus(int status, bool isStatusInit) {
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

            return LocalStatusCode::WaitingForDependencies;
		}

        throw LogicException(LOC);

		//break;
    }
    case LocalStatusCode::Loading_PostDeps: {
		if (!isStatusInit) {
			initStatus_loading_postDependencies();

			return StatusCode::Finished_Success;
		}

        throw LogicException(LOC);

		//break;
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
void TextureAtlasLoader::initStatus_loading_preDependencies() {
	// Read entire file to a char*;
	std::string fullPath = getAssetsFolderPath() + getSubPath();

	std::shared_ptr<std::string> atlassBuffer;
    std::shared_ptr<base::IFileHandlerBlocking> fh = base::Util_FileSystem::fs->getFileHandlerBlocking(fullPath, getFsTarget());
    try {
        atlassBuffer = fh->readCppStringSP();
    }catch(FileNotFoundException& x) {
		throw AssetMissingOrInvalidException();
    }
	if (atlassBuffer == nullptr) {
		throw LogicException(LOC);
	}

	std::string atlasFolderPath = computeAtlasFolderPath();
	parser = new TextureAtlasParser(atlassBuffer, atlasFolderPath, true);
	if(parser->parseData() != 0) {
        throw LogicException(LOC);
	}

	// Push dependencies and wait for dependencies loading.
	for (int i = 0; i < parser->pagesList.size(); i++) {
        TextureAtlasParser::Page& texPageConfig = *parser->pagesList.getPointer(i);

		const Texture::TextureFilterMin* filterMin = Texture::TextureFilterMin::fromString(texPageConfig.texFilter_min);
		const Texture::TextureFilterMag* filterMag = Texture::TextureFilterMag::fromString(texPageConfig.texFilter_mag);
		const Texture::TextureWrap* wrap = nullptr;
		if (texPageConfig.texRepeat == "none") {
			wrap = &Texture::TextureWrap::ClampToEdge;
		} else {
			throw LogicException(LOC);
		}
		if (!filterMin || !filterMag || !wrap) {
            throw LogicException(LOC);
		}
		if (filterMin->usesMipMapping || filterMag->usesMipMapping) {
			// Current logic doesn't support mip mapping;
			//asdA;// Implement svg assets with exact size generation on runtime.
            throw LogicException(LOC);
		}
        std::shared_ptr<TextureLoader::LoaderConfig> loaderConfig = std::make_shared<TextureLoader::LoaderConfig>(
			filterMin,
			filterMag,
			wrap,
			wrap,
			config->globalTextures,
			AppConfig::usePackedTextureDataInRam
		);

        std::shared_ptr<AssetLoaderDependency> dep = std::make_shared<AssetLoaderDependency>(
            AssetIdentifier::getOrCreate(AssetType::Texture, texPageConfig.texPath), getFsTarget(),
            loaderConfig,
            getAssetsFolderPath()
        );
        pushDependency(dep);
	}
}

std::string TextureAtlasLoader::computeAtlasFolderPath() {
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
void TextureAtlasLoader::initStatus_loading_postDependencies() {
	data = new TextureAtlas();

	if (parser->buildRegions(data->getRegionsMap(), peekLoaderDependenciesList()) != 0) {
        throw LogicException(LOC);
	}

	delete parser;
	parser = nullptr;
}

void* TextureAtlasLoader::extractData() {
	TextureAtlas* t = data;

	data = nullptr;

	return t;
}

void TextureAtlasLoader::disposeMain() {
	//void

    super::disposeMain();
}

TextureAtlasLoader::~TextureAtlasLoader() {
	if (parser) {
		delete parser;

        parser = nullptr;
	}

	if (data) {
		delete data;

		data = nullptr;
	}
}
