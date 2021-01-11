#include "FileSystemBitmapFontLoader.h"
#include <base/assets/util/AssetType.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/assets/loader/TextureLoader.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>
#include <base/MM.h>
#include <base/app/config/AppConfig.h>

FileSystemBitmapFontLoader::FileSystemBitmapFontLoader(
    IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier,
    const std::string& assetsFolderPath,
    std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath),
    config(config)
{
    //void
}

int FileSystemBitmapFontLoader::tickStatus(int status, bool isStatusInit) {
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
            initStatus_Loading_PreDeps();

            return LocalStatusCode::WaitingForDependencies;
		}

        throw LogicException(LOC);

		//break;
    }
    case LocalStatusCode::Loading_PostDeps: {
		if (!isStatusInit) {
			initStatus_Loading_PostDeps();

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
<0 - Error code
*/
void FileSystemBitmapFontLoader::initStatus_Loading_PreDeps() {
    // Read entire file to a char*;
    std::string fullPath = getAssetsFolderPath() + getSubPath() + ".fnt";

    std::shared_ptr<std::string> atlassBuffer;
    std::shared_ptr<base::IFileHandlerBlocking> fh = base::Util_FileSystem::fs->getFileHandlerBlocking(fullPath, getFsTarget());
    try {
        atlassBuffer = fh->readCppStringSP();
    } catch (FileNotFoundException& x) {
        throw AssetMissingOrInvalidException();
    }
    if (atlassBuffer == nullptr) {
        throw LogicException(LOC);
    }

    std::string atlasFolderPath;
    if (config->isCachedPackedGlyphsUsed) {
        atlasFolderPath = "";
    } else {
        atlasFolderPath = computeAtlasFolderPath();
    }
    parser = new BitmapFontParser(atlassBuffer, atlasFolderPath, true);
    parser->parseData();

    // Push dependencies and wait for dependencies loading.
    for (int i = 0; i < parser->pagesList.size(); i++) {
        BitmapFontParser::Page& texPageConfig = *parser->pagesList.getPointer(i);

        const Texture::TextureFilterMin* filterMin = &Texture::TextureFilterMin::Nearest;
        const Texture::TextureFilterMag* filterMag = &Texture::TextureFilterMag::Nearest;
        const Texture::TextureWrap* wrap = &Texture::TextureWrap::ClampToEdge;
        if (!filterMin || !filterMag || !wrap)
        {
            throw LogicException(LOC);
        }
        if (filterMin->usesMipMapping || filterMag->usesMipMapping) {
            // Current logic doesn't suport mip mapping;
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
            AssetIdentifier::getOrCreate(AssetType::Texture, texPageConfig.file), getFsTarget(),
            loaderConfig,
            getAssetsFolderPath()
        );
        pushDependency(dep);
    }
}

/*
Return:
0 - Not finished.
1 - Finished.
<0 - Error code.
*/
void FileSystemBitmapFontLoader::initStatus_Loading_PostDeps() {
	for (int i = 0; i < parser->pagesList.size(); i++) {
        BitmapFontParser::Page &texPageConfig = *parser->pagesList.getPointer(i);

        std::string depId = texPageConfig.file;

        std::shared_ptr<AssetLoaderDependency> dep = getDependency(AssetType::Texture, depId);
        if(dep == nullptr) {
            throw LogicException(LOC);
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Failed_AssetMissingOrInvalid) {
            throw AssetMissingOrInvalidException();
        }else if(dep->loadStatus != AssetLoaderDependency::DepStatus::Finished_Success) {
            throw LogicException(LOC);
        }
    }

    data = new base::BitmapFont(
        getIdentifier()->id,
        parser->info.rendering3DSize,
        parser->common.lineHeight, parser->common.lineHeight - parser->common.base,
        parser->common.ascent, parser->common.descent, parser->common.lineGap,
        parser->kerningsCount
    );

    if (parser->buildGlyphs(data, peekLoaderDependenciesList()) != 0) {
        throw AssetMissingOrInvalidException();
    }

    delete parser;
    parser = nullptr;
}

std::string FileSystemBitmapFontLoader::computeAtlasFolderPath() {
    const std::string& atlasPath = getSubPath();
    //atlasPath is somehing like "fonts/Roboto-Medium-AaNSS-17"
    std::size_t found = atlasPath.find_last_of("/\\");

    std::string atlasFolderPath = atlasPath.substr(0, found + 1);

    return atlasFolderPath;
}

void* FileSystemBitmapFontLoader::extractData() {
    base::BitmapFont* t = data;

    data = nullptr;

    return t;
}

void FileSystemBitmapFontLoader::disposeMain() {
	//void

    super::disposeMain();
}

FileSystemBitmapFontLoader::~FileSystemBitmapFontLoader() {
	if (parser) {
		delete parser;

        parser = nullptr;
	}

	if (data) {
        data->reservedDispose();
		delete data;

		data = nullptr;
	}
}
