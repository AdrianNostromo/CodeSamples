#include "BinaryDataLoader.h"
#include <vendor/stb/stb_image.h>
#include <base/opengl/util/GLUtil.h>
#include <assert.h>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/FileNotFoundException.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/fileSystem/fileHandler/blocking/IFileHandlerBlocking.h>

BinaryDataLoader::BinaryDataLoader(IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath, std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath)
{
    if(config != nullptr) {
        this->config = config;
    }else {
        this->config = std::make_shared<LoaderConfig>();
    }
}

int BinaryDataLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
	case LocalStatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::Loading;
        //break;
    }
	case LocalStatusCode::Loading: {
        if (!isStatusInit) {
            initStatus_loading();

            return LocalStatusCode::Finished_Success;
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

void BinaryDataLoader::initStatus_loading() {
    std::string fullPath = getAssetsFolderPath() + getSubPath();

    base::Buffer* buff;
    std::shared_ptr<base::IFileHandlerBlocking> fh = base::Util_FileSystem::fs->getFileHandlerBlocking(fullPath, getFsTarget());
    try {
        buff = fh->readBinaryP();
    }catch(FileNotFoundException& x) {
        throw AssetMissingOrInvalidException();
    }
    if(buff == nullptr) {
        throw LogicException(LOC);
    }

    this->data = buff;
}

void* BinaryDataLoader::extractData() {
    base::Buffer* t = data;

	data = nullptr;

	return t;
}

void BinaryDataLoader::disposeMain() {
	//void

	super::disposeMain();
}

BinaryDataLoader::~BinaryDataLoader() {
	if (data) {
	    delete data;

		data = nullptr;
	}
}
