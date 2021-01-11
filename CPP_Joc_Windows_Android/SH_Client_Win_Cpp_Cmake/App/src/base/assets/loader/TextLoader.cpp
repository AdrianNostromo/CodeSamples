#include "TextLoader.h"
#include <vendor/stb/stb_image.h>
#include <base/opengl/util/GLUtil.h>
#include <assert.h>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>

TextLoader::TextLoader(IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath, std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath)
{
    if(config != nullptr) {
        this->config = config;
    }else {
        this->config = std::make_shared<LoaderConfig>();
    }
}

int TextLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
	case StatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::Loading;
        //break;
    }
	case LocalStatusCode::Loading: {
        if (!isStatusInit) {
            initStatus_loading();

            return StatusCode::Finished_Success;
        }

        //void

        break;
    }
	default: {
        assert(false);

        break;
    }}

	return status;
}

void TextLoader::initStatus_loading() {
    std::string fullPath = getAssetsFolderPath() + getSubPath();

    base::String* _data;
    std::shared_ptr<base::IFileHandlerBlocking> fh = base::Util_FileSystem::fs->getFileHandlerBlocking(fullPath, getFsTarget());
    try {
        _data = fh->readStringP();
    }catch(FileNotFoundException& x) {
        throw AssetMissingOrInvalidException();
    }
    if(_data == nullptr) {
        throw LogicException(LOC);
    }

    this->data = _data;
}

void* TextLoader::extractData() {
    base::String* t = data;

	data = nullptr;

	return t;
}

void TextLoader::disposeMain() {
	//void

	super::disposeMain();
}

TextLoader::~TextLoader() {
	if (data) {
	    delete data;

		data = nullptr;
	}
}
